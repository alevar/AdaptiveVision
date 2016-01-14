/*

Responsibilities of the Server:
1. PUT:
- accept all the data from the clients
- assign the data from clients into arrays based on the clients IP address
- Communicate the stored information to the application
- respond with an acknowledgement that data has been stored
2. GET:
- checks if the application has updated data
- responds with updated data to the client
- notifies the client of the abscence of updated data

*/


#include <stdio.h>
#include <iostream> // GENERAL PURPOSE LIBRARIES
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h> //inet_addr

using namespace std;

#define PORT    1234        // Port used by the server service
#define BACKLOG 1           // Number of connections to queue
#define BUFFERLENGTH 100    // 100 bytes

int main(int argc , char *argv[])
{
    char buffer[BUFFERLENGTH];
    char const *message;
    int socket_desc , new_socket , addrlen;
    struct sockaddr_in server , client;
     
    if((socket_desc = socket(AF_INET , SOCK_STREAM , 0)) < 0)
    {
        printf("Could not create socket");
        close(socket_desc);
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; // Autofill with my ip (lcalhost)
    server.sin_port = htons( PORT );

    memset(&(server.sin_zero), 0, 8); // Set zero values to the rest of the server structure
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        close(socket_desc);
        exit(-1);
    }
    puts("bind done");
     
    listen(socket_desc , BACKLOG);
     
    std::cout<<socket_desc<<endl;
    puts("Waiting for incoming connections...");
    addrlen = sizeof(struct sockaddr_in);

    while(true){
        if((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&addrlen)) < 0){


            close(new_socket);
            exit(-1);

            
        }
        else{

            switch(fork())
            {
                case -1: // Error
                    {
                        perror("fork");
                        close(socket_desc);
                        close(new_socket);
                        exit(-1);
                    }
                case 0: // Child
                    {
                        close(socket_desc);

                        puts("Connection accepted");

                        char *client_ip = inet_ntoa(client.sin_addr);
                        int client_port = ntohs(client.sin_port);

                        puts("Client IP is: ");
                        puts(client_ip);
                         
                        //Reply to the client
                        message = "Hello Client , I have received your connection. But I have to go now, bye\n";
                        write(new_socket , message , strlen(message));
                        sleep(20000);

                        exit(0);
                    }
                default:    // Parent
                    close(new_socket);
                    continue;
            }
        }
    }
     
    return 0;

}