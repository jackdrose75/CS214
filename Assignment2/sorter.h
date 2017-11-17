#ifndef SORTER_
#define SORTER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h> // allows for readdir(), opendir()
#include <errno.h>
#include <unistd.h>


typedef struct{
    char *field_data;
    char *original_row; //holds pointer to array of row.
} Record;

//struct to hold input parameters
typedef struct{
    char *sortTopic;
    char *inputDir;
    char *outputDir;
} InputParams;

// mergesort
//compares two floats like strcmp
int numcmp(float num1, float num2);

int stringcmp (char * a, char * b);
// merge function to be called after merge_sort
void merge(Record** a, int low, int mid, int high);
// merge_sort function that will recursively call merge and itself
void merge_sort(Record** a, int low, int high);

// csv sorter
// run sorter on csv
void sorter(char *filename, char *path, char *subpath, InputParams* inputDetails);
// Parses string for delimiters and returns pointer to the string
char *getCat(char *line, int catIndex);
// Builds string if it detects a ""
char *buildString(char *begin, char *end);
// Checks if item is a number
int isNum(char *str);

// Directory forking
void addArray(pid_t p);

char* pathcat(const char* str1,const char* str2);

void pcounter(char* path);

void dirSearch(char *path, InputParams* inputDetails);



#endif