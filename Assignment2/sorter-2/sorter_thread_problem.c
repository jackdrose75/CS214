#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "sorter.h"
#include "mergesort.c"
#include "linkedlist_problem.c"
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <string.h>


int globalCnt =1; //global counter
pthread_mutex_t counter_lock;
pthread_mutex_t malloc_lock;
pthread_mutex_t path_lock;
pthread_mutex_t print_lock;
//input processing
const int MAXSIZE = 1024;
int col = 0;

Node * pathHead = NULL; 
//to recognize initial TID
pthread_t parent; 

//centralArr
Record ** centralArr;
//know when to realloc
int arrCount = 0;
pthread_mutex_t stack_lock = PTHREAD_MUTEX_INITIALIZER;
int arrLim = 999; 
//int arrLim = 5;
char* firstRow = NULL; 


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
             case '\n':
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

void *sorter(void *sortParams){
    /*if (globalCnt != 1){
        printf("%u,",pthread_self()); 
        fflush(stdout); 
    } */
    SortParams *sortDetails = (SortParams *) sortParams; //cast void pointer from threading to SortParams
    /*if (sortDetails->filename==NULL){
        printf("Null\n"); 
        fflush(stdout); 
    }
    printf("sortDetails->filename:%s\n", sortDetails->filename); 
    fflush(stdout); 
    printSorter(sortDetails); */
    
    
    
    char *filename = sortDetails->filename;
    char *path = sortDetails->path;
    InputParams *inputDetails = sortDetails->inputDetails;
    
    //printf("This is the path in func sorter:%s\n", path);
    //read in file
    FILE *input;
    input = fopen(path, "r");
    if (input == NULL){
        printf("\n"); 
        perror("file"); 
        fflush(stdout); 
       	exit(0);  
        
    }
    //get categories inputted by use
    //match by index number of column
    char first_row[MAXSIZE];
    fgets(first_row, MAXSIZE, input); //get first row
    if (firstRow == NULL){
    	firstRow = strdup(first_row); 
    }
    char *_first_row = strdup(first_row); //tmp to keep track for strsep
    char *cpy = _first_row;
    char *cat; //pulled category from strsep
    int cat_index = -1; //index where category is
    int int_index = 0; //index for total size of line
    //printf("inputDetails:%s\n", inputDetails->sortTopic); 
    //fflush(stdout); 
    while((cat = strsep(&_first_row, ",")) != NULL){
    	//printf("cat:%s\n",cat); 
    	//fflush(stdout); 
        if(strcmp(cat,inputDetails->sortTopic)==0){//strstr(cat, inputDetails->sortTopic)
            cat_index = int_index;
            break;
        }
        int_index++;
    }
    free(cpy); 
    cat_index = col; 
    //check if valid input
    /*if (cat_index == -1){
        printf("%s is not a valid input.\n", inputDetails -> sortTopic);
        exit(0);
    }*/
    
    //initialize array of structs
    Record **recordList = (Record **)malloc(sizeof(Record *));

    //get input line by line from input
    int index = 0;
    char row[MAXSIZE];
    int c; 
    
    while(fgets(row, MAXSIZE, input) !=NULL && strcmp(row,"\n") != 0 ){// && *row!=' '&& strcmp(row,"\n") != 0
    // printf("Getting each row\n"); 
    // fflush(stdout); 
        //strtok(row, "\n");
        //printf("row:%s\n",row); 
        //fflush(stdout); 
        /*if (row[0]=='\n'){
        	printf("newline\n"); 
        	fflush(stdout); 
        	break;
        }*/
        //perror("file"); 
	//fflush(stdout); 
	
        //char *_row = strdup(row);
        
       
	c = fgetc(input);
	 
	if (feof(input) != 0){
		//printf("eof\n"); 
		//fflush(stdout); 
		break;
	}
	fseek(input,-1,SEEK_CUR); 
	char *_row = strdup(row);
	 
        //temporary struct to hold parsed data
        Record *tmpList;
        tmpList = (Record *)malloc(sizeof(Record));

        //parse string for category field_data (i.e. director_name => James Cameron)
        char *tmp = getCat(_row, cat_index); //send _row and cat index into getCat to be parsed, extract pulled field_data
        
        //save category into tmpList.field_data
        tmpList -> field_data = tmp;
        //save original string row to tmpList.original_row
        tmpList -> original_row = _row;

        //add tmpList struct to array of structs recordList[index] at index
        
        recordList = (Record **)realloc(recordList, (index+1)*sizeof(Record *));
        //recordList[index] = (Record *)malloc(sizeof(Record)); 
        recordList[index] = tmpList;
	
        index++;
    }
    //free(recordList[index-1]); 
    /*printf("Index:%d\n", index); 
    fflush(stdout); */
    /*int s = 0; 
    while(s<index){
    
        printf("Record field [%d]:%s\n",s,recordList[s]->field_data); 
        s++; 
    }*/
    //merge sort recordLists by field_data
    //printf("Before merge sort\n"); 
    int close = fclose(input); 
   
    if (close==EOF){
	printf("Failed to close file\n"); 
	fflush(stdout);  
	exit(0);    	
    }
    merge_sort(recordList, 0, index-1);
    // merge_sort(recordList, 0, index-1);
    //Nothing printing past this
    
    //printf("This is after merge sort\n");
    //fflush(stdout);  
    //output
    //push(recordList,index-2); 
    push(recordList,index-1); 
    //**Deleted for Sorter-2**//
    /*char* sortedname;  
    int length = strlen("-sorted-")+strlen(filename)+ 3; 
    sortedname=(char*)malloc(length*sizeof(char));
    
    if(sortedname == NULL){
        printf("Failed to allocate memory\n");  
        exit(1);  
    }  
    printf("Filename:%s\n", filename); 
    fflush(stdout); 
    
    strcpy(sortedname,"-sorted-");
    strcat(sortedname,filename); 
    printf("Sorted name is: %s\n", sortedname);
    fflush(stdout); 
    */
    
    
    
    /*strncpy(sortedname,filename,strlen(filename)-4); 
    strcat(sortedname,"-sorted-");
    strcat(sortedname,inputDetails->sortTopic);
    strcat(sortedname,".csv");
    strcat(sortedname,"\0"); */
    
    //**Remove for Sorter-2**//
    /*
    //pthread_mutex_lock(&path_lock);
    char *outpath;
    outpath = (char*)malloc(strlen(inputDetails -> outputDir)+strlen(sortedname)+ 3);
    if(outpath == NULL){
        printf("Failed to allocate memory\n");  
        exit(1);  
    }
    strcpy(outpath,inputDetails -> outputDir);
    strcat(outpath,"/");   
    strcat(outpath,sortedname); 
    //pthread_mutex_unlock(&path_lock);


    
    FILE *outfp = fopen(outpath, "w");

    int i;
    fprintf(outfp, first_row);
    for(i = 0; i < index; i++) {
        fprintf(outfp, recordList[i] -> original_row);
    }
    
    
    //**Need to free entire array
    int arrCounter = 0; 
    while(arrCounter<index){
        free(recordList[arrCounter]->field_data); 
        free(recordList[arrCounter]->original_row); 
        free(recordList[arrCounter]);
        
        arrCounter++; 
    }
    
    free(recordList);
    
    free(outpath);
    free(sortedname);  */
    //free(path); 
    //free(inputDetails->currPath); 
    //free(inputDetails->sortTopic); 
    
    
    
    //free(sortParams); 
    //return;
    //**Remove for Sorter-2**//
    //pthread_exit(NULL);
    
    
    
    
}

