#include<stdio.h>
#include<stdlib.h>

typedef struct type{
 
	char* value ;
	struct type *next ; //pointer to next column	
};

void mergesort(char *a, int size){
	//initialize mid, assign its value
	int mid = 0;
	mid = size/2 ; 
	// array into two, dynamically 
	char *top, *bottom ;

	//conditions
	if(size = 2) {
		return; 
	}
	else{ //create two top/bottom arrays
		top = (char *)malloc(sizeof(int)*mid);
	}

}

char *parse(char *row){

	return row ; 
}
int main(int argc, char **argv){


{

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





    return  0;

}

