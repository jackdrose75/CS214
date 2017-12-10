#include <arpa/inet.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#define SA struct sockaddr

//trying to implement client example
//https://forgetcode.com/c/1201-file-transfer-using-tcp

int main(int argc,char**argv)
{
    int sockfd;
    char fname[25];
    int len;

    struct sockaddr_in servaddr,cliaddr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0) {
        printf("ERROR opening socket");
        fflush(stdout);
        return -1;
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(atoi(argv[1]));

    // printf("arg[1] %s\n", argv[1]);
    int s;
    s = inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
    if (s <= 0) {
        if (s == 0) {
           printf("Not in presentation format\n");
           fflush(stdout);
           return -1;
        } else {
            printf("Other kind of error\n");
            fflush(stdout);
            return -1;
        }
    }
    int t;
    t = connect(sockfd,(SA*)&servaddr,sizeof(servaddr));
    if (t <0){
        printf("ERROR connecting\n");
        fflush(stdout);
        return -1;
    }
    printf("client connection successful\n");
    
    char buffer[100];
    FILE *f;
    f=fopen("add.txt","r");
    fscanf(f,"%s",buffer);
    write(sockfd,buffer,100);
    printf("the file was sent successfully");
}