//remember to free recordList
void push(Record ** csvData,int index){
	pthread_mutex_lock(&stack_lock);
	int count = 0;  
	while (count <= index){
		if (arrCount==0){
			centralArr = (Record**)malloc(1000*sizeof(Record*)); 
			int i = 0; 
			/*for (;i<6;i++){
				centralArr[i] = (Record*)malloc(sizeof(Record)); 
			}*/
			
		}
		if (arrLim<arrCount){
			centralArr = realloc(centralArr,2*(arrLim+1)*sizeof(Record *)); 
			
			if(centralArr == NULL){
				printf("Realloc failed\n"); 
				fflush(stdout); 
			}
			
			
			//printf("arrlim:%d\n", arrLim); 
			//fflush(stdout); 
			arrLim = (2 * (arrLim+1)) -1; 
			//printf("Arrlim: %d\n", arrLim); 
			//fflush(stdout); 
			/*int j = 0; 
			for (;j<=arrLim;j++){
				printf("%d\n",j); 
				fflush(stdout); 
				centralArr[j] = (Record*)malloc(sizeof(Record)); 
			}*/
		}
		centralArr[arrCount++] = csvData[count]; 
		count++; 
	}
	/*int freeIndex = 0; 
    	while(freeIndex<=index){
        	free(csvData[freeIndex]->field_data); 
        	free(csvData[freeIndex]->original_row); 
        	free(csvData[freeIndex]);
        	freeIndex++; 
    	}
    	free(csvData);*/
    	/*int check = 0; 
    	for(; check<arrCount;check++){
    	
    		printf("Record field [%d]:%s\n",check,centralArr[check]->field_data); 
    		fflush(stdout); 
       
    	}*/
    	free(csvData); 
	//merge_sort(centralArr,0,arrCount-1); 
	pthread_mutex_unlock(&stack_lock); 
}

