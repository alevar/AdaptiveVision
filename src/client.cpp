/*==================================================

execution order for the client:
- The MessageStack object is available to all threads on the client side
- The messaging thread goes to sleep when the stack object is empty
- Other threads may push instructions onto the stack at any time
- When another thread pushes an object onto the stack and the messaging thread
	is known to be asleep, the messaging thread is notifyid to wake up
- the messaging thread executes the top instruction on the stack
- The messaging thread checks if there are any other items on the stack after successful execution of the task
- if there are no other instructions - messaging thread goes to sleep

==================================================*/

/*====================================================================
=========================== FULL PROCEDURE ===========================
======================================================================
=== 1. ==> Wait for the READY/; signal from the server
=== 2. ==> 
====================================================================*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>
#include <unordered_map>
#include <getopt.h>
#include <sys/stat.h>
#include <typeinfo>
#include <thread>
#include <mutex>
#include <cmath>

#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"

#include "MessageOpCode.h"
#include "Message.h"
#include "PID.h"
#include "MessageStack.h"
#include "MessageQueue.h"
#include "MessageException.h"
#include "Histogram.h"
#include "CameraDetect.h"
#include "ObjectIdentification.h"
#include "Compressor.h"
#include "Decompressor.h"
#include "Template.h"
#include "MatchMSER.h"
#include "MatchHSV.h"
#include "Sample.h"

#define DESTINATION_ADDRESS "127.0.0.1"
#define PORT    1234
#define MAXDATASIZE 3000000 // max number of bytes

using namespace std;
using namespace cv;

void readme();
void processGet();
void processPut();
void processEnd();
void protocol(int, Mat*, MessageStack*, MatchHSV*);
void mainCam(MatchHSV*, Mat*, Mat*);
void secCam(MatchHSV*, Mat*, Mat*);

vector<int> updatedHSV;

bool imageReady = false;

VideoCapture source(1); // open the default camera
Mat image;
VideoCapture sourceSec(2); // open the default camera
Mat imageSec;

vector<vector<Point> > contours1;
vector<vector<Point> > contours2;
vector<Vec4i> hierarchy1;
vector<Vec4i> hierarchy2;
Mat canny_output1;
Mat canny_output2;

Point2f largestCenter1;
float largestRadius1;
vector<float> largestContour1;
int largest_contour_index1;
double largest_area1;
Point2f largestCenter2;
float largestRadius2;
vector<float> largestContour2;
int largest_contour_index2;
double largest_area2;

Thresh thresh;
Scalar color(0,255,0);

int main(int argc , char *argv[])
{
	int c;
    int digit_optind = 0;

    int serverPort;

    int destinationPort;
    char * destinationAddress;

   	while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"port",     required_argument, 0,  0 },
            {"addr",     required_argument, 0,  0 },
            {0,         0,                 0,  0 }
        };

       	c = getopt_long(argc, argv, "t:d:0", long_options, &option_index);
        if (c == -1){
        	destinationPort = PORT;
            break;
        }

       	switch (c) {
	        case 0:
	            if (optarg){
	            	if (long_options[option_index].name == "port"){
		            	destinationPort = atoi(optarg);
		            	printf("PORT SET TO: %s\n", optarg );
		            }
		            if (long_options[option_index].name == "addr"){
		            	cout << "==========HI+++++++++++" << endl;
		            	destinationAddress = optarg;
		            	printf("ADDR SET TO: %s\n", destinationAddress);
		            }
		        }
	            break;

	       	case '?':
	            break;

	       	default:
	            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

   	if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

/*=======================================================
================== GLOBAL VARIABLES =====================
=======================================================*/

	cout << "Add: " << destinationAddress << " Port: " << destinationPort << endl;

	int socket_desc;
	struct sockaddr_in server;

	bool connectionStatus = true;

	int numbytes;
	char buf[MAXDATASIZE];

	Message messageGET("GET");
	Message messageREADY("READY");

	InstructionCode initialOpcode;
	InstructionCode workingOpcode;
	Message workingMessage;
	Message interimMessage;
	MessageStack instructionStack(10);

	bool waitRecv;
	bool waitRecv2;
	string incomingData;

	uint32_t dataLength;
	string getData;
	string putData;

	int lengthToReceive;
	int finalNEW_VALUE;

	int widthToSend;
	int heightToSend;
	string resolution;

	
	if(!source.isOpened()){
		return -1;
	}
	if(!sourceSec.isOpened()){
		return -1;
	}
	
	Mat imageToSend;

	for (int i = 0; i < 24; ++i){
		source >> image;
		sourceSec >> imageSec;
	}
	source >> image;
	sourceSec >> imageSec;

	Histogram hist(image);
	hist.calcHisHSV();
	MatchHSV match(&image);
	MatchHSV matchSec(&imageSec);

	Mat imageSampleClone;
	Mat imageSample;
	Mat imageSampleCloneSec;
	Mat imageSampleSec;

	bool foundMatch = false;

	while(!foundMatch){

		source >> image;

		try{
			match.compute();
			match.extractSample();
			imageSample = match.getSampleMAT();
			imageSampleClone = imageSample.clone();
			foundMatch = true;
		}

		catch(const std::overflow_error& e) {
			cout << "OVERFLOW ERROR: " << e.what() << endl;
		}

		catch(const std::runtime_error& e) {
			cout << "RUNTIME ERROR: " << e.what() << endl;
		}

		catch( char const* e ) {
		     cout << "GENERAL EXCEPTION: " <<endl;
		}
	}

	cout << "Width : " << imageSampleClone.size().width << endl;
	cout << "Height: " << imageSampleClone.size().height << endl;

	imageToSend = (imageSampleClone.reshape(0,1));
	int  imgSize = imageSampleClone.total()*imageSampleClone.elemSize();

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1){
		cout << "COULD NOT CREATE SOCKET" << endl;
	}
		 
	server.sin_addr.s_addr = inet_addr(destinationAddress);
	server.sin_family = AF_INET;
	server.sin_port = htons( destinationPort );

	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0){
		cout << "CONNECT ERROR" << endl;
		return 1;
	}
	 
	cout << "CONNECTED" << endl;

	if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1){
		perror("recv()");
		exit(1);
	}
	else
		buf[numbytes] = '\0';

	interimMessage.toMessage(buf);
	cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;
	instructionStack.push(interimMessage);
	
	thread t1(protocol,socket_desc,&imageSampleClone, &instructionStack, &match);
	while(connectionStatus){

		thread t2(mainCam,&match,&imageSampleClone,&imageSample);
		thread t3(secCam,&matchSec,&imageSampleCloneSec,&imageSampleSec);
		t2.join();
		t3.join();
	}
	 
	return 0;
}

