#ifndef SORTER_
#define SORTER_

//Defines our structs to store data about each row
typedef struct rec{
    char *field_data; //holds category data (i.g. "Titanic" for category "movie_title")
    char *original_row; //holds pointer to array of row.
} Record;





#endif