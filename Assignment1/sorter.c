#include<stdio.h>
#include<stdlib.h>


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


int main(int argc, char* arg[]){
        
        FILE *fp;
        fp = stdin;
        char temp[1000];
        fgets(temp, 1000, fp);
        printf("%1000[^,]\n", temp);

        return 0;
}