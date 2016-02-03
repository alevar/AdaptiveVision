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
#include "ActionCode.h"
#include "MessageStack.h"
#include "MessageException.h"

#define DESTINATION_ADDRESS "127.0.0.1"
#define PORT    1234
#define MAXDATASIZE 300 // max number of bytes

using namespace std;
using namespace cv;

int main(int argc , char *argv[])
{

    int socket_desc;
    struct sockaddr_in server;

    int numbytes;
    char buf[MAXDATASIZE];

    Message newmessage("GET","hello world");
    InstructionCode initialOpcode;
    InstructionCode workingOpcode;
    MessageStack instructionStack(10);

    bool waitRecv;
    string newData;

    uint32_t dataLength;
    string dataToSend;

    Message newTestMessage;
    Message testMessage("NEW_VALUE", "hello world");

    VideoCapture source(0); // open the default camera
    if(!source.isOpened())  // check if we succeeded
        return -1;

    Mat image;
    source >> image;

    image = (image.reshape(0,1));
    int  imgSize = image.total()*image.elemSize();
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
         
    server.sin_addr.s_addr = inet_addr(DESTINATION_ADDRESS);
    server.sin_family = AF_INET;
    server.sin_port = htons( PORT );
 
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
     
    puts("Connected\n");

    if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
    {
        perror("recv()");
        exit(1);
    }
    else
        buf[numbytes] = '\0';
        printf("Client-Received: %s", buf);

    // send(socket_desc,&dataLength ,sizeof(uint32_t) ,MSG_CONFIRM); // Send the data length
    // send(socket_desc,dataToSend.c_str(),dataToSend.size(),MSG_CONFIRM);
    // write(socket_desc , test , test.size());

    newTestMessage.toMessage(buf);
    cout << "new OPCODE is:     " << newTestMessage.getOpCode() << "    in the MESSAGE: " << newData << endl;
    instructionStack.push(newTestMessage.getOpCode());


    while(true){

        // opcode = inst->MessageOpCode::getOpCode();

        workingOpcode = instructionStack.pop();

        // The problem is that the receive operation is late
        // Need to have a bool condition that says if a send was submitted the opration of the application
        // should be halted before receive happend

        // needs to take into account the difference between different OpCodes - some will not require a response
        // from the server
        

        switch(workingOpcode){
            case PUT:
            {
                cout << "CASE: PUT" << endl;
                break;
            }
            case GET:
            {
                cout << "CASE: GET" << endl;
                dataLength = newmessage.getLength();
                dataToSend = newmessage.getInfo();
                // send(socket_desc,&dataLength ,sizeof(uint32_t) ,MSG_CONFIRM); // Send the data length
                // send(socket_desc,dataToSend.c_str(),dataToSend.size(),MSG_CONFIRM);
                send(socket_desc, image.data, imgSize, 0);
                
                waitRecv = true;

                while(waitRecv){

                    if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
                    {
                        perror("recv()");
                        exit(1);
                    }
                    else
                        buf[numbytes] = '\0';
                        newData = buf;
                        cout << "Client Received: " << newData << endl;
                        waitRecv = false;

                }

                newTestMessage.toMessage(newData);
                cout << "new OPCODE is:     " << newTestMessage.getOpCode() << "    in the MESSAGE: " << newData << endl;

                instructionStack.push(newTestMessage.getOpCode());
                break;
            }
            case ACK:
            {
                cout << "CASE: ACK" << endl;
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

                string test1 = testMessage.toString();
                cout<<test1<<endl<<endl;

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
                dataLength = newmessage.getLength();
                dataToSend = newmessage.getInfo();
                // send(socket_desc,&dataLength ,sizeof(uint32_t) ,MSG_CONFIRM); // Send the data length
                send(socket_desc,dataToSend.c_str(),dataToSend.size(),MSG_CONFIRM);
                waitRecv = true;

                while(waitRecv){

                    if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
                    {
                        perror("recv()");
                        exit(1);
                    }
                    else
                        buf[numbytes] = '\0';
                        newData = buf;
                        cout << "Client Received: " << newData << endl;
                        waitRecv = false;

                }

                newTestMessage.toMessage(newData);
                cout << "new OPCODE is:     " << newTestMessage.getOpCode() << "    in the MESSAGE: " << newData << endl;

                instructionStack.push(newTestMessage.getOpCode());
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