void secCam(MatchHSV *match, Mat *imageSampleClone, Mat *imageSample){

	cout << "DEBUG 1" << endl;

	sourceSec >> imageSec;
	cout << "DEBUG 2" << endl;

	Mat inputSCN2_THRESH;

	if(!updatedHSV.empty()){

		cvtColor(imageSec, inputSCN2_THRESH, COLOR_BGR2HSV);
		cout << "DEBUG 3" << endl;

	    inRange(inputSCN2_THRESH, Scalar(updatedHSV[0], updatedHSV[2], updatedHSV[4]), Scalar(updatedHSV[1], updatedHSV[3], updatedHSV[5]), inputSCN2_THRESH);
	    cout << "DEBUG 4" << endl;

	    erode(inputSCN2_THRESH, inputSCN2_THRESH, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
	    dilate(inputSCN2_THRESH, inputSCN2_THRESH, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)));
	    cout << "DEBUG 5" << endl;

	    Canny(inputSCN2_THRESH, canny_output2, thresh.minThresh, thresh.minThresh*2, 3 );
	    cout << "DEBUG 6" << endl;
	    findContours(canny_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	    cout << "DEBUG 7" << endl;

	    vector<vector<Point>> contours_poly2(contours2.size());
	    vector<Point2f> center2(contours2.size());
	    vector<float> radius2(contours2.size());

	    if(contours2.size()>0){

	        for( int i = 0; i < contours2.size(); i++ )
	        {
	            double area2 = contourArea(contours2[i],false);
	            if(area2>largest_area2){
	                largest_area2 = area2;
	                largest_contour_index2 = i;
	            }
	            if(i == contours2.size()-1){
	                minEnclosingCircle( Mat (contours2[largest_contour_index2]), largestCenter2, largestRadius2); // Allows better estimation of the real size of the object, independent of the rotation
	                largestContour2 = {largestCenter2.x,largestCenter2.y, largestRadius2};
	            }
	        }
	    }
	    cout << "DEBUG 8" << endl;

	    // circle( imageSec, Point2f(largestContour2[0],largestContour2[1]), (int)largestContour2[2], color, 2, 8, 0 );
	    cout << "DEBUG 9" << endl;
	    // finalAngle2 = ((double)largestContour2[0]/ratioT)-((double)viewAngle/2)+90;

	    // finalAngle2Rad = (finalAngle2*M_PI)/180;
	}
}

