#ifndef SORTER_
#define SORTER_

//Defines our structs to store data about each row
/*typedef struct{
    typedef enum {POINTER, INT} DataType;
    union{
        int i_data;
        char *c_data;
    } field_data;
    char *original_row; //holds pointer to array of row.
} Record; */

typedef struct{
    void *field_data;
    // type = 0 --> int
    // type = 1 --> float
    // type = 2 --> string
    int type;
    char *original_row; //holds pointer to array of row.
} Record;

// merge function to be called after merge_sort
void merge(Record** a, int low, int mid, int high);

// merge_sort function that will recursively call merge and itself
void merge_sort(Record** a, int low, int high);

// Parses string for delimiters and returns pointer to the string
void *getCat(char *line, int catIndex);

// Builds string if it detects a ""
void *buildString(char *start, char *end, bool isInt, bool isFloat);


#endif