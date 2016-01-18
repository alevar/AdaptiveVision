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

#include "OpCode.h"

#define DESTINATION_ADDRESS "127.0.0.1"
#define PORT    1234
#define MAXDATASIZE 300 // max number of bytes

int main(int argc , char *argv[])
{
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

    OpCode first("DEFAULT");
    cout<< first.toString() << endl;

    InstructionCode opcode;
    int operand;
    OpCode *inst;
    inst = &first;
    opcode = inst->OpCode::getOpCode();
    // operand = inst->OpCode::getOperand();

    switch(opcode){
        case ACK:
        {
            cout << "HI" << endl;
            break;
        }
        case DEFAULT:
        {
            cout << "Bye" << endl;
            break;
        }
        case NEW_VALUE:
        {
            cout << "New" << endl;
            break;
        }
        case NOTHING:
        {
            break;
        }
        case ERROR:
        {
            break;
        }
        case READY:
        {
            break;
        }
        case CONTINUE:
        {
            break;
        }
        case CONFIRM:
        {
            break;
        }
        case AGAIN:
        {
            break;
        }
        case TIME:
        {
            break;
        }
        default:
        {
            
        }
    }
     
    return 0;
}