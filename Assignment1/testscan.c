#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h> // need strncmp/strlen for finding ".csv" files
#include <dirent.h> // allows for readdir(), opendir()

/* To test:
cd Desktop
cd <desired directory>
gcc -o <outfile> testscan.c
./<outfile>

Lists the files in current directory
and prints out if condition when .csv files are located
*/


// convert main function to void dirscanner(char *dir);
int main(int argc, char *argv[])
{
	DIR *dir;	// directory pointer
	struct dirent *sd;
	
	dir = opendir("."); //points to current directory
	
	// null case: failed to open directory
	if(dir == NULL) 
	{
		printf("Error: Directory N/A");
		exit(1);
	}
	
	// searches through current directory and prints files inside
	while( (sd=readdir(dir)) != NULL){
		int length = strlen(sd->d_name);
		printf(">> %s\n", sd->d_name);
		
		// if file's last 4 bytes == ".csv" then execute if statement
		if(strncmp(sd->d_name + length - 4,".csv",4) == 0){
			printf("\n CSV IS FOUND: %s\n\n", sd->d_name);
			printf("fork() here\n");
		}
	}
	closedir(dir);
	
}
