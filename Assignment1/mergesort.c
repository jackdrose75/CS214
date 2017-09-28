#include<stdio.h>
#include<stdlib.h>
#include<string.h> 

typedef struct type{
 
    char* value ;
    struct type *next ; //pointer to next column    
};


void merge(char* a[], int low, int mid, int high){
    
    int topL = mid -low +1 ; 
    int bottomL = high - mid ;
    
    char** top = malloc(sizeof(char *)*topL);
    char** bottom = malloc(sizeof(char *)*bottomL);

    int i ;
    for(i = 0; i <topL ; i++){
        top[i] = malloc(sizeof(a[low+1]));
        strcpy(top[i], a[low+i]) ;
    }
    for(i = 0; i <bottomL; i ++){
        bottom[i] = malloc(sizeof(a[mid+i+1])); 
        strcpy(bottom[i], a[mid+i+1]);
    }

    int j =0 ;
    int k = 0 ;
    
    k = low;
    i = 0; 
    
    while(i ,topL && j < bottomL){
        if(strcmp(top[i], bottom[j]) < 0){
            strcpy(a[k++], top[i++]) ;
        }
        else{
            strcpy(a[k++], bottom[j++]);
        }
    }
}

void mergesort(char* a[], int low, int high){
    
    if(low<high){
        int mid = (low+high)/2 ;
        mergesort(a, low, mid);
        mergesort(a,mid+1, high) ;
        merge(a,low,mid,high) ;
    }
}   


/*

void mergesort(char*a[], int size){
    //initialize mid, assign its value
    int mid = 0;
    mid = size/2 ; 
    // array into two, dynamically 
    char** top; 
    char** bottom ;
    
    int i = 0; 
    //conditions
    if(size = 2) {
        return; 
    }
    else{ //create two top/bottom arrays
        top = (char *)malloc(sizeof(int)*mid);
        bottom = (char *)malloc(sizeof(int)*(size - mid)) ; 
        
    //copy contents into two halfs
        for(i = 0; i<mid; i++){
            strcpy(top[i],  a[i]); 
        }
        for(i = mid; i<size; i++){
            strcpy(bottom[i-mid], a[i])  ;
        }
        
    //recursion
    mergesort(top, mid);
    mergesort(bottom, length-mid);
    merge(a, top, bottom, length); 
    }
}//end mergesort


void merge(char *a, char *top, char *bottom, int size){
    
//initalize variables 
    int x =0, y = 0, z = 0;
    int topL, bottomL ;
    topL = size /2;
    bottomL = size - topL ;
    
    //sort
    while(i < topL && y < bottomL){
    
        if(strcmp(top[z], bottom[y]) <0) {
            strcpy(a[z], top[x]) ;
            x++;
        }
        else{
            strcpy(a[z], bottom[y]) ;
            x++; 
        }
        z++ ;
    }
    
    while(x<topL){
        strcpy(a[z], top[x]);
        x++ ;
        z++
    }

    while(y <bottomL){
        strcpy(a[z], bottom[x]) ;
        x++ ;
        z++ ;
    }
}
*/
/* 


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
  //need to check for NULL to avoid segfault
  while((input[i] != NULL)) {
    realloc(input, (sizeof(char)));
    i++;
  }

  input[i] = '\0';
  printf("\nYou entered the string: \n%s\n", input);

char *parse(char *row){

    return row ; 
}

*/
int main(int argc, char **argv){




    //get stdin input
    char str[1000]; //buffer input 
    
    struct type row ; 
    row.value = str ;
    char *token ;
    token = strtok(row. value, ",") ;
    int i = 0;  
    while(fgets(str, 1000,stdin) != NULL) {
            
        if(i ==0) {  //first row read in, make catergory array 
    
        }
    
        else { //all data rows 
        

        }
    printf("%s\n", str);
    
           
        }


 //stores the input from the user as a string variable
    char *sort_type = argv[1]; //get argument to sort by, -c for column
    char *sort_topic = argv[2]; //get topic i.e. 'movies'
/*
    int num = 1;

    //get input line by line from stdin
    char *str_line = (char *)malloc(num*sizeof(char *));

    while(fgets(str_line, 1024, stdin) != NULL){
        printf("About to enter for loop\n\n");
        char *test = strParser(str_line);

    }

*/
    return  0;

}

