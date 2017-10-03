#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
//#include "sorter.h"


//
typedef struct{
    char *field_data; //holds category data (i.g. "Titanic" for category "movie_title")
    char *original_row; //holds pointer to array of row.
} Record;
//

void merge(Record** a, int low, int mid, int high){
    
    int topL = mid - low + 1; 
    int bottomL = high - mid;
    
    Record** top = (Record **)malloc(sizeof(Record **));
    Record** bottom = (Record **)malloc(sizeof(Record **));

    int i;
    for(i = 0; i < topL ; i++){
        //top[i] = malloc(sizeof(a[low+1]));
        top[i] = malloc(sizeof(Record));
        top[i] = a[low + 1];
        // strcpy(top[i], a[low+i]);
    }
    for(i = 0; i < bottomL; i ++){
        //bottom[i] = malloc(sizeof(a[mid+i+1])); 
        bottom[i] = malloc(sizeof(Record));
        bottom[i] = a[mid+i+1];
        // strcpy(bottom[i], a[mid+i+1]);
    }

    int j = 0;
    int k = 0;
    
    k = low;
    i = 0; 
    
    while(i < topL && j < bottomL){
        if(strcmp(top[i] -> field_data, bottom[j] -> field_data) < 0){
            a[k++] = top[i++];
            //strcpy(a[k++] -> field_data, top[i++] -> field_data) ;
        }
        else{
            a[k++] = bottom[j++];
            //strcpy(a[k++] -> field_data, bottom[j++] -> field_data);
        }
    }

    free(top);
    free(bottom);
}

void merge_sort(Record** a, int low, int high){
    
    if (low < high){
        int mid = (low + high)/2;
        merge_sort(a, low, mid);
        merge_sort(a, mid+1, high);
        merge(a, low, mid, high);
    }
} 

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//returns category substring
char *getCat(char *input, int index){

    char *sub_str = NULL; //substring we want to return
    //check to see if input is null, else set substring ptr to input
    for(sub_str = strtok(input, ",\""); sub_str && *sub_str; sub_str = strtok(NULL, ",\n")){
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
        tmpList = (Record **)realloc(recordList, 1*sizeof(Record **));

        if(tmpList == NULL){
            printf("Error with tmpList\n");
            return 0;
        }
        recordList = tmpList;
        char *tmp = getCat(strdup(_row), cat_index); //send _row and cat index into getCat to be parsed, extract pulled field_data

        recordList[index] = malloc(sizeof(Record));

        recordList[index] -> field_data = tmp;
        printf("recordList[%d] -> field_data: %s\n", index, recordList[index]->field_data);
        
        recordList[index] -> original_row = _row;
        printf("recordList[%d] -> original_row : %s\n", index, recordList[index]->original_row);

        index++;
        free(_row);
    }

    printf("HELLO WORLD\n");


    //merge sort recordLists by field_data
    merge_sort(recordList, 0, index);



    //printf("Here\n");
    free(recordList);
    return 0;

}




