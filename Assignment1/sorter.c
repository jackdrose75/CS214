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
char **itemize_row(char *input){
    char *sub_str = NULL; //substring we want to return

    //check to see if input is null, else set substring ptr to input
    if (input == NULL){
        return NULL;
    }
    sub_str = input;

    //delimit based on comma and quotations
    bool inQuote = false;
    int size = 1;
    printf("TEST\n");
    char **buffer = (char **)malloc(size*sizeof(char **));

    //char *buffer;
    printf("POST-TEST\n");
    while(*sub_str){
        switch(*sub_str){
            case ',':
                printf("case 1\n");
                if(inQuote == false){
                    buffer = (char **)realloc(buffer, size*sizeof(char **));
                    buffer[size-1] = sub_str;
                    size++;
                    sub_str++;
                }
                break;

            case '\"':
                printf("case 2\n");
                inQuote = !inQuote;

            case '\n':
                printf("case 3\n");
                buffer = (char **)realloc(buffer, size*sizeof(char **));
                buffer[size-1] = NULL;
                break;

            default:
                printf("default case\n");
                sub_str++;
        }
    }
    return buffer;

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
    int cat_index = 0;
    int row_size = 1;

    char **testArray;
    printf("PRE-TEST\n");
    testArray = (char **)malloc(1*sizeof(char *));
    printf("POST-TEST\n");

    while((cat = strsep(&_first_row, ",")) != NULL){
        if(strcmp(cat, sort_topic) == 0){
            cat_index = row_size;  //index of where category is
        }
        testArray = (char **)realloc(testArray, row_size*sizeof(char **));
        testArray[row_size-1] = cat;
        printf("%s\n",testArray[row_size]);
        row_size++;
    }
    free(first_row); //get rid of space used to hold first row

    free(testArray);

    //get input line by line from stdin
    char *row = (char *)malloc(sizeof(char *));
    char *_row = row;

    while(fgets(row, 1024, stdin) != NULL){
        itemize_row(row);
/*        while((item = strsep(&_row, ",")) != NULL){
            if(_row == NULL){
                break;
            }
            else if(*_row == "\""){
                item++;
                char *_item = item;
                item = strsep(&_row, "\"");
            } else {
                //printf("%s\n", item);
            }
        }
        */
    }

    return 0;

}