//traverseDir
int checkcsv (char* name){ //returns 0 if not valid, 1 if valid csv
    //printf("file name is: %s\n", name);
    fflush(stdout);
    int csv = 1;
    int counter = 0; 
    //char *newname = (char*)malloc(sizeof(char)*strlen(name)+1); 
    
    //newname[strlen(name)] = '\0'; 
    //strcpy(newname,name);
    
    char * token; 
    //token = strstr(newname,"-sorted-"); 
    token = strstr(name,"-sorted-"); 
    if (token!=NULL){
        return 0;
    } 
    
    int index = strlen(name) -1;
    if (index <= 3){ //assuming that .csv is not a valid file name
        csv = 0; 
        //printf("This is a shorter string than .csv"); 
        return csv; 
    }
    
    char * ext = ".csv";  
    int ext_index = strlen(ext) - 1;
    for (;index>=strlen(name) - 4; index--){
        if (name[index] != ext[ext_index]){
            csv = 0; 
            return csv;
        }
        ext_index = ext_index -1;
    
    }
    //printf("The return value is: %d\n", csv);
    //free(newname);
   
    return csv;
}

char* createPath (char* orig, char* new){
    char * newPath = (char*)malloc(3 + strlen(orig) + strlen(new)); 
    newPath[0] = '\0'; 
    strcpy (newPath,orig); 
    strcat (newPath,"/"); 
    strcat (newPath,new); 
    strcat (newPath, "\0"); 
    return newPath; 
}

//created to debug traverseDir
void * fake_sorter(void* sortDetails){

    SortParams *sortParams = (SortParams*) sortDetails;
    if (globalCnt != 1){
        printf("%u,",pthread_self()); 
        fflush(stdout); 
    } 
    printSorter(sortParams); 
    fflush(stdout);
    //**uncomment**//
    /*printf("**SortParams print**\n"); 
    fflush(stdout); 
    printSorter(sortParams); 
    printf("**This is fake_sorter**\n");
    fflush(stdout); */

}

//temp func to print parameters of input
void printParam(InputParams *params){
    
    printf("tid:%u\n",pthread_self()); 
    printf("Sort topic:%s\n", params->sortTopic); 
    printf("Curr path:%s\n", params->currPath); 
    printf("Output dir:%s\n", params->outputDir);
    fflush(stdout);  

}

void printSorter(SortParams * params){

    printf("tid:%u\n",pthread_self()); 
    //printf("before printing filename\n"); 
    //fflush(stdout); 
    printf("filename:%s\n",params->filename); 
    printf("path:%s\n", params->path); 
    printParam(params->inputDetails); 
    fflush(stdout); 

}

