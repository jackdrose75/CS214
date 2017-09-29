#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct movie_data{
    char* cat_data; //holds category data (i.g. "Titanic" for category "movie_title")
    char* row_array; //holds pointer to array of row.
} Data;


int main(int argc, char **argv){

    //stores the input from the user as a string variable
    char *sort_type = argv[1]; //get argument to sort by, -c for column
    char *sort_topic = argv[2]; //get topic i.e. 'movies'
    int num = 1;

    //get categories inputted by use
    //match by index number of column
    char *first_row = (char *)malloc(num*sizeof(char *));
    char *first = first_row; //need to keep track for strsep
    fgets(first_row, 1024, stdin);
    char *found;
    int count = 0;
    while((found = strsep(&first, ",")) != NULL){
        if(strcmp(found, sort_topic) == 0){
            break;
        }
        count++;
    }
    free(first_row); //get rid of space used to hold first row

    //get input line by line from stdin
    char *str_line = (char *)malloc(num*sizeof(char *));
    while(fgets(str_line, 1024, stdin) != NULL){
        printf("%s\n", str_line);
        
    }

    return  0;

}




