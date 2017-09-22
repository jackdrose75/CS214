#include<stdio.h>
#include<stdlib.h>


/*
plan:	read the first 'header file' for value types each entry will follow,
		creat the list of all stuc valueTypes into valueList
		once all values are in list, parse through by sorting
things to remember:		memory allocation - dont free always initalize 
						allocate valuType as needed, dont creat list with predefined size(user determines)
						
useful methods:		mergsort, fastest way to sort large datasets 
					finding if valueType exists
					printinf the list
					debugging tools(flags, error checking etc)
*/


int main(int argc, char* arg[]){
		FILE *file ; 
        printf("This is a value out %d\n", argc);
        printf("This is the first character %s\n", arg[0]);
        printf("This is the second character %s\n", arg[1]);
        printf("This is the third character %s\n", arg[2]);
        printf("This is the fourth character %s\n", arg[3]);

        return 0;
}