void mainCam(MatchHSV *match, Mat *imageSampleClone, Mat *imageSample){
	bool foundMatch = false;
	Mat inputSCN1_THRESH;

	while(!foundMatch){
		source >> image;

		if(!updatedHSV.empty()){

			cvtColor(image, inputSCN1_THRESH, COLOR_BGR2HSV);
			inRange(inputSCN1_THRESH, Scalar(updatedHSV[0], updatedHSV[2], updatedHSV[4]), Scalar(updatedHSV[1], updatedHSV[3], updatedHSV[5]), inputSCN1_THRESH);
			erode(inputSCN1_THRESH, inputSCN1_THRESH, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)));
		    dilate(inputSCN1_THRESH, inputSCN1_THRESH, getStructuringElement(MORPH_ELLIPSE, Size(20, 20)));
		    cout << "DEBUG 5" << endl;

		    Canny(inputSCN1_THRESH, canny_output1, thresh.minThresh, thresh.minThresh*2, 3 );
		    cout << "DEBUG 6" << endl;
		    findContours(canny_output1, contours1, hierarchy1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		    cout << "DEBUG 7" << endl;

		    vector<vector<Point>> contours_poly1(contours1.size());
		    vector<Point2f> center1(contours1.size());
		    vector<float> radius1(contours1.size());

		    if(contours1.size()>0){

		        for( int i = 0; i < contours1.size(); i++ )
		        {
		            double area1 = contourArea(contours1[i],false);
		            if(area1>largest_area1){
		                largest_area1 = area1;
		                largest_contour_index1 = i;
		            }
		            if(i == contours1.size()-1){
		                minEnclosingCircle( Mat (contours1[largest_contour_index1]), largestCenter1, largestRadius1); // Allows better estimation of the real size of the object, independent of the rotation
		                largestContour1 = {largestCenter1.x,largestCenter1.y, largestRadius1};
		            }
		        }
		    }
		    cout << "DEBUG 8" << endl;

		}

		try{
			match->compute();
			match->extractSample();
			*imageSample = match->getSampleMAT();
			*imageSampleClone = imageSample->clone();
			imageReady = true;

			foundMatch = true;
		}

		catch(const std::overflow_error& e) {
			cout << "OVERFLOW ERROR: " << e.what() << endl;
		}

		catch(const std::runtime_error& e) {
			cout << "RUNTIME ERROR: " << e.what() << endl;
		}

		catch( char const* e ) {
		     cout << "GENERAL EXCEPTION: " << endl;
		}
	}
}

