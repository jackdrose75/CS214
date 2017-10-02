#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>

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
    for(sub_str = strtok(input, ","); sub_str && *sub_str; sub_str = strtok(NULL, ",\n")){
        if (!--index){
            return sub_str;
        }
    }

    return NULL;

}



int main(int argc, char **argv){

    //stores the input from the user as a string variable
    char *sort_type = argv[1]; //get argument to sort by, -c for column
    char *sort_topic = argv[2]; //get topic i.e. 'movies'

    //get categories inputted by use
    //match by index number of column
    char *first_row = (char *)malloc(sizeof(char *));
    char *_first_row = first_row; //need to keep track for strsep
    fgets(first_row, 1024, stdin); //get first row
    char *cat; //pulled category from strsep
    int cat_index = 0; //index where category is
    while((cat = strsep(&_first_row, ",")) != NULL){
        if(strcmp(cat, sort_topic) == 0){
            printf("%s, %d\n", cat, cat_index);
            break;
        }
        cat_index++;
    }
    free(first_row); //get rid of space used to hold first row


    //initialize array of structs
    Record **recordList = (Record **)malloc(sizeof(Record **));

    printf("TEST\n");    

    //get input line by line from stdin
    char *row = (char *)malloc(sizeof(char *));



    int index = 0;
    int size = 1;
    while(fgets(row, 1024, stdin) != NULL){
        
        //temporary struct to hold parsed data
        Record *tmpList;
        tmpList = (Record *)realloc(recordList, 1*sizeof(Record *));
        char *tmp = getCat(strdup(row), cat_index); //send row and cat index into getCat to be parsed, extract pulled field_data

        if (!tmp){
            tmpList -> field_data = NULL;
        } else {
            tmpList -> field_data = tmp;
        }
        tmpList -> original_row = row;
        
        //resize recordList array
        recordList = (Record **)realloc(recordList, size*sizeof(Record *));
        recordList[index] = tmpList;
        index++;
        size++;
    }

    printf("Here\n");

    free(recordList);
    free(row);
    return 0;

}




