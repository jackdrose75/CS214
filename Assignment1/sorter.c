#include "sorter.h"

// Main file to parse and pass data

//Returns category substring
//parses through input string by looking for delimiters
//pStart marks beginning of category word, pEnd marks end by delimiter
//finds out length of substring
//finds length of substring by substracting pEnd and pStart
//malloc space for new char array and copy characters to it 
char *getCat(char *line, int catIndex){

    char *pStart = line; //keeps track of beginning of substring
    char *pEnd = line; //continues until finds substring index

    int strIndex = 0;

    bool inQuote = false;

    while(*pEnd){
        switch(*pEnd){
            case ',':
                if(inQuote == false){
                    //printf("pStart : %s\n", pStart);
                    if(strIndex == catIndex){
                        //create and return substring
                        //char *str = buildString(pStart, pEnd);
                        //printf("%s\n", str);
                        return buildString(pStart, pEnd);
                    } 
                    
                    //if strIndex != catindex, move pStart to pEnd
                    pEnd++;
                    pStart = pEnd;
                    strIndex++; //update strIndex
                } else {
                    pEnd++;
                }
                break;

            case '\"':
                //printf(">>>%c >> %ld \n", *pEnd, (pEnd-pStart));
                if (!inQuote){
                    inQuote = !inQuote;
                    pEnd++;
                    pStart = pEnd;
                } else {
                    inQuote = !inQuote;
                    if (strIndex++ == catIndex){
                        pEnd--;
                        return buildString(pStart, pEnd);
                    }
                    pEnd = pEnd+2;
                    pStart = pEnd;
                }
            case '\r':
                if(*(pEnd++)){
                    if(*pEnd == '\n'){
                        pEnd--;
                        return buildString(pStart, pEnd);
                    } else {
                        pEnd++;
                        break;
                    }
                }
                pEnd--;
                return buildString(pStart, pEnd);
                
            default:
                pEnd++;
        }
    }
    return '\0';
}

char *buildString(char *start, char *end){

    int strLen = end - start;
    char *strBuffer = (char *)malloc((strLen+1)*sizeof(char *));
    //copies chars from one string to another for strLen
    memcpy(strBuffer, start, strLen);
    strBuffer[strLen] = '\0';
    return strBuffer;
}

void sorter(FILE *input, int argc, char **argv){

    //read in file
    input = fopen(argv[1], "r");
    if (NULL == input) {
        fprintf(stderr, "Unable to open file\n");
        exit(EXIT_FAILURE);
    } else {
        input = stdin;
    }


    const int MAXSIZE = 1024;

    //stores the input from the user as a string variable
    char *sortType = argv[1]; //get argument to sort by, -c for column
    char *sortTopic = argv[2]; //get topic i.e. 'movies'

    //get categories inputted by use
    //match by index number of column
    char first_row[MAXSIZE];// = (char *)malloc(MAXSIZE);
    fgets(first_row, MAXSIZE, stdin); //get first row
    char *_first_row = strdup(first_row); //tmp to keep track for strsep

    char *cat; //pulled category from strsep
    int cat_index = -1; //index where category is
    int int_index = 0; //index for total size of line
    while((cat = strsep(&_first_row, ",")) != NULL){
        if(strstr(cat, sortTopic)){
            cat_index = int_index;
        }
        int_index++;
    }

    //check if valid input
    if (cat_index == -1){
        printf("%s is not a valid input.\n", sortTopic);
        exit(0);
    }

    //initialize array of structs
    Record **recordList = (Record **)malloc(sizeof(Record **));

    //get input line by line from stdin
    int index = 0;
    char row[MAXSIZE];
    //char *row;

    while(fgets(row, MAXSIZE, stdin) != NULL){

        //strtok(row, "\n");
        char *_row = strdup(row);

        //temporary struct to hold parsed data
        Record *tmpList;
        tmpList = (Record *)malloc(sizeof(Record *));



        //parse string for category field_data (i.e. director_name => James Cameron)
        char *tmp = getCat(strdup(_row), cat_index); //send _row and cat index into getCat to be parsed, extract pulled field_data
        
        //save category into tmpList.field_data
        tmpList -> field_data = tmp;
        // printf("tmpList -> field_data: %s\n", tmpList->field_data);

        //save original string row to tmpList.original_row
        tmpList -> original_row = _row;
        // printf("tmpList -> original_row : %s\n", tmpList->original_row);

        //add tmpList struct to array of structs recordList[index] at index
        recordList = (Record **)realloc(recordList, (index+1)*sizeof(Record **));
        recordList[index] = (Record *)malloc(sizeof(Record *)); 
        recordList[index] = tmpList;

        index++;
    }

    //merge sort recordLists by field_data
    merge_sort(recordList, 0, index-1);

    //output
    int i;
    printf("%s", first_row);
    for(i = 0; i < index; i++) {
        printf("%s", recordList[i] -> original_row);
    }

    //printf("Here\n");
    free(recordList);

}


int main(int argc, char **argv){

    FILE *input;
    sorter(input, argc, argv);
    return 0;

}




