#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h> // need strncmp/strlen for finding ".csv" files
#include <dirent.h> // allows for readdir(), opendir()

int pc = 0; 
int pa = 0;
pid_t pidArray[5];
pid_t p;

void addArray(pid_t p){
	//printf("pidArray: %d %d\n", pa, p);
	pidArray[pa] = p;
	pa++;
}

void printpids(pid_t* pa){
	int i;
	for(i = 0; i < sizeof(pidArray)/sizeof(pid_t); i++){
	printf("%d, ",pidArray[i]);
	}
}

// allocates memory for subpaths and also appends /
char* pathcat(const char* str1,const char* str2){ 
    char* subpath;  
    subpath=(char*)malloc(strlen(str1)+strlen(str2)+ 3);

    if(subpath == NULL){
        printf("failed to allocate memory\n");  
        exit(1);  
    }  
	strcpy(subpath,str1);
	strcat(subpath,"/");   
	strcat(subpath,str2);  
    return subpath;  
} 

void pcounter(char* path){ // , char* colsort
    DIR *dir;
    dir = opendir(path);
    struct dirent *sd;
    
    if(dir == NULL) 
    {
        printf("Error: Directory N/A");
        exit(1);
    }
    
    while ((sd = readdir(dir)) != NULL){
	char* subpath;
	int length = strlen(sd->d_name); 
	subpath = pathcat(path, sd->d_name);
	//struct stat s;
	//stat(subpath, &s);
       
        if(((sd->d_type) == DT_DIR) && (strcmp(sd->d_name, ".") !=0) && (strcmp(sd->d_name, "..") !=0)){
            pc++;
            pcounter(subpath); // , colsort
        }else if(((sd->d_type) == DT_REG) && (strncmp(sd->d_name+length-4, ".csv", 4) == 0)){
            pc++;
	}
    }
}

// dirSearch takes in the given directory (dir), column being sorted (sortpath), and output file (outdir). 
void dirSearch(char *path){ // , char *colsort, char* outdir
    DIR* dir = opendir(path);
    struct dirent* sd;
    pid_t pid; //assign fork to this value for child process
    pid_t ptemp; //for holding child pid vals to insert in pidArray
	
    // null case: failed to open directory
    if(dir == NULL) 
    {
        printf("Error: Directory N/A");
        exit(1);
    }
	fflush(stdout);

    // searches through current directory and prints files & subdirs inside
    while((sd=readdir(dir)) != NULL){
	char* subpath;
	int length = strlen(sd->d_name); 
	subpath = pathcat(path, sd->d_name);
	       
	//get sub directories excluding current (.) and prev directories (..)
        if (((sd->d_type) == DT_DIR) && (strcmp(sd->d_name, ".") != 0) && (strcmp(sd->d_name, "..") != 0)){
            //printf("DIRECTORY HERE: %s\n\n", sd->d_name); //need to fork here
            pid = fork();
            if (pid == -1){
                printf("fork failed\n");
                exit(1); //fork failed
            }

           if (!pid){
                // child process
		ptemp = getpid();
		addArray(ptemp);
		// printf("%d, ", ptemp);
		// dir = opendir(path);
                dirSearch(subpath); // , colsort, outdir
		exit(0); // waits for child w/ specific pid to finish before continuing
            }else{
		printf("%d, ", pid);
		fflush(stdout); 
            }
        }

        // if file's last 4 bytes == ".csv" then execute if statement
        if (((sd->d_type) == DT_REG) && (strncmp(sd->d_name+length-4, ".csv", 4) == 0)){
            //printf("CSV IS FOUND: %s\n\n", sd->d_name); //fork here
            pid = fork();

           if (!pid){
                // child process
		ptemp = getpid();
		addArray(ptemp);
		//printf("ACTUAL CHILD %d, ", ptemp);
                //sort_csv(path, sd->d_name, sortpath, outdir);
		exit(0); // waits for child w/ specific pid to finish
            }else if(pid > 0){ // parent process
		printf("%d, ", pid);
		fflush(stdout); 
            }
        }
    }

   // printf("PIDS of all child processes:\n\n"); 
    pid_t waitid;
	//printf("wait here:::");
    while ((waitid = wait(NULL)) > 0){ 
	
	//printf("---");      
        // exit when -1 all children done;
    }	

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
	p = getpid();

	//search directories
	//printf("\nPIDS of all child processes: ");

	printf("Initial PID: %d\n", getpid());
   	printf("PIDS of all child processes: ");
   	fflush(stdout);
	dirSearch(".");
	wait(NULL);
	pcounter(".");
	
	int i;
	for (i = 0; i < sizeof(pidArray)/sizeof(pid_t); i++){
		//printf("%d, ",pidArray[i]);
		/*	if (pidArray[i] == 0){
			break;
		}*/
	}

	printf("\nTotal # processes: %d\n", pc+1);
	// children counter + 1 original process

    
    

	return 0;
}
