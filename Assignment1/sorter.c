#include "sorter.h"

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

void sorter(FILE *input, int argc, char **argv){

    //read in file
    input = fopen(argv[1], "r");
    if (NULL == input) {
        fprintf(stderr, "Unable to open file\n");
        exit(EXIT_FAILURE);
    } else {
        input = stdin;
    }


    const int MAXSIZE = 1024;

    //stores the input from the user as a string variable
    char *sortType = argv[1]; //get argument to sort by, -c for column
    char *sortTopic = argv[2]; //get topic i.e. 'movies'

    //get categories inputted by use
    //match by index number of column
    char first_row[MAXSIZE];// = (char *)malloc(MAXSIZE);
    fgets(first_row, MAXSIZE, stdin); //get first row
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

    //get input line by line from stdin
    int index = 0;
    char row[MAXSIZE];
    //char *row;

    while(fgets(row, MAXSIZE, stdin) != NULL){

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
    int i;
    printf("%s", first_row);
    for(i = 0; i < index; i++) {
        printf("%s", recordList[i] -> original_row);
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
            
            //
            FILE *input;
            sorter(input, argc, argv);
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
            
            }
            else if(pid > 0) { // parent process
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





int main(int argc, char **argv){

    //
    // PID forking
    //
    p = getpid();
    //search directories
    printf("Initial PID: %d\n", getpid());
    printf("PIDS of all child processes: ");
    fflush(stdout);
    dirSearch(".", argc, argv);
    wait(NULL);
    pcounter(".");
    
    int i;
    for (i = 0; i < sizeof(pidArray)/sizeof(pid_t); i++){
        //printf("%d, ",pidArray[i]);
        /*  if (pidArray[i] == 0){
            break;
        }*/
    }
    printf("\nTotal # processes: %d\n", pc+1);
    // children counter + 1 original process

    



    return 0;

}