//traverses directories
void *traverseDir(void* inDetails){//curr path
    //int index = 0;
    int localCnt = 0; 
    Node *head = NULL;
    DIR *root = NULL; // curr directory
    struct dirent *currItem = NULL; //curr item that could be directory or file
    
    
    //**uncomment**//
    /*printf("Beginning\n"); 
    fflush(stdout); 
    printf("CURRENT TID:%u\n",pthread_self()); 
    fflush(stdout); */
    
    //re-cast struct
    InputParams *params = (InputParams*)inDetails; //user input parameters passed in
    //pthread_mutex_lock(&malloc_lock);     
    //InputParams * nextIter = (InputParams*)malloc(sizeof(InputParams)); 
    //pthread_mutex_unlock(&malloc_lock); 
    //**uncomment**//
    /*printf("Passed in params:\n"); 
    fflush(stdout); 
    printParam(params);*/
    
    
    root = opendir(params->currPath); //set root path to user input path 
    if (root == NULL){
        printf("Can't open the directory\n");
        fflush(stdout);
        exit(0);
    }
   
        
    
  
    //char *curr = params->currPath; //temp set curr to root initially
    char *curr = NULL; 
    char *csvpath = NULL; 
    //free(curr);
    
    /*if (globalCnt != 1){
        printf("%u,",pthread_self()); 
        fflush(stdout); 
        
    }*/
    //traverse directories and subdirectories
    while (currItem = readdir(root)){
    
    //**uncomment**//
    /*printf("currItem is:%s\n", currItem->d_name); 
    fflush(stdout);*/ 

        if (!strcmp(".", currItem->d_name) || !strcmp("..", currItem->d_name) || currItem->d_name[0] == '.'){
            continue;
        }
        
        
        //create new inputParams struct to hold updated parameters to be passed
        

        //if encountered is a subdirectory
        if(currItem->d_type == DT_DIR){
        
            char *dirname = currItem->d_name;
            
            //**uncomment**//
            /*printf("Current directory:%s\n", currItem->d_name); 
            fflush(stdout); */
            
            
            
            //curr = createPath(params->currPath, dirname);
       
        //InputParams send;
            //InputParams * nextIter = &send;
            //nextIter->currPath = curr;
            //nextIter->currPath = createPath(params->currPath, dirname);
            
            
            //**Try to malloc for InputParams**//
            
            InputParams * nextIter = (InputParams*)malloc(sizeof(InputParams)); 
            
            curr = createPath(params->currPath, dirname);
            //nextIter->currPath = createPath(params->currPath, dirname);
            nextIter->currPath = curr;
            //curr = nextIter->currPath; 
            
            /*pthread_mutex_lock(&path_lock);
            if (pathHead == NULL){
                pathHead = init(NULL,NULL, nextIter->currPath,0, 0,NULL); 
            } else{
                Node *adder = init(NULL,NULL, nextIter->currPath,0, 0,NULL);  
                pathHead = append(adder, pathHead); 
            }
            pthread_mutex_unlock(&path_lock);*/
            
            //free(params->currPath); 
            
            //check input params
            //printParam(params);
            
            
            //put lock around this
            pthread_mutex_lock(&counter_lock); 
            
            globalCnt = globalCnt +1;
            pthread_mutex_unlock(&counter_lock);
            localCnt = localCnt + 1; 
            //free curr after doing memcpy for sorter 
        
            //new thread to traverse directory
            
            
            //create local linked list to track children tids
            
            //create directory Node to hold child tid
            
            if (head == NULL){
                head = init(NULL,NULL, nextIter->currPath,pthread_self(), 0,nextIter,NULL); 
            } else{
                Node *adder = init(NULL,NULL, nextIter->currPath, pthread_self(), 0,nextIter,NULL); 
                head = append(adder, head); 
            }
                        
          
            Node *currNode = head;
        
        //get to the newly added node 
        while (currNode->next!=NULL){
            currNode = currNode->next;
        }
        //see current state of ll
       
        //**uncomment**//
        /*printf("\nNODE\n");
        fflush(stdout); 
            print(head);
            printf("\n");*/
        
            

            //spawn thread to traverseDir
            
            //**METHOD WITHOUT ADDITIONAL NODE**//
            //int ret = pthread_create (&currNode->c_tid, NULL, &traverseDir, (void*)params);
            
            //**METHOD WITH ADDITIONAL NODE **//
            //InputParams send;
            //InputParams * nextIter = &send;
            
            //without strdup
            nextIter->sortTopic = params->sortTopic;
            //nextIter->currPath = params->currPath; 
            nextIter->outputDir = params->outputDir;
            //with strdup
            /*char * iterTopic = strdup (params->sortTopic); 
            nextIter->sortTopic = iterTopic;
            char * iterPath = strdup (params->currPath);
            nextIter->currPath = iterPath;  
            char * iterOut = strdup (params->outputDir);
            nextIter->outputDir = iterOut;  */
             
             
            //**uncomment**//
            /*printf("**InputParam**\n"); 
            fflush(stdout); 
            printParam(nextIter);*/ 
            
            int ret = pthread_create (&currNode->c_tid, NULL, &traverseDir, (void*)nextIter);
             
            if (ret != 0){
                printf("Error creating thread\n"); 
                fflush(stdout);
                exit(1);
            }

                     
        }
        
        //if encountered is a file
        if(currItem->d_type == DT_REG){
             
            //check if valid csv 
            int valid = checkcsv(currItem->d_name); 
            if (valid == 0 || currItem->d_name[0] == '.'){
                continue;
            }
            
            //put lock here
            pthread_mutex_lock(&counter_lock); 
            
            globalCnt = globalCnt +1;
            pthread_mutex_unlock(&counter_lock); 
            localCnt = localCnt + 1;
            //set fields of sortParam
            //SortParams sort; 
            //SortParams *sortDetails = &sort; 
            SortParams * sortDetails = (SortParams*)malloc(sizeof(SortParams)); 
            

            sortDetails->filename = currItem->d_name; 
            
            //sortDetails->path = params->currPath;
            
            //**uncomment**//
            /*printf("Params->currPath:%s\n", params->currPath); 
            fflush(stdout);*/
            
            
            csvpath = createPath(params->currPath,currItem->d_name); 
            sortDetails->path = csvpath; 
            sortDetails->inputDetails = params; 
        
        /*pthread_mutex_lock(&path_lock);
            if (pathHead == NULL){
                pathHead = init(NULL,NULL,csvpath,0, 0,NULL); 
            } else{
                Node *adder = init(NULL,NULL,csvpath,0, 0,NULL);  
                pathHead = append(adder, pathHead); 
            }
            pthread_mutex_unlock(&path_lock);*/
        
        //**uncomment**//
        /*printf("**SortParam**\n"); 
        fflush(stdout); 
        printSorter(sortDetails);*/
        
         
            //create thread, call sorter


            //this will handle CSVs
            if (head == NULL){
                head= init(NULL,currItem->d_name,sortDetails->path, pthread_self(),0,NULL,sortDetails); 
            } else{
                Node *addSort = init(NULL,currItem->d_name, sortDetails->path, pthread_self(),0,NULL,sortDetails); 
                head = append(addSort,head); 
            }

            
            Node *currSort = head;
            //get last node
            while(currSort->next!=NULL){
                currSort = currSort->next;
            }
            
        //**uncomment**//
            /*printf("\nNODE\n");
            print(head);
            printf("\n");*/



            //create thread and check if valid
            //printf("**Before creating thread**\n"); 
            
            //printf("sortDetails->filename:%s\n", sortDetails->filename); 
            //fflush(stdout); 
            //printSorter(sortDetails); 
            int ret = pthread_create (&currSort->c_tid,NULL,&sorter,(void*)sortDetails); 
            if (ret != 0){
                printf("Error creating thread\n"); 
                fflush(stdout);
                exit(1); 
            }
        }
    }
    
    //join threads following the LL
    
    
    //**uncomment**//
    /*printf("Outside of main while loop\n"); 
    fflush(stdout);*/
    
     
    //free(params->currPath); 
    
    //will also have to free the linked list Nodes
    if (head!=NULL){
    	printf("\n"); 
    	printf("INITIAL PID:%lu\n",pthread_self()); 
    	//printf("TIDS of all child threads: ");
    	fflush(stdout); 
    	
        Node * dir_tracker = head;
        //pthread_mutex_lock(&print_lock);
        printLL(head); 
        //pthread_mutex_unlock(&print_lock);
        //loops through the directory LL with children tids
        //used for parent thread to know child thread has exited
        while (dir_tracker!=NULL){
            //join using the current node's tid
            //look into the second parameter (temporarily NULL)
            
            //**uncomment**//
            //printf("My tid:%u\n",pthread_self()); 
            //printf("Joining with c_tid:%u\n",dir_tracker->c_tid); 
            //fflush(stdout); 
            //printf("Joining with file:%s\n",dir_tracker->filename);
            //fflush(stdout); 
            //printf("%u,",dir_tracker->c_tid); 
            int success = pthread_join(dir_tracker->c_tid, NULL);
            /*if (success == 0){
                printf("JOINING SUCCESS\n"); 
                fflush(stdout); 
            }*/
            free(dir_tracker->path);
            
            //free(dir_tracker->filename); 
            
            if (dir_tracker->input !=NULL){
                //free(dir_tracker->input->currPath); 
                free(dir_tracker->input); 
                
            }
             if (dir_tracker->sortInput !=NULL){ 
                //free(dir_tracker->sortInput->path);
                free(dir_tracker->sortInput);  
                
            }
            if (curr!=NULL){
                //free(curr); 
            }
            if (csvpath!=NULL){
                //free(csvpath); 
            }
            
            
            
            //printf("Join successful c_tid: %u\n", dir_tracker->c_tid);
            //printf("Joining with file:%s\n",dir_tracker->filename);
            //fflush(stdout); 
            Node * delete = dir_tracker; 
            dir_tracker = dir_tracker->next; 
            free(delete);
            
             
            
        }
        //printf("Total number of threads:%d\n",localCnt);
    	//fflush(stdout); 
    }
    
    /*Node * path_tracker = pathHead; 
    while(path_tracker!=NULL){
    
        //free(path_tracker->path); 
        Node * delPath = path_tracker; 
        path_tracker = path_tracker->next;
        //free(delPath); 
    }*/
    //free(inDetails); 
    //free(sortDetails); 
    //temporarily with () 
    if(pthread_self() == parent){
    	closedir(root); 
        return; 
    } 
    closedir(root); 
    pthread_exit(NULL);
    
     
    //pthread_exit(NULL);
}

