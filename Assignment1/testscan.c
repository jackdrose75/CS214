#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h> // need strncmp/strlen for finding ".csv" files
#include <dirent.h> // allows for readdir(), opendir()

/* To test:
cd Desktop
cd <desired directory>
gcc -o <outfile> testscan.c
./<outfile>

Lists the files in current directory
and prints out if condition when .csv files are located
*/



void childProcess(void){
    printf("child process\n");
}

void parentProcess(void){
    printf("parent process\n");
}


// convert main function to void dirscanner(char *dir);
int main(int argc, char *argv[])
{
    DIR *dir;   // directory pointer
    struct dirent *sd;
    dir = opendir("."); //points to current directory
    
    //for pid and forking
    pid_t pid; //assign fork to this value for child process


    // null case: failed to open directory
    if(dir == NULL) 
    {
        printf("Error: Directory N/A");
        exit(1);
    }
    
    // searches through current directory and prints files & subdirs inside
    while( (sd=readdir(dir)) != NULL){
        int length = strlen(sd->d_name);
        printf(">> %s\n", sd->d_name);
        
        //get sub directories
        if (strcmp(sd->d_name, ".") == 0 || strcmp(sd->d_name, "..") == 0){
            printf("\nDIRECTORY HERE: %s\n\n", sd->d_name);
        }

        // if file's last 4 bytes == ".csv" then execute if statement
        if(strncmp(sd->d_name + length - 4,".csv",4) == 0){
            printf("\n CSV IS FOUND: %s\n\n", sd->d_name);
            printf("fork() here\n");
            
            /*
            pid = fork();
            if (pid == -1){
                printf("failed fork\n");
            }
            else if (pid == 0){
                childProcess();
                printf("child pid: %d\n", getpid());
            }
            else {
                parentProcess();
                printf("parent pid: %d\n", getppid());
            } 
            */
        }
    }
    closedir(dir);
    return 0;
}


