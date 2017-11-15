#include "sorter.h"

/* To test:
cd Desktop
cd <desired directory>
gcc -o <outfile> testscan.c
./<outfile>

Lists the files in current directory
and prints out if condition when .csv files are located
*/


void dirSearch(char *path){
    //find files within directory
    DIR *dir;   // directory pointer
    struct dirent *sd;
    dir = opendir(path); //points to current directory
    
    //for pid and forking
    pid_t pid; //assign fork to this value for child process

    // null case: failed to open directory
    if(dir == NULL) 
    {
        printf("Error: Directory N/A");
        return 0;
    }
    
    // searches through current directory and prints files & subdirs inside
    while((sd=readdir(dir)) != NULL){
        int length = strlen(sd->d_name);
        //get sub directories excluding current (.) and prev directories (..)
        if (((sd->d_type) == DT_DIR) && (strcmp(sd->d_name, ".") != 0) && (strcmp(sd->d_name, "..") != 0)) {
            printf("\nDIRECTORY HERE: %s\n\n", sd->d_name); //need to fork here
            
            pid = fork();
            if (pid == -1){
                printf("fork failed\n");
                exit(1); //fork failed
            }
            else if (pid == 0){
                //there is a child, get exit status
                printf("CHILD PROCESS\n");
                printf("parent ppid : %d\n", getppid());
                printf("child pid : %d\n", getpid());     
            } else {
                //parent process
                printf("PARENT PROCESS\n");
                printf("child pid : %d\n", getpid());
                
                //waits for child w/ specific pid to finish before continuing
                int status;
                waitpid(pid, &status, 0);
            }
            printf("i'm here?\n");
            printf("path : %s\n", sd->d_name);
        }

        // if file's last 4 bytes == ".csv" then execute if statement
        if (((sd->d_type) == DT_REG) && (strncmp(sd->d_name+length-4, ".csv", 4) == 0)){
            printf("\n CSV IS FOUND: %s\n\n", sd->d_name); //fork here
            

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

}

//search for csv files
void fileSearch(char *path){


}

// convert main function to void dirscanner(char *dir);
int main(int argc, char **argv)
{
    //read stdin parameters
    if (argc == 3) {
        //search current directory
        printf("Sorting by %s and storing in current directory.\n", argv[2]);
        char *sortType = argv[1]; //get argument to sort by, -c for column
        char *sortTopic = argv[2]; //get topic i.e. 'movies'
    }
    else if ((argc == 7) && (strcmp(argv[3], "-d")) == 0) {
        //sort and store in new directory
        printf("Sorting by %s and starting in %s and storing in %s\n", argv[2], argv[4], argv[6]);
        char *sortType = argv[1]; //get argument to sort by, -c for column
        char *sortTopic = argv[2]; //get topic i.e. 'movies'
        char *inputDir = argv[4]; //dir to start sort
        char *outputDir = argv[7]; //dir to store sorted files
    } else {
        //too few parameters
        printf("Invalid parameters");
        return 0;
    }

    //search directories
    dirSearch(".");

    return 0;
}