void printLL(Node * head){
	printf("TIDS of all child threads: ");
    	fflush(stdout);
    	int count = 0; 
	Node * curr = head;
	while (curr!=NULL){
		printf("%lu,",curr->c_tid); 
		fflush(stdout); 
		count++; 
		curr = curr->next;
	}
	printf("\n"); 
	printf("Total number of threads:%d\n",count); 
	fflush(stdout); 
	return; 

}
void outputFile(char * outPath,char *topic){
    
   
    
    int len = strlen(outPath)+strlen("AllFiles-sorted-")+strlen(topic)+strlen(".csv")+1; 
    char *path = (char*)malloc(len*sizeof(char));
    path[len-1] = '\0'; 
    if(path == NULL){
        printf("Failed to allocate memory\n");  
        exit(1);  
    }
    strcpy(path,outPath);
    strcat(path,"/");   
    strcat(path,"AllFiles-sorted-"); 
    strcat(path,topic); 
    strcat(path,".csv"); 
    
    //printf("Path:%s\n",path); 
    //fflush(stdout); 
    //pthread_mutex_unlock(&path_lock);


    
    FILE *outfp = fopen(path, "w");
     //printf("Inside outputFile\n"); 
    //fflush(stdout); 
    int i;
    fprintf(outfp, firstRow);
    for(i = 0; i < arrCount; i++) {
    	//printf("%d\n",i); 
        fprintf(outfp, centralArr[i] -> original_row);
    }
     
    int close = fclose(outfp); 
   
    if (close==EOF){
	printf("Failed to close file\n"); 
	fflush(stdout);  
	return;   	
    }
    int index = 0; 
    
    while(index<arrCount){
        free(centralArr[index]->field_data); 
        
        free(centralArr[index]->original_row); 
        free(centralArr[index]);
        
        index++; 
    }
    
    free(path); 






}

