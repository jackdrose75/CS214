#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>

#include "sorter.h"

#define BUF_SIZE 1024



//move above to sorter_server.h
// #include "sorter_server.h"

/*
Server
The server will open a port and wait for connection requests. 
On connection, it will spawn a service thread to handle that 
connection and go back to waiting for requests. Each service 
thread should read in a client request, if it is a sort request, 
it should perform the sort and store the results at the server. 

If it is a dump request, it should merge the current collection 
of sorted results into one sorted list and send the result back 
to the client. 

You may want/need to make use of synchronization constructs like 
mutex_locks, semaphores, and/or condition variables in your 
implementation to prevent memory corruption.
 
The server will run until stopped by a SIGKILL (i.e. kill <pid of server>).
 
To STDOUT, output a list of the ip addresses of all the clients that have connected: (?-> when?)
Received connections from: <ipaddress>,<ipaddress>,<ipaddress>,…
*/


/*
 * Server portion. 
 *
 * Creates a socket file descriptor for a client to connect its socket to.
 * Binds socket to port 12345.
 * Listens for client connections.
 * Checks for either dump or sort request.
 * Closes the port.
 */

//INPUT: ./sorter_server -p 12345
int main(int argc, char** argv) {

    //Declare variables.
    struct sockaddr_in server_address, client_address;
    int clisize; //client address size
    int port, sockfd;


    //Check input format.
    int args;
    for (args = 0; args < argc; args++){
        if (strcmp(argv[args], "-p") == 0){
            if (argv[args+1]){
                //Initialize port number into portnum
                // printf("argv[%d+1]: %s\n", args, argv[args+1]);
                port = atoi(argv[args+1]);
            } else {
                printf("No port number provided\n");
                return -1;
            }
        }
    }

    //Fill server struct
    // bzero((char *)&server_address, sizeof(struct sockaddr_in));
    memset(&server_address, '0', sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);                 

    //Creates a socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0) {
        printf("Socket creation unsuccessful.\n");
        return -1;
    }
    printf("Socket creation successful.\n");

    //Bind
    if(bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address))<0) {
        printf("Binding server to socket unsuccessful.\n");
        return -1;
    }
    printf("Binding server to socket successful.\n");
    printf("Congratulations. Server is now up and running.\n");

    //Listen
    if(listen(sockfd, 20) <0 ) {
        printf("Listening to socket unsuccessful.\n");
        return -1;
    }
    printf("Listening to socket...\n");

    while(1) {
        clisize = sizeof(client_address);
        int getfilefd = accept(sockfd, (struct sockaddr*)&client_address, &clisize);
        //Open sent file.
        FILE *fp = fopen("filename.txt", "rb");
        if(fp == NULL) {
            printf("File open unsuccessful.\n");
            return 1;
        }
        printf("File open successful.\n");
        while(1) {
            //Read file in blocks of BUF_SIZE bytes
            unsigned char buff[BUF_SIZE]={0};
            int nread = fread(buff,1,BUF_SIZE,fp);
            printf("Bytes read: %d\n", nread);
            //If read was successful, send data
            if(nread > 0) {
                printf("Sending data now.\n");
                write(getfilefd,buff,nread);
            }
            if(nread < BUF_SIZE) {
                if(feof(fp)) {
                    printf("End of file.\n");
                }
                if(ferror(fp)) {
                    printf("Reading error.\n");
                }
                break;
            }
        }
        close(getfilefd);
        sleep(1);
    }
    return 0;
}