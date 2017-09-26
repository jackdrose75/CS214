#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct type{
 
    char* value ;
};

//used to save strings into struct array
/*char *token_string(char *str, int sizeofArray){
    struct type array_input;



}
*/

int main(int argc, char **argv){


    //stores the input from the user as a string variable
    char *sort_type = argv[1]; //get argument to sort by, -c for column
    char *sort_topic = argv[2]; //get topic i.e. 'movies'

/*
    //catch if -c is not implemented
    if (strcomp(sort_type, "") == 0) {
        printf("Please specify type of sorting required. \n");
        return 0;

    //catch if movie not implemented
    if (strcomp(sort_topic, "") == 0) {
        printf("Please specify topic to sort by. \n");
        return 0;
    }
*/

    //get stdin input
    char temp[1000]; //buffer input 
    //fgets(temp, 1000, stdin);
    while(scanf("%s",temp) != EOF) {
        //printf("%s\n", temp);        
    }


    char* ptr = temp;

    //store an array holding column entry
    struct type entry;
    entry.value = temp ;

    //tokenization using comma delimiter
    char* tokenp ;
    tokenp = strtok(entry.value, ",") ; //separate by comma
    
    // get categories
    while(tokenp != NULL){
        printf("%s \n", tokenp);
        tokenp = strtok(NULL, ",") ;
    }



    return  0;

}