int main(int argc, char **argv){

    int c_flag = 0; //category flag
    int d_flag = 0; //input directory flag
    int o_flag = 0; //output directory flag


    InputParams inputDetails;
    char * outputPath;   
    char * topic; 
    //InputParams *inputDetails = (InputParams*)malloc(sizeof(InputParams));
    int args; //placeholder for arg index
    for(args = 0; args < argc; args++){
        //find place of category flag
        if (strcmp(argv[args], "-c") == 0){
            if (argv[args+1]){

                inputDetails.sortTopic = argv[args+1];
                topic = argv[args+1];
                //inputDetails->sortTopic = argv[args+1];
                c_flag = 1;
            } else {
                printf("Invalid\n");
                exit(0);
            }
        }

        //get input directory
        if (strcmp(argv[args], "-d") == 0){
            if (argv[args+1]){
                inputDetails.currPath = argv[args+1];
                //inputDetails->currPath = argv[args+1];
                d_flag = 1;
            } else {
                printf("Invalid\n");
                exit(0);
            }
        }
        
        //get output directory
        if (strcmp(argv[args], "-o") == 0){
            if (argv[args+1]){
                inputDetails.outputDir = argv[args+1];
                outputPath = argv[args+1]; 
                //inputDetails->outputDir = argv[args+1];
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
        inputDetails.currPath = ".";
        
        //inputDetails->currPath = ".";
    }
    
    if (o_flag == 0){
    
    
    	
        inputDetails.outputDir = ".";
        outputPath = "."; 
        //inputDetails->currPath = ".";
    }

    int valid_input_flag = 0;
    		char*catstring=strdup("color,director_name,num_critic_for_reviews,duration,director_facebook_likes,actor_3_facebook_likes,actor_2_name,actor_1_facebook_likes,gross,genres,actor_1_name,movie_title,num_voted_users,cast_total_facebook_likes,actor_3_name,facenumber_in_poster,plot_keywords,movie_imdb_link,num_user_for_reviews,language,country,content_rating,budget,title_year,actor_2_facebook_likes,imdb_score,aspect_ratio,movie_facebook_likes");
    
    
    char *cat; //pulled category from strsep
    char *sepcpy = catstring;
    
    int index = 0; 
    while((cat = strsep(&catstring, ",")) != NULL){
	if(strcmp(cat, inputDetails.sortTopic)==0){
		col = index;
		//printf("%s\n",inputDetails.sortTopic); 
		//fflush(stdout); 
		valid_input_flag = 1;
	}
	index++;
    }
    //printf("index:%d\n",col); 
    //fflush(stdout); 
   if (valid_input_flag == 0){
		printf("Invalid input!\n");
		exit(1);
   }
    free(sepcpy); 
    //printf("o_flag:%d\n",o_flag); 
    if (o_flag==1){
	    DIR *outDir = NULL;
	    outDir = opendir(outputPath);
	    //printf("outputpath:%s\n",outputPath); 
	    //fflush(stdout); 
	    if (outDir==NULL){
	    	printf("Invalid output directory\n");
	    	fflush(stdout);
	    	closedir(outDir);  
		return;
	    }
	    closedir(outDir); 
    }
    /*struct stat statbuf; 
    if (stat(outputPath,&statbuf)!= -1){
    	printf("Invalid output path\n"); 
    	fflush(stdout); 
    	return; 
    }
    else{	
    	if(S_ISDIR(statbuf.st_mode)){
    	
	    	printf("Invalid output path\n"); 
	    	fflush(stdout); 
	    	return; 
    	}
    }*/
    
    //checks for -c flag to be inputted
    //p = getpid();
    
    //void * a = traverseDir((void*)inputDetails);
    parent = pthread_self(); 
    printf("MAIN PID:%lu\n",pthread_self()); 
    fflush(stdout); 
    //printf("TIDS of all child threads: "); 
    //fflush(stdout); 
    traverseDir((void*)&inputDetails);
    //traverseDir((void*)inputDetails);
    //printf("\n"); 
    //fflush(stdout); 
    //printf("End of traverseDir\n"); 
    //fflush(stdout); 
    printf("\n"); 
    printf("Total number of threads at program end:%d\n",globalCnt);
    fflush(stdout); 
    
    
    
    /*int p = 0; 
    for (;p<arrCount; p++){
    	printf("Arr[%d]:%s\n",p,centralArr[p]->field_data); 
    }
    printf("arrCount:%d\n", arrCount); 
    printf("arrLim:%d\n", arrLim); 
    fflush(stdout); */
    
    merge_sort(centralArr,0,arrCount-1); 
    
    /*int h = 0; 
    for (;h<arrCount; h++){
    	printf("Arr[%d]:%s\n",h,centralArr[h]->field_data); 
    }*/
    outputFile(outputPath,topic); 
    
    
    
    
    
    
    free(centralArr); 
    free(firstRow); 
    //free(inputDetails); 
    // pthread_t begin;
    // int ret = pthread_create(&begin, NULL, traverseDir, inputDetails);
    

    //free(inputDetails);
    return 0;

}





/*DirA
  DirB DirC
  
A's linked list
DirB tid -> DirC tid
  
  
  
DirA
  DirB  hi.csv 
    DirC

A's linked list
  DirB tid->hi.csv tid-> NULL
B's linked list 
  DirC tid */






