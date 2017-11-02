#include "sorter.h"
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

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

char *getCat(char *line, int catIndex){

    char *pStart = line; //keeps track of beginning of substring
    char *pEnd = line; //continues until finds substring index

    int strIndex = 0;

    bool inQuote = false;

    while(*pEnd){
        switch(*pEnd){
            case ',':
                if(inQuote == false){
                    //printf("pStart : %s\n", pStart);
                    if(strIndex == catIndex){
                        //create and return substring
                        //char *str = buildString(pStart, pEnd);
                        //printf("%s\n", str);
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
                //printf(">>>%c >> %ld \n", *pEnd, (pEnd-pStart));
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

void sorter(char *filename, char *path, char *subpath, int argc, char **argv){

    //read in file
    FILE *input;
    input = fopen(path, "r");
    
    // printf("PATH : %s\n", path);
    // printf("SUBPATH : %s\n", subpath);

    char *sortType;
    char *sortTopic;
    char *inputDir;
    char *outputDir;

    const int MAXSIZE = 1024;

    if(strcmp(argv[1], "-c")){
        printf("Missing -c first flag, please input again\n");
        exit(1);
    }
    
    //read stdin parameters
    if (argc == 3) { // <prgname> -c <input directory>
        //search current directory
        // printf("Sorting by %s and storing in current directory.\n", argv[2]);
        //char *sortType = argv[1]; //get argument to sort by, -c for column
        // path = argv[1]; //input directory
        sortType = argv[1]; //get argument to sort by, -c for column
        sortTopic = argv[2]; //get topic i.e. 'movies'

        inputDir = "."; //dir to start sort
        outputDir = "."; //dir to store sorted files

    } else if ((strcmp(argv[3], "-d")) == 0){
        if (argc == 5 ) {
            sortType = argv[1]; //get argument to sort by, -c for column
            sortTopic = argv[2]; //get topic i.e. 'movies'
            inputDir = argv[4]; //dir to start sort
            outputDir = subpath;
        } else if ((argc == 7) && (strcmp(argv[3], "-d")) == 0) {
            //sort and store in new directory
            // printf("Sorting by %s and starting in %s and storing in %s\n", argv[2], argv[4], argv[6]);
            sortType = argv[1]; //get argument to sort by, -c for column
            sortTopic = argv[2]; //get topic i.e. 'movies'
            inputDir = argv[4]; //dir to start sort
            outputDir = argv[7]; //dir to store sorted files
        }
    } else {
        //too few parameters
        printf("Invalid parameters");
        exit(0);
    }

    //stores the input from the user as a string variable
    // char *sortType = argv[1]; //get argument to sort by, -c for column
    // char *sortTopic = argv[2]; //get topic i.e. 'movies'

    //get categories inputted by use
    //match by index number of column
    char first_row[MAXSIZE]; // = (char *)malloc(MAXSIZE);
    fgets(first_row, MAXSIZE, input); //get first row
    char *_first_row = strdup(first_row); //tmp to keep track for strsep

    char *cat; //pulled category from strsep
    int cat_index = -1; //index where category is
    int int_index = 0; //index for total size of line
    while((cat = strsep(&_first_row, ",")) != NULL){
        if(strstr(cat, sortTopic)){
            cat_index = int_index;
        }
        int_index++;
    }

    //check if valid input
    if (cat_index == -1){
        printf("%s is not a valid input.\n", sortTopic);
        exit(0);
    }

    //initialize array of structs
    Record **recordList = (Record **)malloc(sizeof(Record **));

    //get input line by line from input
    int index = 0;
    char row[MAXSIZE];
    //char *row;

    while(fgets(row, MAXSIZE, input) != NULL){

        //strtok(row, "\n");
        char *_row = strdup(row);

        //temporary struct to hold parsed data
        Record *tmpList;
        tmpList = (Record *)malloc(sizeof(Record *));



        //parse string for category field_data (i.e. director_name => James Cameron)
        char *tmp = getCat(strdup(_row), cat_index); //send _row and cat index into getCat to be parsed, extract pulled field_data
        
        //save category into tmpList.field_data
        tmpList -> field_data = tmp;
        // printf("tmpList -> field_data: %s\n", tmpList->field_data);

        //save original string row to tmpList.original_row
        tmpList -> original_row = _row;
        // printf("tmpList -> original_row : %s\n", tmpList->original_row);

        //add tmpList struct to array of structs recordList[index] at index
        recordList = (Record **)realloc(recordList, (index+1)*sizeof(Record **));
        recordList[index] = (Record *)malloc(sizeof(Record *)); 
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
    outpath = (char*)malloc(strlen(outputDir)+strlen(sortedname)+ 3);
    if(outpath == NULL){
        printf("Failed to allocate memory\n");  
        exit(1);  
    }
    strcpy(outpath,outputDir);
    strcat(outpath,"/");   
    strcat(outpath,sortedname); 


    FILE *outfp = fopen(outpath, "w");

    // printf("original file name: %s\n", filename);
    // printf("output file name: %s\n", outpath);

    int i;
    fprintf(outfp, first_row);
    for(i = 0; i < index; i++) {
        fprintf(outfp, recordList[i] -> original_row);
    }

    //printf("Here\n");
    free(recordList);
}


//>>>>>>>>>>>>TESTSCAN2.C>>>>>>>>>>>>>>>>>>
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
        printf("Error: Directory N/A");
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
void dirSearch(char *path, int argc, char **argv){ // , char *colsort, char* outdir
    DIR* dir = opendir(path);
    struct dirent* sd;
    pid_t pid; //assign fork to this value for child process
    pid_t ptemp; //for holding child pid vals to insert in pidArray
    
    // null case: failed to open directory
    if(dir == NULL) {
        printf("Error: Directory N/A");
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
                printf("fork failed\n");
                exit(1); //fork failed
            }

           if (!pid){
                // child process
                ptemp = getpid();
                addArray(ptemp);
                // printf("%d, ", ptemp);
                // dir = opendir(path);
                dirSearch(fullpath, argc, argv); // , colsort, outdir
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

            char *filename = sd->d_name;

            //sort CSV


            printf("fullpath : %s\n", fullpath);

            sorter(filename, fullpath, path, argc, argv);

            exit(0); // waits for child w/ specific pid to finish
            
            }
            else if(pid > 0) { // parent process
                printf("%d, ", pid);
                fflush(stdout);
            }
        }
    }

   // printf("PIDS of all child processes:\n\n"); 
    pid_t waitid;
    // allows for the children processes to finish running
    while ((waitid = wait(NULL)) > 0){  
        continue;
    }   
}



int main(int argc, char **argv){

    char *sortType;
    char *sortTopic;
    char *inputDir;
    char *outputDir;

    const int MAXSIZE = 1024;

    if(strcmp(argv[1], "-c")){
        printf("Missing -c first flag, please input again\n");
        exit(1);
    }
    
    //read stdin parameters
    if (argc == 3) { // <prgname> -c <input directory>
        //search current directory
        // printf("Sorting by %s and storing in current directory.\n", argv[2]);
        //char *sortType = argv[1]; //get argument to sort by, -c for column
        // path = argv[1]; //input directory
        sortType = argv[1]; //get argument to sort by, -c for column
        sortTopic = argv[2]; //get topic i.e. 'movies'

        inputDir = "."; //dir to start sort
        outputDir = "."; //dir to store sorted files

    } else if ((strcmp(argv[3], "-d")) == 0){
        if (argc == 5 ) {
            sortType = argv[1]; //get argument to sort by, -c for column
            sortTopic = argv[2]; //get topic i.e. 'movies'
            inputDir = argv[4]; //dir to start sort
            outputDir = ".";
        } else if ((argc == 7) && (strcmp(argv[3], "-d")) == 0) {
            //sort and store in new directory
            // printf("Sorting by %s and starting in %s and storing in %s\n", argv[2], argv[4], argv[6]);
            sortType = argv[1]; //get argument to sort by, -c for column
            sortTopic = argv[2]; //get topic i.e. 'movies'
            inputDir = argv[4]; //dir to start sort
            outputDir = argv[7]; //dir to store sorted files
        }
    } else {
        //too few parameters
        printf("Invalid parameters");
        exit(0);
    }

    //checks for -c flag to be inputted
    p = getpid();
    //search directories
    printf("Initial PID: %d\n", getpid());
    printf("PIDS of all child processes: ");
    fflush(stdout);

    dirSearch(inputDir, argc, argv);

    wait(NULL);
    pcounter(inputDir);
    

    printf("\nTotal # processes: %d\n", pc+1);
    // children counter + 1 original process

    return 0;

}




