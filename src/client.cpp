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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h> //inet_addr
#include <unordered_map>

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
#include "MessageCompress.h"
#include "Template.h"
#include "Match.h"

#define DESTINATION_ADDRESS "127.0.0.1"
#define PORT    1234
#define MAXDATASIZE 3000000 // max number of bytes

using namespace std;
using namespace cv;

void readme();
void processGet();
void processPut();
void processEnd();

int main(int argc , char *argv[])
{

	if( argc != 3 )
  	{
  		readme();
  		return -1;
  	}

/*=======================================================
======================= QUEUE TEST ======================
=======================================================*/ 
	
	// Message testMessage0("GET", "hello");
	// Message testMessage1("PUT", "hello");
	// MessageQueue testQueue(10);
	// testQueue.push(testMessage0);
	// testQueue.push(testMessage1);

/*=======================================================
================== GLOBAL VARIABLES =====================
=======================================================*/

	const char * destinationAddress = argv[1];
	int destinationPort = atoi(argv[2]);

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

/*=======================================================
=======================================================*/

/*=======================================================
================== CAM INITIALIZATION ===================
=======================================================*/

	VideoCapture source(0); // open the default camera
	if(!source.isOpened())  // check if we succeeded
	{
		return -1;
	}

	Mat image;
	source >> image;

	// image = imread("/home/sparrow/Pictures/Webcam/logoT.jpg",0);

	cout << "Width : " << image.size().width << endl;
	cout << "Height: " << image.size().height << endl;

	image = (image.reshape(0,1));
	int  imgSize = image.total()*image.elemSize();

/*=======================================================
=======================================================*/
	 
/*=======================================================
================== CREATING SOCKET +=====================
=======================================================*/

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		cout << "COULD NOT CREATE SOCKET" << endl;
	}
		 
	server.sin_addr.s_addr = inet_addr(destinationAddress);
	server.sin_family = AF_INET;
	server.sin_port = htons( destinationPort );

	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		cout << "CONNECT ERROR" << endl;
		return 1;
	}
	 
	cout << "CONNECTED" << endl;

/*=======================================================
=======================================================*/

/*=======================================================
===================== READY SIGNAL ======================
=======================================================*/

	// The RECV statement below will not let the application advance unless a ready status was received

	// Need to write a way to make sure a READY always gets received to begin the protocol
	// Otherwise if first RECV != READY it should wait for READY
	// Or perhaps it can ask the server if it is READY

	if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
	{
		perror("recv()");
		exit(1);
	}
	else
		buf[numbytes] = '\0';

	// send(socket_desc,&dataLength ,sizeof(uint32_t) ,MSG_CONFIRM); // Send the data length
	// send(socket_desc,dataToSend.c_str(),dataToSend.size(),MSG_CONFIRM);
	// write(socket_desc , test , test.size());

	interimMessage.toMessage(buf);
	cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;
	instructionStack.push(interimMessage);

/*=======================================================
=======================================================*/

