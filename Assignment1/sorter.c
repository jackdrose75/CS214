#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include <ctype.h>
#include "sorter.h"


// Main file to parse and pass data

//Returns category substring
//parses through input string by looking for delimiters
//pStart marks beginning of category word, pEnd marks end by delimiter
//finds out length of substring
//finds length of substring by substracting pEnd and pStart
//malloc space for new char array and copy characters to it 
void *getCat(char *line, int catIndex){

    char *pStart = line; //keeps track of beginning of substring
    char *pEnd = line; //continues until finds substring index

    int strIndex = 0;
    int strLen = 0;
    bool inQuote = false;

    // flags to check if substring is float or int
    bool isInt = false;
    int numDots = 0;
    bool isFloat = false;

    while(*pEnd){
        switch(*pEnd){
            case ',':
                if(inQuote == false){
                    //build string
                    if(strIndex == catIndex){
                        //create and return substring
                         //char *str = buildString(pStart, pEnd);
                         //printf("%s\n", str);
                        return buildString(pStart, pEnd, isInt, isFloat);;
                    } 
                    
                    //if strIndex != catindex, move pStart to pEnd
                    pEnd++;
                    pStart = pEnd;
                    strIndex++; //update strIndex

                    //reset flags
                    isInt = false;
                    numDots = 0;
                    isFloat = false;

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
                    //reset flags
                    isInt = false;
                    numDots = 0;
                    isFloat = false;

                } else {
                    //build string
                    inQuote = !inQuote;
                    if (strIndex++ == catIndex){
                        pEnd--;
                        return buildString(pStart, pEnd, isInt, isFloat);
                    }
                    pEnd = pEnd+2;
                    pStart = pEnd;
                    //reset flags
                    isInt = false;
                    numDots = 0;
                    isFloat = false;
                }

            default:
                //check if character is not a digit
                if(!isdigit(*pEnd)){
                    //check if float
                    if(*pEnd == '.'){
                        if (isInt && (numDots == 0)){
                            //is a float so far
                            isFloat = true;
                            isInt = false;
                        } else if (isInt && (numDots > 0)){
                            //is a char
                            isFloat = false;
                            isInt = false;
                        }
                        numDots++;
                    } else {
                        isInt = false;
                        isFloat = false;
                    }
                } else if((pEnd-pStart) == 0){
                    //start of char
                    isInt = true;
                }

                pEnd++;
        }
    }

    return "\0";
}

void *buildString(char *start, char *end, bool isInt, bool isFloat){

    int strLen = end - start;
    void *buffer;
    if(!isInt && !isFloat){
        buffer = (char *)malloc((strLen+1)*sizeof(char *));
    } else if (isInt){
        buffer = (int *)malloc((strLen+1)*sizeof(int));
    } else{
        buffer = (float *)malloc((strLen+1)*sizeof(float));
    }
    
    //copies chars from one string to another for strLen
    memcpy(buffer, start, strLen);
    //if(!isInt && !isFloat){
    //    buffer[strLen+1] = '\0';
    //}

    // if isAllInts:
        // recordList[] -> type = 0
    return buffer;
}


int main(int argc, char **argv){

    const int MAXSIZE = 1024;

    //stores the input from the user as a string variable
    char *sortType = argv[1]; //get argument to sort by, -c for column
    char *sortTopic = argv[2]; //get topic i.e. 'movies'

    //get categories inputted by use
    //match by index number of column
    char first_row[MAXSIZE];// = (char *)malloc(MAXSIZE);
    fgets(first_row, MAXSIZE, stdin); //get first row
    char *_first_row = first_row; //tmp to keep track for strsep

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
        return 0;
    }

    //initialize array of structs
    Record **recordList = (Record **)malloc(sizeof(Record **));

    //get input line by line from stdin
    int index = 0;
    char row[MAXSIZE];

    while(fgets(row, MAXSIZE, stdin) != NULL){

        char *_row = strdup(row);

        //temporary struct to hold parsed data
        Record *tmpList;
        tmpList = (Record *)malloc(sizeof(Record *));

        /*
        if(tmpList == NULL){
            printf("Error with tmpList\n");
            return 0;
        }
        */

        //parse string for category field_data (i.e. director_name => James Cameron)
        char *tmp = getCat(strdup(_row), cat_index); //send _row and cat index into getCat to be parsed, extract pulled field_data
        //test ifdigit
/*        if(isdigit(*tmp)){
            printf("PRINTING DIGITS\n");
            int _tmp = atoi(tmp);
        }
*/
        //save category into tmpList.field_data
        tmpList -> field_data = tmp;
        //printf("tmpList -> field_data: %s\n", tmpList->field_data);        
        //save original string row to tmpList.original_row
        tmpList -> original_row = _row;
        //printf("tmpList -> original_row : %s\n", tmpList->original_row);

        //add tmpList struct to array of structs recordList[index] at index
        recordList = (Record **)realloc(recordList, (index+1)*sizeof(Record **));
        recordList[index] = (Record *)malloc(sizeof(Record *)); 
        recordList[index] = tmpList;
/*
        printf("recordList[%d] -> field_data: %s\n", index, recordList[index]->field_data);        
        printf("recordList[%d] -> original_row : %s\n", index, recordList[index]->original_row);
*/
        index++;
    }


    printf("\n\n++++++");
    printf("BEFORE MERGESORT\n");
    int i;
    for(i = 0; i < index; i++) {
        printf("%s\n", recordList[i] -> field_data);
    }
    printf("\n");
    printf("++++++");

    //merge sort recordLists by field_data
    // printf("\nindex-1: %d\n", index-1);

    merge_sort(recordList, 0, index-1);

    printf("\n\n++++++");
    printf("AFTER MERGESORT\n");
    for(i = 0; i < index; i++) {
        printf("%s\n", recordList[i] -> field_data);
    }
    printf("\n");
    printf("++++++");


    //printf("Here\n");
    free(recordList);
    return 0;

}




