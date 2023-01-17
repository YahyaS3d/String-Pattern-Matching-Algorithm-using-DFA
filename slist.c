//Yahya Saad
//slist.c
#include <stdio.h>
#include <stdlib.h>
#include "slist.h"
#define SUCCESS 0
#define ERROR -1
//start a double linked list
void dbllist_init(dbllist_t *dbllist)
{
    if(dbllist == NULL){return;}
    //init size, head and tail
    dbllist_size(dbllist)= 0;
    dbllist_head(dbllist)= NULL;
    dbllist_tail(dbllist)= NULL;
}
//delete list
void dbllist_destroy(dbllist_t *dbllist,dbllist_destroy_t destroy)
{
    if(dbllist==NULL || dbllist_head(dbllist)==NULL)//list doesn't exist
        return;
    dbllist_node_t* tmp= dbllist_head(dbllist);
    while(tmp != NULL)
    {
        if(destroy==DBLLIST_FREE_DATA)//check flag
            free(dbllist_data(tmp));
        tmp=dbllist_next(dbllist_head(dbllist));
        dbllist_prev(dbllist_head(dbllist))= NULL;
        free(dbllist_head(dbllist));
        dbllist_head(dbllist)=tmp;
        dbllist_size(dbllist)=dbllist_size(dbllist)-1;
    }
    dbllist_init(dbllist);//empty the list again
}
//add to the end -> as a new node after the tail
int dbllist_append(dbllist_t *dbllist,void *data)
{
    dbllist_node_t *node= (dbllist_node_t*) malloc(sizeof(dbllist_node_t));
    if(!node || !dbllist || !data)
        return ERROR;
    dbllist_data(node)= data;
    dbllist_next(node)= NULL;
    dbllist_prev(node) = NULL;
    if(dbllist_size(dbllist)==0){
        dbllist_head(dbllist)= node;
        dbllist_tail(dbllist)=dbllist_head(dbllist);
        dbllist_prev(node) = dbllist_next(node);
    }else{
        dbllist_prev(node) = dbllist_tail(dbllist);
        dbllist_next(dbllist_tail(dbllist))= node;
        dbllist_tail(dbllist)= node;
    }
    dbllist_size(dbllist)++; //inc list size by 1, adding a new node in any case
    return SUCCESS;
}
//add to the begun -> as a new head for the list
int dbllist_prepend(dbllist_t *dbllist,void *data)
{
    dbllist_node_t *node = (dbllist_node_t *) malloc(sizeof(dbllist_node_t));
    if(!node || !dbllist || !data) {
        return ERROR;
    }
        dbllist_data(node) = data;
    if (dbllist_size(dbllist) == 0) {
        dbllist_head(dbllist) = node;
        dbllist_tail(dbllist) = node;
    } else {
        dbllist_prev(dbllist_head(dbllist)) = node;
        dbllist_next(node) = dbllist_head(dbllist);
        dbllist_head(dbllist) = node;
    }
    dbllist_size(dbllist)++;
    return SUCCESS;
}
//remove a specific node with giving node
int dbllist_remove(dbllist_t *dbllist, dbllist_node_t *node ,dbllist_destroy_t destroy){
    //base case
    if (dbllist == NULL || node == NULL) {
        return ERROR;
    }
    dbllist_node_t* tmp= dbllist_head(dbllist);
    while(tmp != NULL) {
        if (tmp == node) {
            //------- check both prev and next -------
            if(dbllist_prev(tmp)){
                dbllist_next(dbllist_prev(tmp)) = dbllist_next(tmp);
            }
            if(dbllist_next(tmp)){
                dbllist_prev(dbllist_next(tmp)) = dbllist_prev(tmp);
            }
            //------- check both head and tail -------
            if(tmp == dbllist_head(dbllist)){
                dbllist_head(dbllist) = dbllist_next(tmp);
            }
            if(tmp == dbllist_tail(dbllist)){
                dbllist_tail(dbllist) = dbllist_prev(tmp);
            }
            //------- check flag -------
            if(destroy==DBLLIST_FREE_DATA){
                free(dbllist_data(tmp));
            }
            // free and resize the list after delete
            free(tmp);
            dbllist_size(dbllist)--;
            break;
        }
        tmp = dbllist_next(tmp); // check the next node
    }

    return SUCCESS;
}
