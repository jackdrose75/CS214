#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>

#define maxlen 100000

//
typedef struct{
    char *field_data; //holds category data (i.g. "Titanic" for category "movie_title")
    char *original_row; //holds pointer to array of row.
} Record;


//split row into an array of substrings
//returns an array
char *getCat(char *input, int index){
    char *sub_str = NULL; //substring we want to return
    //check to see if input is null, else set substring ptr to input
    for(sub_str = strtok(input, ",\""); sub_str && *sub_str; sub_str = strtok(NULL, ",\n")){
        if (!index--){
            return sub_str;
        }
    }

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

    printf("%s\n", cat);

    //free(first_row); //get rid of space used to hold first row

    //initialize array of structs
    Record **recordList = (Record **)malloc(sizeof(Record **));

    //get input line by line from stdin
    int index = 0;
    int size = 1;
    char row[MAXSIZE];

    while(fgets(row, MAXSIZE, stdin) != NULL){

        char *_row = strdup(row);
        printf("_row : %s\n", _row);
        //temporary struct to hold parsed data
        Record **tmpList;
        tmpList = (Record **)realloc(recordList, 1*sizeof(Record **));
        if(tmpList == NULL){
            printf("Error with tmpList\n");
            return 0;
        }
        recordList = tmpList;
        char *tmp = getCat(strdup(_row), cat_index); //send _row and cat index into getCat to be parsed, extract pulled field_data

        if (!tmp){
            printf("!tmp: %s\n", tmp);
            //tmpList -> field_data = NULL;
            strncpy(recordList[index] -> field_data, NULL, MAXSIZE);
        } else {
            printf("tmp: %s\n", tmp);
            //tmpList -> field_data = tmp;
            printf("index : %d\n", index);
            strncpy(recordList[index] -> field_data, tmp, MAXSIZE);
            printf("recordList[%d] : %s\n", index, recordList[index]->field_data);
        }
        //tmpList -> original_row = _row;
        printf("tmp otherwise: %s\n", _row);
        strncpy(recordList[index] -> original_row, _row, MAXSIZE);
        
        //resize recordList array
        // recordList = (Record **)realloc(recordList, size*sizeof(Record *));
        // recordList[index] = tmpList;
        index++;
        size++;
        free(_row);
    }

    //printf("Here\n");
    free(recordList);
    return 0;

}




