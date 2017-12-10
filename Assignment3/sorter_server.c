#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include <fcntl.h>

#define INET_ADDRSTLEN 16
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

void get_file_name(int socket, char* filename) {
    char recv_str[BUF_SIZE];
    ssize_t rcvd_bytes;

    //Read file name from socket.
    if((rcvd_bytes = recv(socket, recv_str, BUF_SIZE, 0))<0) {
        printf("rcvd unsuccessful.\n");
        return;
    }
    printf("rcvd successful.\n");
    sscanf(recv_str, "%s\n", filename); //Discard CR/LF
}

int send_file(int socket, char* filename) {
    int sent_count,f;           //How many sending chunks; file handle
    ssize_t read_bytes,     //Bytes read from local file
            sent_bytes,     //Bytes sent to socket
            sent_file_size;
    char send_buf[BUF_SIZE];//Max chunk size for file sending

    //Initialize ints to 0.
    sent_count = 0;
    sent_file_size = 0;

    //Open file for reading
    if((f = open(filename, O_RDONLY))<0) {
        printf("File read unsuccessful.\n");
        return -1;
    } else {
        printf("Sending file: %s\n", filename);
        while((read_bytes = read(f, send_buf, BUF_SIZE))>0) {
            if((sent_bytes = send(socket, send_buf, read_bytes, 0)) < read_bytes) {
                printf("File sending unsuccessful.\n");
                return -1;
            }
            sent_count++;
            sent_file_size += sent_bytes;
        }
        close(f);
    }
    printf("Client task completed. Sent %d bytes in %d send(s)\n\n", sent_file_size, sent_count);
    return sent_count;
}

int main(int argc, char** argv) {

    //Check input format.
    int args, p_flag, port;
    p_flag = 0; //flag for if port given
    for (args = 0; args < argc; args++){
        if (strcmp(argv[args], "-p") == 0){
            if (argv[args+1]){
                //Initialize port number into portnum
                port = atoi(argv[args+1]);
                p_flag = 1;
            } else {
                printf("No port number provided\n");
                return -1;
            }
        }
    }
    if (p_flag == 0){
        printf("No port provided\n");
        return -1;
    }

    // if (!((argc == 3) && (strcmp(argv[1],"-p")==0) && isdigit(argv[2]))) {
    //     printf("Incorrect input.\n");
    //     return 1;
    // }   

    //Declare variables.
    struct sockaddr_in server_address, client_address;
    int clisize;    //client address size
    // int port, sockfd, connfd;
    int sockfd, connfd;

    char file_name [BUF_SIZE];          //File name to be sent
    char print_addr [INET_ADDRSTRLEN];  //Readable IP address

    //Initialize port number into port
    // port = atoi(argv[2]);

    //Fill server struct
    memset(&server_address, '0', sizeof(server_address));
    memset(&client_address, '0', sizeof(client_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);  //Port number is supplied in port
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
    if(listen(sockfd, 20) < 0) {
        printf("Listening to socket unsuccessful.\n");
        return -1;
    }

    while(1) {
        clisize = sizeof(client_address);
        printf("Waiting for client connection...\n\n");
        if((connfd = accept(sockfd, (struct sockaddr*)&client_address, &clisize))<0) {
            printf("Client accept unsuccessful.\n");
            return -1;
        }
        printf("Client accept successful.\n");
        
        //Convert numeric IP to readable format for displaying
        inet_ntop(AF_INET, &(client_address.sin_addr), print_addr, INET_ADDRSTLEN);
        printf("Client connection successful.\nIP address: %s\nPort: &d\n", print_addr, ntohs(client_address.sin_port));

        get_file_name(connfd, file_name);
        send_file(connfd, file_name);

        printf("Closing connection...\n");
        close(connfd);
    }
    close(sockfd);
    printf("Connection closed.\n");
    return 0;
}