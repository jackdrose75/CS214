#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include "sorter.h"

// Mergesort functions to sort structs


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
    
    while(i < topL && j < bottomL){
        /*printf("top[%d]->field_data : %s\n", i, top[i]->field_data);
        printf("bottom[%d]->field_data : %s\n", i, bottom[j]->field_data);*/

        // if top[i] -> type == 0 and bottom[i] -> type == 0:
        //  (int *)top[i] -> field_data < bottom[i] -> field_data
        // else if top[i] -> type == 1 and bottom[i] -> type == 1:
        //  (float *)
        // else if top[i] -> type == 2 and bottom[i] -> type == 2:
        //  (char *)
        // else
        //      some fallback for invalid input




        if(strcmp(top[i] -> field_data, bottom[j] -> field_data) < 0){
            a[k] = top[i++];
            //strcpy(a[k++] -> field_data, top[i++] -> field_data) ;
        }
        else{
            a[k] = bottom[j++];
            //strcpy(a[k++] -> field_data, bottom[j++] -> field_data);
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
