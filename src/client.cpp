/*

Responsibilities of the Client:
1. PUT:
- ask the server if its ready
- send information to the server application
- receive acknowledgement
2. GET:
- ask the server if the data is ready
- accept the data
- supply new data to the client application
- send acknowledgement

*/


/*

Perhaps provide means for writing out a procedure client/server would like followed.
The procedure could be written out as an expression composed of the OpCodes and arguments.

OpCodes need to be tokenized, pushed onto a stack.
A parser and lexer would be responsible for analyzing the tokens on the stack.

*/


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

#include "MessageOpCode.h"
#include "Message.h"

#define DESTINATION_ADDRESS "127.0.0.1"
#define PORT    1234
#define MAXDATASIZE 300 // max number of bytes

using namespace std;


int main(int argc , char *argv[])
{
    Message testMessage("ERROR");

    int socket_desc;
    struct sockaddr_in server;

    char *message;
     
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

    puts("Data Send\n");

    int numbytes;
    char buf[MAXDATASIZE];

    if((numbytes = recv(socket_desc, buf, MAXDATASIZE-1, 0)) == -1)
    {
        perror("recv()");
        exit(1);
    }
    else
        printf("Client-The recv() is OK...\n");
     
    buf[numbytes] = '\0';
    printf("Client-Received: %s", buf);

    message = "GET";
    write(socket_desc , message , strlen(message));

    // Need to split the string provided by the server
    // DEduce the number of operand from there
    // Compare to the language standard provided in the OpCode class

    MessageOpCode first("NEW_VALUE",3);
    cout<< first.toString() << endl;

    InstructionCode opcode;
    int operand;
    MessageOpCode *inst;
    inst = &first;
    opcode = inst->MessageOpCode::getOpCode();
    operand = inst->MessageOpCode::getOperand();

    cout << "OPERAND:  " << operand << endl;

    switch(opcode){
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
        }
    }
     
    return 0;
}