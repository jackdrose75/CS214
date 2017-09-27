#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct movie_data{
    char* cat_data; //holds category data (i.g. "Titanic" for category "movie_title")
    char* row_array; //holds pointer to array of row.
} Data;

//used to parse a string line based on delimiter commas and quotations
//builds character array (string) dynamically
//returns pointer to built string
char *strParser(char *input){
  int i = 0;
  char c;

  if(input == NULL) {
    return NULL;
  }
  
  //traverse char by char of input
  while((input[i] != '\n')) {
    realloc(input, (sizeof(char)));
    i++;
  }

  input[i] = '\0';
  printf("\nYou entered the string: %s\n", input);

}


int main(int argc, char **argv){

    //stores the input from the user as a string variable
    char *sort_type = argv[1]; //get argument to sort by, -c for column
    char *sort_topic = argv[2]; //get topic i.e. 'movies'

    int num = 1;

    //get input line by line from stdin
    char *str_line = (char *)malloc(num*sizeof(char *));

    while(fgets(str_line, 1024, stdin) != NULL){
        printf("About to enter for loop\n\n");
        char *test = strParser(str_line);

    }

    return  0;

}