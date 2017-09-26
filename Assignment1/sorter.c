#include<stdio.h>
#include<stdlib.h>
#include<string.h> 


/*
plan:   read the first 'header file' for value types each entry will follow,
        creat the list of all stuc valueTypes into valueList
        once all values are in list, parse through by sorting
things to remember:     memory allocation - dont free always initalize 
                        allocate valuType as needed, dont creat list with predefined size(user determines)
                        
useful methods:     mergsort, fastest way to sort large datasets 
                    finding if valueType exists
                    printinf the list
                    debugging tools(flags, error checking etc)
*/

	struct type{

	char* value ;
}; 

int main(int argc, char* arg[]){
        
        FILE *fp;
        fp = stdin;
  	char temp[40] ;
        fgets(temp,40, fp);
       // printf("%s\n", temp);
		 
	char *ptr = temp;
	struct type entry ;
	entry.value = temp ; 
	printf("%s\n",temp);
	printf("%s\n", ptr) ;
	printf("%s\n", entry.value) ; 	

	
	char *tokenp ;
	tokenp = strtok(entry.value, ",") ; 
	char entries[2] ; 
	int i = 0 ;
	while(tokenp != NULL){
	printf("%s  ", tokenp); 
	tokenp = strtok(NULL, ",") ;
	i++ ;
	
	printf("%d\n", i) ; 
	}	


 	return  0;
}