/*=======================================================
===================== SWITCH LOOP =======================
=======================================================*/

	while(connectionStatus){

		source >> image;

		// image = imread("/home/sparrow/Pictures/Webcam/logoT.jpg",0);

		cout << "Width : " << image.size().width << endl;
		cout << "Height: " << image.size().height << endl;

		image = (image.reshape(0,1));
		int  imgSize = image.total()*image.elemSize();

		// opcode = inst->MessageOpCode::getOpCode();

		workingMessage = instructionStack.pop();

		workingOpcode = workingMessage.getOpCode();

		// The problem is that the receive operation is late
		// Need to have a bool condition that says if a send was submitted the opration of the application
		// should be halted before receive happend

		// needs to take into account the difference between different OpCodes - some will not require a response
		// from the server
		

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
				=======================================================
				============ EXAMPLE OF SENDING OPENCV MAT ============
				=======================================================
				Mat frame;
				frame = (frame.reshape(0,1)); // to make it continuous

				int  imgSize = frame.total()*frame.elemSize();

				// Send data here
				bytes = send(clientSock, frame.data, imgSize, 0))
				=====================================================*/

				cout << "CASE: PUT" << endl;

				Message messagePUT("PUT","hello");
				Message messagePUT2("PUT","hello2");

				putData = messagePUT.toString();
				string putData2 = messagePUT2.toString();

				send(socket_desc,putData.c_str(),putData.size(),MSG_CONFIRM);

				// send(socket_desc, image.data, imgSize, MSG_CONFIRM);
				
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

					instructionStack.push(interimMessage);

				}

				waitRecv = true;

				cout << "IMAGE SIZE: " << imgSize << endl;

				send(socket_desc,image.data,imgSize,MSG_CONFIRM);

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

						cout << incomingData << endl;
						interimMessage.toMessage(incomingData);
					}

				}

				if(interimMessage.getOpCode() == ACK){
					
					cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;

					instructionStack.push(interimMessage);

				}

				else{
					cout << "ERROR IN ACK" << endl;
				}

				
				// connectionStatus = false;
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
				=======================================================
				=========== EXAMPLE OF RECEIVENG OPENCV MAT ===========
				=======================================================
				// Mat  img = Mat::zeros( height,width, CV_8UC3);
				// int  imgSize = img.total()*img.elemSize();
				// uchar sockData[imgSize];

				//  //Receive data here

				// for (int i = 0; i < imgSize; i += bytes) {
				//     if ((bytes = recv(connectSock, sockData +i, imgSize  - i, 0)) == -1) {
				//         quit("recv failed", 1);
				//     }
				// }

				//  // Assign pixel value to img

				// int ptr=0;
				// for (int i = 0;  i < img.rows; i++) {
				//     for (int j = 0; j < img.cols; j++) {                                     
				//         img.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr+ 0],sockData[ptr+1],sockData[ptr+2]);
				//         ptr=ptr+3;
				//     }
				// }
				=====================================================*/

				cout << "CASE: GET" << endl;

				getData = messageGET.toString();
				send(socket_desc,getData.c_str(),getData.size(),MSG_CONFIRM);

				// send(socket_desc,&dataLength ,sizeof(uint32_t) ,MSG_CONFIRM); // Send the data length
				// send(socket_desc,dataToSend.c_str(),dataToSend.size(),MSG_CONFIRM);
				// send(socket_desc, image.data, imgSize, 0);
				
				waitRecv = true;

				while(waitRecv){

					if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
					{
						perror("recv()");
						exit(1);
					}
					else
						buf[numbytes] = '\0';
						incomingData = buf;
						waitRecv = false;

				}

				interimMessage.toMessage(incomingData);
				cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;

				instructionStack.push(interimMessage);
				// connectionStatus = false;
				break;
			}
			case ACK:
			{
				cout << "CASE: ACK" << endl;

				InstructionCode testCode;
				Message testOpCode("PUT","lul");
				testCode = testOpCode.getOpCode();                
				instructionStack.push(testOpCode);

				// connectionStatus = false;
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
				// Provide the new value to the color detection algorithm
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
				instructionStack.push(testOpCode);

				// dataLength = newmessage.getLength();
				// dataToSend = newmessage.getInfo();
				// send(socket_desc,&dataLength ,sizeof(uint32_t) ,MSG_CONFIRM); // Send the data length
				// send(socket_desc,dataToSend.c_str(),dataToSend.size(),MSG_CONFIRM);
				// waitRecv = true;

				// while(waitRecv){

				//     if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
				//     {
				//         perror("recv()");
				//         exit(1);
				//     }
				//     else
				//         buf[numbytes] = '\0';
				//         incomingData = buf;
				//         cout << "Client Received: " << incomingData << endl;
				//         waitRecv = false;

				// }

				// interimMessage.toMessage(incomingData);
				// cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;

				// Transmit the information to the server as the next package
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

				//=====================================================
				//========= EXAMPLE OF RECEIVENG OPENCV MAT ===========
				//=====================================================
				// Mat  img = Mat::zeros( height,width, CV_8UC3);
				// int  imgSize = img.total()*img.elemSize();
				// uchar sockData[imgSize];

				//Receive data here

				// for (int i = 0; i < imgSize; i += bytes) {
				//     if ((bytes = recv(connectSock, sockData +i, imgSize  - i, 0)) == -1) {
				//         quit("recv failed", 1);
				//     }
				// }

				//  // Assign pixel value to img

				// int ptr=0;
				// for (int i = 0;  i < img.rows; i++) {
				//     for (int j = 0; j < img.cols; j++) {                                     
				//         img.at<cv::Vec3b>(i,j) = cv::Vec3b(sockData[ptr+ 0],sockData[ptr+1],sockData[ptr+2]);
				//         ptr=ptr+3;
				//     }
				// }

				// while(waitRecv2){

				//     if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
				//     {
				//         perror("recv()");
				//         exit(1);
				//     }

				//     else{

				//         buf[numbytes] = '\0';
				//         incomingData = buf;
				//         cout << "Client Received: " << incomingData << endl;

				//         finalNEW_VALUE = stoi(incomingData,nullptr);

				//         waitRecv2 = false;
				//     }
				// }

				interimMessage.toMessage(incomingData);
				cout << "new OPCODE is: " << interimMessage.getOpCode() << " in the MESSAGE: " << incomingData << endl;

				instructionStack.push(interimMessage);

				// receive the length of the comming package
				
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
	 
	return 0;
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