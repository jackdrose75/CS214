#ifndef SORTER_
#define SORTER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct{
    char *field_data;
    char *original_row; //holds pointer to array of row.
} Record;

//compares two floats like strcmp
int numcmp(float num1, float num2);
// merge function to be called after merge_sort
void merge(Record** a, int low, int mid, int high);

// merge_sort function that will recursively call merge and itself
void merge_sort(Record** a, int low, int high);

// Parses string for delimiters and returns pointer to the string
char *getCat(char *line, int catIndex);
// Builds string if it detects a ""
char *buildString(char *begin, char *end);
// Checks if item is a number
int isNum(char *str);

#endif