void protocol(int socket_desc, Mat *sample, MessageStack *instructionStack, MatchHSV *match){

	Mat imageToSend;

	bool connectionStatus = true;

	int numbytes;
	char buf[MAXDATASIZE];

	Message messageGET("GET");
	Message messageREADY("READY");

	InstructionCode workingOpcode;
	Message workingMessage;
	Message interimMessage;

	bool waitRecv;
	string incomingData;

	uint32_t dataLength;
	string getData;
	string putData;

	int lengthToReceive;
	int finalNEW_VALUE;

	int widthToSend;
	int heightToSend;
	string resolution;

	int  imgSize;

	while(connectionStatus){

		if(imageReady){
			Mat *workingSample = new Mat();
			*workingSample = sample->clone();

			imshow("WORKING SAMPLE", *workingSample);
			if(waitKey(30) >= 0){
				break;
			}

			widthToSend = int(workingSample->size().width);
			heightToSend = int(workingSample->size().height);

			imageToSend = (workingSample->reshape(0,1));
			imgSize = (workingSample->total())*(workingSample->elemSize());

			workingMessage = instructionStack->pop();
			workingOpcode = workingMessage.getOpCode();

			switch(workingOpcode){
				case PUT:
				{
					/*=====================================================
					============== SUBMITTING A PUT REQUEST ===============
					=======================================================
					===     Submit: PUT/type/len;
					=======================================================
					=========== RECEIVING A RESPONSE FROM PUT =============
					=======================================================
					=== 1. ==> wait for ACK/len/;
					=== 2. ==> push ACK to instructionStack
					=======================================================
					================ INSIDE THE ACK CASE ==================
					=======================================================
					=== 1. ==> verify the original length to the one received by the server
					=====================================================*/

					cout << "CASE: PUT" << endl;

					resolution = to_string(widthToSend)+"&"+to_string(heightToSend);

					Message messagePUT("PUT",resolution);
					Message messagePUT2("PUT","hello2");

					putData = messagePUT.toString();
					string putData2 = messagePUT2.toString();

					send(socket_desc,putData.c_str(),putData.size(),MSG_CONFIRM);
					
					waitRecv = true;

					while(waitRecv){

						if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
						{
							perror("recv()");
							exit(1);
						}
						else
						{
							buf[numbytes] = '\0';
							incomingData = buf;
							waitRecv = false;

							cout << "READY: " << incomingData << endl;
						}

					}

					if(interimMessage.getOpCode() == READY){

						interimMessage.toMessage(incomingData);
						cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;

						instructionStack->push(interimMessage);

					}

					waitRecv = true;

					cout << "IMAGE SIZE: " << imgSize << endl;

					send(socket_desc,imageToSend.data,imgSize,MSG_CONFIRM);

					// memset(&buf[0], 0, sizeof(buf));

					while(waitRecv){

						if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1){
							perror("recv()");
							exit(1);
						}
						else{
							buf[numbytes] = '\0';
							incomingData = buf;
							waitRecv = false;
							cout << incomingData << endl;
							interimMessage.toMessage(incomingData);
						}
					}

					if(interimMessage.getOpCode() == ACK){
						
						cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;

						instructionStack->push(interimMessage);

					}

					else{
						cout << "ERROR IN ACK" << endl;
					}

					break;
				}
				case GET:
				{

					/*=====================================================
					============== SUBMITTING A GET REQUEST ===============
					=======================================================
					=== 1. ==> submit: GET/;
					=======================================================
					=========== RECEIVING A RESPONSE FROM GET =============
					=======================================================
					=== 1. ==> wait for LEN/len/;
					=== 2. ==> push LEN to instructionStack
					=======================================================
					================ INSIDE THE LEN CASE ==================
					=======================================================
					=== 1. ==> prepare to receive a new message of the specified length
					=== 2. ==> send READY/; signal to the server
					=== 3. ==> receive NEW_VALUE/new value/;
					=== 4. ==> push NEW_VALUE to instructionStack
					======================================================*/

					cout << "CASE: GET" << endl;

					getData = messageGET.toString();
					send(socket_desc,getData.c_str(),getData.size(),MSG_CONFIRM);
					
					waitRecv = true;

					while(waitRecv){

						if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1){
							perror("recv()");
							exit(1);
						}
						else{
							buf[numbytes] = '\0';
							incomingData = buf;
							waitRecv = false;
						}
					}

					interimMessage.toMessage(incomingData);
					cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;

					instructionStack->push(interimMessage);
					break;
				}
				case ACK:
				{
					cout << "CASE: ACK" << endl;
					cout << "ACK MESSAGE " << workingMessage.getInfo() << endl;

					if(workingMessage.getInfo() != "null"){
						istringstream ss(workingMessage.getInfo());

						int i;

						updatedHSV.clear();
						
						while(ss >> i){
							updatedHSV.push_back(i);

					        if (ss.peek() == '&')
					            ss.ignore();
							cout << "===================== TOKEN: " << i << endl;
						}

						match->setHSV(updatedHSV[0],updatedHSV[1],updatedHSV[2],updatedHSV[3],updatedHSV[4],updatedHSV[5]);
					}

					InstructionCode testCode;
					Message testOpCode("PUT","lul");
					testCode = testOpCode.getOpCode();                
					instructionStack->push(testOpCode);

					break;
				}
				case DEFAULT:
				{
					cout << "CASE: DEFAULT" << endl;
					break;
				}
				case NEW_VALUE:
				{
					cout << "CASE: NEW_VALUE" << endl;

					cout << workingMessage.toString() << " INFO: " << workingMessage.getInfo() << endl;

					finalNEW_VALUE = stoi(workingMessage.getInfo(),nullptr);

					cout << "FINAL NEW_VALUE: " << finalNEW_VALUE << endl;

					connectionStatus = false;
					break;
				}
				case NOTHING:
				{
					cout << "CASE: NOTHING" << endl;
					break;
				}
				case ERROR:
				{
					cout << "CASE: ERROR" << endl;
					break;
				}
				case READY:
				{
					cout << "CASE: READY" << endl;

					InstructionCode testCode;
					Message testOpCode("PUT","lul");
					testCode = testOpCode.getOpCode();                
					instructionStack->push(testOpCode);

					break;
				}
				case CONTINUE:
				{
					cout << "CASE: CONTINUE" << endl;
					// start the new GET or PUT procedure
					break;
				}
				case CONFIRM:
				{
					cout << "CASE: CONFIRM" << endl;
					break;
				}
				case AGAIN:
				{
					cout << "CASE: AGAIN" << endl;
					// resubmit the previous request
					break;
				}
				case LEN:
				{
					cout << "CASE: LEN" << endl;

					lengthToReceive = stoi(workingMessage.getInfo(),nullptr);

					send(socket_desc,messageREADY.toString().c_str(),messageREADY.toString().size(),MSG_CONFIRM);
					cout << "READY TO RECEIVE" << endl;

					waitRecv = true;

					while(waitRecv){

						if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
						{
							perror("recv()");
							exit(1);
						}

						else{

							buf[numbytes] = '\0';
							incomingData = buf;
							cout << "Client Received: " << incomingData << endl;

							waitRecv = false;
						}
					}

					interimMessage.toMessage(incomingData);
					cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;

					instructionStack->push(interimMessage);
					
					break;
				}
				case TIME:
				{
					cout << "CASE: TIME" << endl;
					break;
				}
				default:
				{
					cout << "DEFAULT" << endl;
					continue;
				}
			}
		}
		// delete workingSample;
	}
}

void readme()
{
	std::cout << "Need additional arguments: IP PORT" << std::endl;
}

void processGet(){

}

void processPut(){

}

void processEnd(){

	// Meant to set connectionStatus = false; and empty all the holders for the used values

}