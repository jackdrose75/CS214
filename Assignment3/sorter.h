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
#include <pthread.h> //for threading
#include <semaphore.h> //for semaphores


typedef struct{
    char *field_data;
    char *original_row; //holds pointer to array of row.
} Record;

//struct to hold input parameters
typedef struct{
    char *sortTopic;
    char *currPath;
    char *outputDir;
} InputParams;

//sorter struct
typedef struct{
    char *filename;
    char *path; //path of just the directory
    InputParams *inputDetails;
} SortParams;

//Node 
typedef struct node{
    char *path; //current path
    char *filename; //current filename if csv
    pthread_t p_tid; //parent thread tid
    pthread_t c_tid; //child thread tid
    struct node *next;
    InputParams * input;
    SortParams *sortInput;  
} Node; 

typedef struct stack_node{
    Record ** csvData; 
    struct stack_node * next;
}stackNode; 



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
//void sorter(char *filename, char *path, InputParams *inputDetails);
void *sorter(void*sortDetails);

//used to debug traverseDir.c 
void * fake_sorter(void* sortDetails); 

// Parses string for delimiters and returns pointer to the string
char *getCat(char *line, int catIndex);
// Builds string if it detects a ""
char *buildString(char *begin, char *end);
// Checks if item is a number
int isNum(char *str);

// Directory forking
int checkcsv (char* name); 

char *createPath (char* orig, char *new);

void* traverseDir(void* inDetails);

void printParam(InputParams * params); 

void printSorter(SortParams * params); 

//Linked list functions
Node * init(Node *next, char*filename, char *path, pthread_t p_tid, pthread_t c_tid,InputParams * input,SortParams* sortInput);
//Node * init(Node *next, char*filename, char *path, pthread_t p_tid, pthread_t c_tid);
//add to end
Node* append(Node *add,Node *head);
//returns total number of Nodes in list
int count(Node *head);
void print(Node *head);
Node *removeFront (Node *head); 


//Stack functions
void push(Record ** csvData,int index); 
void printLL (Node* head); 








#endif
