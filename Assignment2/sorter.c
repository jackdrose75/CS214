#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "sorter.h"
#include "mergesort.c"

// Main file to parse and pass data

//Returns category substring
//parses through input string by looking for delimiters
//pStart marks beginning of category word, pEnd marks end by delimiter
//finds out length of substring
//finds length of substring by substracting pEnd and pStart
//malloc space for new char array and copy characters to it 

//global variables for pid counter
int pc = 0; 
int pa = 0;
pid_t pidArray[5];
pid_t p;

const int MAXSIZE = 1024;

char *getCat(char *line, int catIndex){

    char *pStart = line; //keeps track of beginning of substring
    char *pEnd = line; //continues until finds substring index

    int strIndex = 0;

    bool inQuote = false;

    while(*pEnd){
        switch(*pEnd){
            case ',':
                if(inQuote == false){
                    if(strIndex == catIndex){
                        //create and return substring
                        return buildString(pStart, pEnd);
                    }
                    //if strIndex != catindex, move pStart to pEnd
                    pEnd++;
                    pStart = pEnd;
                    strIndex++; //update strIndex
                } else {
                    pEnd++;
                }
                break;

            case '\"':
                if (!inQuote){
                    inQuote = !inQuote;
                    pEnd++;
                    pStart = pEnd;
                } else {
                    inQuote = !inQuote;
                    if (strIndex++ == catIndex){
                        pEnd--;
                        return buildString(pStart, pEnd);
                    }
                    pEnd = pEnd+2;
                    pStart = pEnd;
                }
            case '\r':
                if(*(pEnd++)){
                    if(*pEnd == '\n'){
                        pEnd--;
                        return buildString(pStart, pEnd);
                    } else {
                        pEnd++;
                        break;
                    }
                }
                pEnd--;
                return buildString(pStart, pEnd);
                
            default:
                pEnd++;
        }
    }
    return '\0';
}

char *buildString(char *start, char *end){

    int strLen = end - start;
    char *strBuffer = (char *)malloc((strLen+1)*sizeof(char *));
    //copies chars from one string to another for strLen
    memcpy(strBuffer, start, strLen);
    strBuffer[strLen] = '\0';
    return strBuffer;
}

void sorter(char *filename, char *path, char *subpath, InputParams* inputDetails){

    //read in file
    FILE *input;
    input = fopen(path, "r");

    //get categories inputted by use
    //match by index number of column
    char first_row[MAXSIZE];
    fgets(first_row, MAXSIZE, input); //get first row
    char *_first_row = strdup(first_row); //tmp to keep track for strsep

    char *cat; //pulled category from strsep
    int cat_index = -1; //index where category is
    int int_index = 0; //index for total size of line
    while((cat = strsep(&_first_row, ",")) != NULL){
        if(strstr(cat, inputDetails->sortTopic)){
            cat_index = int_index;
        }
        int_index++;
    }

    //check if valid input
    if (cat_index == -1){
        printf("%s is not a valid input.\n", inputDetails -> sortTopic);
        exit(0);
    }

    //initialize array of structs
    Record **recordList = (Record **)malloc(sizeof(Record *));

    //get input line by line from input
    int index = 0;
    char row[MAXSIZE];

    while(fgets(row, MAXSIZE, input) != NULL){

        //strtok(row, "\n");
        char *_row = strdup(row);

        //temporary struct to hold parsed data
        Record *tmpList;
        tmpList = (Record *)malloc(sizeof(Record));

        //parse string for category field_data (i.e. director_name => James Cameron)
        char *tmp = getCat(strdup(_row), cat_index); //send _row and cat index into getCat to be parsed, extract pulled field_data
        
        //save category into tmpList.field_data
        tmpList -> field_data = tmp;
        //save original string row to tmpList.original_row
        tmpList -> original_row = _row;

        //add tmpList struct to array of structs recordList[index] at index
        recordList = (Record **)realloc(recordList, (index+1)*sizeof(Record *));
        recordList[index] = (Record *)malloc(sizeof(Record)); 
        recordList[index] = tmpList;

        index++;
    }

    //merge sort recordLists by field_data
    merge_sort(recordList, 0, index-1);

    //output
    char* sortedname;  
    sortedname=(char*)malloc(strlen("-sorted-")+strlen(filename)+ 3);
    if(sortedname == NULL){
        printf("Failed to allocate memory\n");  
        exit(1);  
    }  
    strcpy(sortedname,"-sorted-");
    strcat(sortedname,filename); 

    char *outpath;
    outpath = (char*)malloc(strlen(inputDetails -> outputDir)+strlen(sortedname)+ 3);
    if(outpath == NULL){
        printf("Failed to allocate memory\n");  
        exit(1);  
    }
    strcpy(outpath,inputDetails -> outputDir);
    strcat(outpath,"/");   
    strcat(outpath,sortedname); 


    FILE *outfp = fopen(outpath, "w");

    int i;
    fprintf(outfp, first_row);
    for(i = 0; i < index; i++) {
        fprintf(outfp, recordList[i] -> original_row);
    }
    free(recordList);
}

