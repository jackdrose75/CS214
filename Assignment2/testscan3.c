#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h> // need strncmp/strlen for finding ".csv" files
#include <dirent.h> // allows for readdir(), opendir()

int pc = 0;
int pc1 = 1;
int child = 1; 
pid_t pidArray[256];
pid_t p;

// allocates memory for subpath's and also appends /
char* pathcat(const char* str1,const char* str2){ 
    char* subpath;  
    subpath=(char*)malloc(strlen(str1)+strlen(str2)+ 3);

    if(!subpath){
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
    
    while (sd = readdir(dir)){
	char* subpath;
	int length = strlen(sd->d_name); 
	subpath = pathcat(path, sd->d_name);
	//struct stat s;
	//stat(subpath, &s);
       
        if(((sd->d_type) == DT_DIR) && (strcmp(sd->d_name, ".") !=0) && (strcmp(sd->d_name, "..") !=0)){
            pc1++;
            pcounter(subpath); // , colsort
        }else if(((sd->d_type) == DT_REG) && (strncmp(sd->d_name+length-4, ".csv", 4) == 0)){
            pc1++;
	}
    }
}

void addpid(pid_t p){
	pidArray[pc] = p;
	pc++;
}

void clear(){
	int i;
	for (i=0; i < sizeof(pidArray)/sizeof(pid_t); i++){
		pidArray[i] = 0;
	}
	pc = 0;
}

void openDir(char* path){
	DIR *dir;
	struct dirent *di;
	struct stat st;
	pid_t pid;
	pid_t pi = getpid();
	
	if(pi==getpid()){
		printf("PIDS of all child processes: ");
	}
	fflush(stdout); // flush buffer for stdout

	if(!(dir = opendir(path))){ // check to see if path exists
		printf("%s ----------fail\n",path);
		return;
	}
//dir = opendir(path);

// go thru directory
	while( (di =readdir(dir)) != NULL){
		//ignore parent and current directory
		if(strcmp(di->d_name,".") != 0 && strcmp(di->d_name,"..") != 0){ //x
			//printf("%s\n",di->d_name);
			// directory
			if(di->d_type==DT_DIR){//w
				pid = fork();

				if(pid==0){
				clear();
	//path2 =(char*) malloc( strlen(path) +strlen(di->d_name) + 5);
				strcat(path,"/");
				strcat(path,di->d_name);
 				//printf("%s\n",path);
				//path ="/home/user/Desktop/proj";
				dir = opendir(path);
				}

				if(pid >0){
            				addpid(pid);
				}

				if(pid<0){

				}

			}else{ //w
			//not dir
				char* tmp2= di->d_name;
				int length = strlen(tmp2);
				if(((di->d_type) == DT_REG) && (strncmp(di->d_name+length-4, ".csv", 4) == 0)){//j
				char* path2= malloc(sizeof(char)*1500);
				strcpy(path2,path);
				strcat(path2,"/");
				strcat(path2,di->d_name);
					if(1){ //check to see if it is right csv file
					//printf("ayyy\n");
					//fork here 
					pid=fork();
	
					if(pid==0){
					clear();
					//printf("ayyy\n");
					//sorter(path,path2,di->d_name,odir,input);
					free(path2);
					//printf("\n%d  %d  new\n",getpid(),getppid());
					exit(child);
					}

					if(pid>0){
		            		addpid(pid);
					continue;
					}
				}
				//printf("----%s----\n",di->d_name);
			} //j

		}//w
		}else{ //x
		//not dir
		} 
	}//end

	int i;
 	int status=0;
    	for (i = 0; i < sizeof(pidArray)/sizeof(pid_t); i++){
		if (pidArray[i] == 0){
			break;
		}
		/*if(pi != getpid()){
		printf("%d, ",pidArray[i]);
		}else{
		printf("%d",pidArray[i]);
		}; */
		printf("%d, ",pidArray[i]);
		// /sizeof(pid_t))
		waitpid(pidArray[i], &status, 0);
		if(WIFEXITED(status)>0){
			//printf("\n %d status: %d \n",pidArray[i],WEXITSTATUS(status));
			child = child + WEXITSTATUS(status);
		}
	}
	closedir(dir);
		if(pi!=getpid()){
			exit(child);
		}
		printf(" \nTotal number of processes: %d \n",child);
		wait(NULL);
		//printf("%d\n",getpid());
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
	// /ilab/users/ewj12/Desktop/pro1
	openDir(".");
	pcounter(".");
	printf("Total # processes Array: %d\n", pc);
	printf("Total # processes Recur: %d\n", pc1);
    return 0;
}

