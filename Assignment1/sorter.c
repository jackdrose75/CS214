#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct movie_data{
    char* cat_data; //holds category data (i.g. "Titanic" for category "movie_title")
    char* row_array[]; //holds pointer to array of row.
} Data;

//used to parse a string line based on delimiter commas and quotations
//builds character array (string) dynamically
//returns pointer to built string
char *parseStr(char *str){
    
    //build string as character array
    int i = 0;
    char c;
    str = (char *)malloc(sizeof(char)); //build string as character array

    if (str == NULL){
        return NULL; 
    }
    //parse until end of line
    while((c = getchar()) != '\n' && (c != ',') && (c != '\0')){

        //if encounter opening quote
        if(c == '\"'){
            while((c = getchar()) != '\"') {
                //realloc(str, (sizeof(char)));
                str = realloc(str, (i*sizeof(char)));
                str[i++] = c;
            }
        } else {
            //
            str = realloc(str, (sizeof(char)));
            str[i++] = c;
        }
    }

    str[i] = '\0';
    printf("String contains %s\n", str);
    return str;
}


int main(int argc, char **argv){

    //stores the input from the user as a string variable
    char *sort_type = argv[1]; //get argument to sort by, -c for column
    char *sort_topic = argv[2]; //get topic i.e. 'movies'


    Data test;
    printf("Here\n");
    int num = 1; //to init array
    Data **data_array = (Data **)malloc(num*sizeof(Data **)); //init data_array to single unit of type Data

    //get input line by line from stdin
    char str_line[1024]; //hold string line
    while(fgets(str_line, 1024, stdin) != NULL){
        printf("pass to parse function\n");
        printf("%s\n", parseStr(str_line));
    }

    return  0;

}