void addArray(pid_t p){
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
char* pathcat(const char* path, const char* filename){ 
    char* subpath;  
    subpath=(char*)malloc(strlen(path)+strlen(filename)+ 3);

    if(subpath == NULL){
        printf("Failed to allocate memory\n");  
        exit(1);  
    }

    strcpy(subpath,path);
    strcat(subpath,"/");   
    strcat(subpath,filename);  
    return subpath;  
} 

void pcounter(char* path){ // , char* colsort
    DIR *dir;
    dir = opendir(path);
    struct dirent *sd;
    
    if(dir == NULL) {
        printf("Error: Directory N/A\n");
        exit(1);
    }
    
    while ((sd = readdir(dir)) != NULL){
        char* subpath;
        int length = strlen(sd->d_name); 
        subpath = pathcat(path, sd->d_name);
        //struct stat s;
        //stat(subpath, &s);
        
        //if directory
        if(((sd->d_type) == DT_DIR) && (strcmp(sd->d_name, ".") !=0) && (strcmp(sd->d_name, "..") !=0)){
            pc++;
            pcounter(subpath); // , colsort
        }
        //if csv file
        else if(((sd->d_type) == DT_REG) && (strncmp(sd->d_name+length-4, ".csv", 4) == 0)){
            pc++;
        }
    }
}

// dirSearch takes in the given directory (dir), column being sorted (sortpath), and output file (outdir). 
void dirSearch(char *path, InputParams* inputDetails){
    DIR* dir = opendir(path);
    struct dirent* sd;
    pid_t pid; //assign fork to this value for child process
    pid_t ptemp; //for holding child pid vals to insert in pidArray


    // null case: failed to open directory
    if(dir == NULL) {
        printf("Error: Directory N/A\n");
        exit(1);
    }
    fflush(stdout);

    // searches through current directory and prints files & subdirs inside
    while((sd=readdir(dir)) != NULL){
        char* fullpath;
        int length = strlen(sd->d_name); 
        fullpath = pathcat(path, sd->d_name);
        

        //get sub directories excluding current (.) and prev directories (..)
        if (((sd->d_type) == DT_DIR) && (strcmp(sd->d_name, ".") != 0) && (strcmp(sd->d_name, "..") != 0)){
            //printf("DIRECTORY HERE: %s\n\n", sd->d_name); //need to fork here
            pid = fork();
            if (pid == -1){
                printf("Fork failed\n");
                exit(1); //fork failed
            }

           if (!pid){
                // child process
                ptemp = getpid();
                addArray(ptemp);                
                dirSearch(fullpath, inputDetails); // , colsort, outdir
                exit(0); // waits for child w/ specific pid to finish before continuing
            
                }else{
            
                printf("%d, ", pid);
                fflush(stdout); 
            }
        }

        // if file's last 4 bytes == ".csv" then execute if statement
        if (((sd->d_type) == DT_REG) && (strncmp(sd->d_name+length-4, ".csv", 4) == 0)){
            pid = fork();

           if (!pid){
                // child process
            ptemp = getpid();
            addArray(ptemp);

            char *filename = sd->d_name;

            //sort CSV
            sorter(filename, fullpath, path, inputDetails);
            exit(0); // waits for child w/ specific pid to finish
            } else if(pid > 0) { // parent process
                printf("%d, ", pid);
                fflush(stdout);
            }
        }
    }

    pid_t waitid;
    // allows for the children processes to finish running
    while ((waitid = wait(NULL)) > 0){  
        continue;
    }   
}


int main(int argc, char **argv){

    int c_flag = 0; //category flag
    int d_flag = 0; //input directory flag
    int o_flag = 0; //output directory flag


    InputParams *inputDetails;
    inputDetails = (InputParams*)malloc(sizeof(InputParams*));

    int args; //placeholder for arg index
    for(args = 0; args < argc; args++){
        //find place of category flag
        if (strcmp(argv[args], "-c") == 0){
            if (argv[args+1]){
                inputDetails -> sortTopic = argv[args+1];
                c_flag = 1;
            } else {
                printf("Invalid\n");
                exit(0);
            }
        }

        //get input directory
        if (strcmp(argv[args], "-d") == 0){
            if (argv[args+1]){
                inputDetails -> inputDir = argv[args+1];
                d_flag = 1;
            } else {
                printf("Invalid\n");
                exit(0);
            }
        }

        //get output directory
        if (strcmp(argv[args], "-o") == 0){
            if (argv[args+1]){
                inputDetails -> outputDir = argv[args+1];
                o_flag = 1;
            } else {
                printf("Invalid\n");
                exit(0);
            }
        }
    }

    if (c_flag == 0) {
        printf("No sort topic\n");
        exit(0);
    }
    if (d_flag == 0){
        inputDetails -> inputDir = ".";
    }
    if (o_flag == 0){
        inputDetails -> outputDir = ".";
    }

    //checks for -c flag to be inputted
    p = getpid();

    //search directories
    printf("Initial PID: %d\n", getpid());
    printf("PIDS of all child processes: ");
    fflush(stdout);

    dirSearch(inputDetails->inputDir, inputDetails);

    wait(NULL);

    pcounter(inputDetails->inputDir);
    

    printf("\nTotal # processes: %d\n", pc+1);
    // children counter + 1 original process

    free(inputDetails);
    return 0;

}




