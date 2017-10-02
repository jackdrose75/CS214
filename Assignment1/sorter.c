#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>

//
typedef struct{
    char *category; //holds category data (i.g. "Titanic" for category "movie_title")
    char *original_row; //holds pointer to array of row.
} Record;


//split row into an array of substrings
//returns an array
char *getCat(char *input, int index){
    char *sub_str = NULL; //substring we want to return

    //check to see if input is null, else set substring ptr to input
    if (input == NULL){
        return NULL;
    }
    sub_str = input;

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
    char *field_data; //pulled category from strsep
    int field_index = 0;
    int row_size = 1;
 
    while((field_data = strsep(&_first_row, ",")) != NULL){
        if(strcmp(field_data, sort_topic) == 0){
            field_index = row_size;  //index of where category is
            printf("%s, %d\n", field_data, field_index);
        }
    }
    free(first_row); //get rid of space used to hold first row

    //initialize array of structs
    Record **outputArray = (Record **)malloc(sizeof(Record **));
    
    //get input line by line from stdin
    char *row = (char *)malloc(sizeof(char *));
    int index = 0;
    int size = 1;
    while(fgets(row, 1024, stdin) != NULL){
        if (getCat(row, field_index) == NULL){
            outputArray[index] -> category = NULL;
        }
        outputArray[index] = (Record *)realloc(outputArray, size*sizeof(Record *));
        outputArray[index] -> category = field_data;
        outputArray[index] -> original_row = row;
        size++;
    }

    printf("Here\n");

    free(row);
    free(outputArray);
    return 0;

}




