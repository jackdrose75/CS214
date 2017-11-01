#include "sorter.h"

// Mergesort functions to sort structs

//check if num
//returns 1 if number
//returns 0 if not number
int isNum(char *str){
    int dotCount = 0;
    while(*str) {
        if(!isdigit(*str)){
            if (*str == '.'){
                dotCount++;
                if (dotCount > 1){
                    return 0;
                }
            } else {
                return 0;
            }
        }
        str++;
    }
    return 1;
}

//return 1 if num1 greater than num2
//return 0 if num1 == num2
//return -1 if num1 < num2
int numcmp(float num1, float num2){
    if (num1 > num2) {
        return 1;
    } else if (num1 == num2) {
        return 0;
    } else {
        return -1; //num1 < num2
    }
}

void merge(Record** a, int low, int mid, int high){

    int topL = mid - low + 1; 
    int bottomL = high - mid;
    int i, j, k;
    
    Record** top = (Record **)malloc(sizeof(Record **)*topL);
    Record** bottom = (Record **)malloc(sizeof(Record **)*bottomL);

    // printf("high : %d\n", high);
    // printf("mid : %d\n", mid);
    // printf("low : %d\n", low);

    // printf("++++++");
    // printf("BEGIN\n");
    // int blah;
    // for(blah = 0; blah < high; blah++) {
    //     printf("%s ", a[blah] -> field_data);
    // }
    // printf("\n");
    // printf("++++++");

    for(i = 0; i < topL; i++){
        //top[i] = malloc(sizeof(a[low+1]));
        top[i] = malloc(sizeof(Record));
        top[i] = a[low + i];
        // printf("a[low %d] : %s\n", low,  a[low]->field_data);
        //printf("top updated w/ a: %s\n", top[i]->field_data);
        // strcpy(top[i], a[low+i]);
    }
    for(j = 0; j < bottomL; j++){
        //bottom[i] = malloc(sizeof(a[mid+i+1])); 
        bottom[j] = malloc(sizeof(Record));
        bottom[j] = a[mid+1+j];
        // printf("bottom updateed w/ a: %s\n", bottom[j]->field_data);
        // strcpy(bottom[i], a[mid+i+1]);
    }
    
    // merge temps back into records
    i = 0; // starting index of first subarray
    j = 0; // starting index of second subarray
    k = low; // starting index of merged subarray
    
    while(i < topL && j < bottomL) {
        /*printf("top[%d]->field_data : %s\n", i, top[i]->field_data);
        printf("bottom[%d]->field_data : %s\n", i, bottom[j]->field_data);*/

        //compare nums if num
        // printf("top[%d]->field_data : %s\n", i, top[i]->field_data);
        // printf("bottom[%d]->field_data : %s\n", j, bottom[j]->field_data);
        // printf("isNum(top[%d]->field_data) : %d : %s\n", i, isNum(top[i]->field_data), top[i]->field_data);
        // printf("isNum(bottom[%d]->field_data) : %d : %s\n", j, isNum(bottom[j]->field_data), bottom[j]->field_data);

        if(isNum(top[i]->field_data) && isNum(bottom[j]->field_data)){
            float num1 = atof(top[i]->field_data); //save as floats
            float num2 = atof(bottom[j]->field_data);
            
            // printf("numcmp(%f, %f) : %d\n", num1, num2, numcmp(num1,num2));
            // printf("num1 : %f :: num2 : %f\n", num1, num2);
            if(numcmp(num1, num2) < 0){
                // printf("num1 <= num2 : %f <= %f\n\n", num1, num2);
                a[k] = top[i];
                i++;
            }
            else {
                // printf("num1 > num2 : %f > %f\n\n", num1, num2);
                a[k] = bottom[j];
                j++;
            }
        }

        //compare strings if string
        else { 
            // printf(">>>>>>I'm comparing strings when i'm not supposed to\n");
            // printf("top[%d]->field_data : %s\n", i, top[i]->field_data);
            // printf("bottom[%d]->field_data : %s\n\n", j, bottom[j]->field_data);

            if(strcmp(top[i] -> field_data, bottom[j] -> field_data) < 0){
                a[k] = top[i++];
                //strcpy(a[k++] -> field_data, top[i++] -> field_data) ;
            }
            else {
                a[k] = bottom[j++];
                //strcpy(a[k++] -> field_data, bottom[j++] -> field_data);
            }
        }
        k++;
    }

    //add remaining top
    while (i < topL){
        a[k] = top[i];
        i++;
        k++;
    }
    //add remaining bottom
    while (j < bottomL){
        a[k] = bottom[j];
        j++;
        k++;
    }


/*
    printf("++++++");
    for(i = 0; i < k; i++) {
        printf("%s ", a[i] -> field_data);
    }
    printf("\n");
    printf("++++++");
*/

    free(top);
    free(bottom);
}

void merge_sort(Record** a, int low, int high){
    
    if (low < high){
        int mid = (low + high)/2;
        merge_sort(a, low, mid);
        merge_sort(a, mid+1, high);
        merge(a, low, mid, high);
    }


}
