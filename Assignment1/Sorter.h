/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/
/* valueType is how each row is represented.  W create one struct to define each row, the function each row has
// the type of value for testing purposes is limited but can be expanded to fit larger files 
  a user defined input on what to sort the file by will be given in command line -> global char? thi way we
  always know what to sort 
  */ 


typedef struct valueType{//this struct is gonna hange once testing is done, corresponds to movie_metatest rn
    char * color ;
    int  * duration ;
    int * director factor ;
    char * generes ; 
    struct valueType *ptr ; // a pointer to be able to assing values as read, used when reading in data 
}
/*when comparing a struct element to sort by some things to test: 
//elments with same name/value,
repeated elements, elements not present */

/*next we need a list of all the different types of value we read from vsc
We can have an array(pointers to blocks) of structs of valueType.  this becomes our converted data to use
all elements in this list must be intialized when reading in file. 
*/
typdef struct valueList{
    struct valuetype *entry ; //my idea is a list is pointing to how many valueTypes we have
    int size ; // useful for knowing size of list? maybe global variable instead, 
}

