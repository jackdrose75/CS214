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

int pc = 0; 

void pcounter(char* path){ // , char* colsort
    DIR *dir;
    dir = opendir(path);
    struct dirent *sd;
    
    if(dir == NULL) 
    {
        printf("Error: Directory N/A");
        return 0;
    }
    
    while (sd = readdir(dir)){
	char* subpath;
	int length = strlen(sd->d_name); 
	subpath = pathcat(path, sd->d_name);
	struct stat s;
	stat(subpath, &s);
       
        if(((sd->d_type) == DT_DIR) && (strcmp(sd->d_name, ".") !=0) && (strcmp(sd->d_name, "..") !=0)){
            pc++;
            childcounter(subpath); // , colsort
        }else if(((sd->d_type) == DT_REG) && (strncmp(sd->d_name+length-4, ".csv", 4) == 0)){
            pc++;
	}
    }
}

// allocates memory for subpath's and also appends /
char* pathcat(const char* str1,const char* str2){ 
    char* subpath;  
    result=(char*)malloc(strlen(str1)+strlen(str2)+ 3);

    if(!result){
        printf("failed to allocate memory\n");  
        exit(1);  
    }  
	strcpy(result,str1);
	strcat(result,"/");   
	strcat(result,str2);  
    return subpath;  
} 

// dirSearch takes in the given directory (dir), column being sorted (sortpath), and output file (outdir). 
void dirSearch(char *path){ // , char *colsort, char* outdir
    DIR* dir = opendir(path);
    struct dirent* sd;

    //int childnum = 0; // counts the children for output
    pid_t pid; //assign fork to this value for child process

    // null case: failed to open directory
    if(dir == NULL) 
    {
        printf("Error: Directory N/A");
        return 0;
    }

    // searches through current directory and prints files & subdirs inside
    while((sd=readdir(dir)) != NULL){
	char* subpath;
	int length = strlen(sd->d_name); 
	subpath = pathcat(path, sd->d_name);
	struct stat s;
	stat(subpath, &s);
	       
	//get sub directories excluding current (.) and prev directories (..)
        if (((sd->d_type) == DT_DIR) && (strcmp(sd->d_name, ".") != 0) && (strcmp(sd->d_name, "..") != 0)) { //dir_item->d_type ==4 subdirectories
            printf("\nDIRECTORY HERE: %s\n\n", sd->d_name); //need to fork here
            pid = fork();
		
            if (pid == -1){
                printf("fork failed\n");
                exit(1); //fork failed
            }

           if (!pid){
                // child process
                printf("CHILD PROCESS\n");
                printf("child pid : %d\n", getpid());
                dirSearch(subpath); // , colsort, outdir
		exit(0); // waits for child w/ specific pid to finish before continuing
            }else{ // if pid = 0 then child
		//childnum++;
                printf("Parent PROCESS\n");
                printf("parent ppid : %d\n", getppid());
                printf("child pid : %d\n", getpid());
            }
            printf("i'm here?\n");
            printf("path : %s\n", sd->d_name);
        }

        // if file's last 4 bytes == ".csv" then execute if statement
        if (((sd->d_type) == DT_REG) && (strncmp(sd->d_name+length-4, ".csv", 4) == 0)){
            printf("\n CSV IS FOUND: %s\n\n", sd->d_name); //fork here
            pid = fork();
           if (!pid){
                // parent process
                printf("CHILD PROCESS\n");
                printf("child pid : %d\n", getpid());
                //sort_csv(path, sd->d_name, sortpath, outdir);
		//childnum++;
		exit(0); // waits for child w/ specific pid to finish before continuing
            }else{ // if pid = 0 child
		// there is a child
                printf("PARENT PROCESS\n");
                printf("parent ppid : %d\n", getppid());
                printf("child pid : %d\n", getpid());
            }
            printf("i'm here?\n");
            printf("path : %s\n", sd->d_name);
        }
    }
    closedir(dir);
    wait(NULL);
}

//search for csv files
void fileSearch(char *path){

}

int main(int argc, char **argv)
{
/*
	//recursion forking variables
	char* path, colsort, outdir;
	
	//checks for -c flag to be inputted
	if(strcmp(argv[1], "-c")){
		printf("Missing -c first flag, please input again\n");
		exit(1);
	}
	
    //read stdin parameters
    if (argc == 3) { // <prgname> -c <input directory>
        //search current directory
        printf("Sorting by %s and storing in current directory.\n", argv[2]);
        //char *sortType = argv[1]; //get argument to sort by, -c for column
        path = argv[1]; //input directory
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
*/
    //search directories
    dirSearch(".");
    pcounter(".");
    printf("Total # processes: %d\n", pc);
    return 0;
}
