#include <stdio.h>
#include <stdlib.h>
#include "sorter.h"


//create node
Node *init(Node *next,char *filename, char *path, pthread_t p_tid, pthread_t c_tid,InputParams* input,SortParams* sortInput){
	Node * curr = (Node*) malloc(sizeof(Node));
	curr->input = input; 
	curr->sortInput = sortInput; 
	curr->next = next;
    	curr->path = path;
    	curr->filename = filename;
    	curr->p_tid = p_tid;
    	curr->c_tid = c_tid; 
	return curr; 
}


//add to end of list
Node *append(Node *add,Node *head){
	Node *curr = head;  

	//get last node
	while(curr->next != NULL){
		curr = curr->next;
	}
	curr->next = add; 
	add->next = NULL; 
	return head;	 
}


Node *removeFront (Node *head){
	Node * newHead = head->next;
	free(head->filename); 
	free(head->path); 
	free(head); 
	return newHead;
	




}
/*
//append to front of list for faster retrieval of most recent
Node *append(Node *new, Node *head){
    new->next = head;
    head = new; 
    return head;     
}
*/

//get list count

int count(Node *head){
	if (head ==NULL){
		return 0; 
	}
	int count = 1; 
	Node * curr = head;
	while(curr->next != NULL){
		//might need lock? 
		count++; 
		curr = curr->next;
	}

	return count;

}

//print list
void print(Node*head){
	Node *curr = head; 
	while(curr!=NULL){
		if (curr == head){
			printf("HEAD\n"); 
		}
        	printf("path:%s\n", curr->path);
        	printf("filename:%s\n",curr->filename); 
		printf("p_tid:%u\n",curr->p_tid); 
		printf("c_tid:%u\n",curr->c_tid);
		if (curr->next){
		    printf("next path:%s\n", (curr->next)->path); 
		} else {
		    printf("next path: NULL\n");
		}
		fflush(stdout); 
		curr = curr->next;
	}

}

/*int main (int argc,char** argv){

	//Node * head = (Node*)malloc(sizeof(Node));
	//head->p_tid = 500; 
	//head->next = NULL; 
	Node *head = init(NULL,23,43); 
	pid_t tid2 = 758;
	pid_t tid3 = 400; 
	Node * two = init(NULL,tid2,tid3); 
	Node * three = init(NULL,tid2,tid3); 
	head = append(two,head);
	head = append(three,head); 
	
	int total = count(head); 
	printf("count is:%d\n", total); 
	print(head); 


	return 0; 
}*/
