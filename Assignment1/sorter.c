#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include "sorter.h"
#include "mergesort.c"

// Main file to parse and pass data

//returns category substring
char *getCat(char *input, int index){

    char *sub_str = NULL; //substring we want to return
    //check to see if input is null, else set substring ptr to input
    for(sub_str = strtok(input, ","); sub_str && *sub_str; sub_str = strtok(NULL, ",\n")){
        if (!index--){
            return sub_str;
        }
    }

    return NULL;


/*
    char *sub_str = NULL; //substring we want to return
    bool inQuote = false;

    int index_count = 0;

    while(*input){
        switch(char){
            case ',':
                index_count++;
                if(inQuote == false){

                }
                break;
            case '\"':
                inQuote = !inQuote;

            default:

        }
    }
*/
}

//parse substring until next delimiter and return
char *stringBuild(char *rawString, char *delimiter){
    return NULL;
}



int main(int argc, char **argv){

    const int MAXSIZE = 1024;

    //stores the input from the user as a string variable
    char *sort_type = argv[1]; //get argument to sort by, -c for column
    char *sort_topic = argv[2]; //get topic i.e. 'movies'

    //get categories inputted by use
    //match by index number of column
    char first_row[MAXSIZE];// = (char *)malloc(MAXSIZE);
    fgets(first_row, MAXSIZE, stdin); //get first row
    char *_first_row = first_row; //need to keep track for strsep

    char *cat; //pulled category from strsep
    int cat_index = 0; //index where category is
    while((cat = strsep(&_first_row, ",")) != NULL){
        if(strcmp(cat, sort_topic) == 0){
            //printf("%s, %d\n", cat, cat_index);
            break;
        }
        cat_index++;
    }

    //free(first_row); //get rid of space used to hold first row

    //initialize array of structs
    Record **recordList = (Record **)malloc(sizeof(Record **));

    //get input line by line from stdin
    int index = 0;
    char row[MAXSIZE];

    while(fgets(row, MAXSIZE, stdin) != NULL){

        char *_row = strdup(row);

        //temporary struct to hold parsed data
        Record **tmpList;
        //tmpList = (Record **)realloc(recordList, 1*sizeof(Record **));
        tmpList = (Record **)malloc(1*sizeof(Record **));

        if(tmpList == NULL){
            printf("Error with tmpList\n");
            return 0;
        }
        //recordList = tmpList;
        tmpList = recordList;
        char *tmp = getCat(strdup(_row), cat_index); //send _row and cat index into getCat to be parsed, extract pulled field_data

        recordList[index] = malloc(sizeof(Record));
        recordList[index] -> field_data = tmp;
        printf("recordList[%d] -> field_data: %s\n", index, recordList[index]->field_data);
        
        recordList[index] -> original_row = _row;
        printf("recordList[%d] -> original_row : %s\n", index, recordList[index]->original_row);

        index++;
        free(_row);
    }

    printf("++++++");
    printf("BEFORE MERGESORT\n");
    int i;
    for(i = 0; i < index; i++) {
        printf("%s ", recordList[i] -> field_data);
    }
    printf("\n");
    printf("++++++");

    //merge sort recordLists by field_data
    merge_sort(recordList, 0, index);

    //printf("Here\n");
    free(recordList);
    return 0;

}




