//By: Yahya Saad ID: 322944869
//Ex1: pattern_matching.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pattern_matching.h"
#define SUCCESS 0
#define ERROR -1
void *dbllist_delete_head(dbllist_t *,dbllist_destroy_t);//delete first node' depends on the flag mode
int dbllist_connect(dbllist_t*, dbllist_t*);//connect 2 lists

//implementation of pattern matching algorithm using Deterministic Finite Automaton:
// for every p and q where p is the current state and q is the next state!
//initialize DFA
int pm_init(pm_t* finite)
{
    if(finite == NULL)
        return ERROR;
    //init the zero state
    finite->zerostate = (pm_state_t*)malloc(sizeof(pm_state_t)+1);
    if(finite->zerostate == NULL)
        return ERROR;
    finite->zerostate->id = 0;
    finite->zerostate->depth = 0;
    finite->newstate = 1;
    //init output list
    finite->zerostate->output = (dbllist_t*)malloc(sizeof(dbllist_t)+1);
    if(finite->zerostate->output == NULL)
        return ERROR;
    dbllist_init(finite->zerostate->output);
    finite->zerostate->fail = NULL;
    //init transitions list
    finite->zerostate->_transitions = (dbllist_t*)malloc(sizeof(dbllist_t)+1);
    if(finite->zerostate->_transitions == NULL)
        return ERROR;
    dbllist_init(finite->zerostate->_transitions);
    return SUCCESS;
}
//get the other state p -> q
pm_state_t* pm_goto_get(pm_state_t *state,unsigned char str)
{
    if(state==NULL)
        return NULL;
    dbllist_node_t *node=dbllist_head(state->_transitions);
    pm_labeled_edge_t* edge;
    while(node!=NULL)
    {
        edge=(pm_labeled_edge_t*)dbllist_data(node);
        if(edge->label==str)
        {
            return edge->state;
        }
        node=dbllist_next(node);
    }
    return NULL;
}
// set new edge from source to destination with given symbol
int pm_goto_set(pm_state_t *src, unsigned char str, pm_state_t *dest){
    pm_labeled_edge_t* edge= (pm_labeled_edge_t*) malloc(sizeof(pm_labeled_edge_t));
    if(!edge || !src || !dest)
        return ERROR;
    //set for label and state: the destination and the value!
    edge->label = str;
    edge->state = dest;
    printf("%d ->%c-> %d\n", src->id, str, dest->id);
    if(dbllist_append(src->_transitions, edge)== -1){
        free(edge);
        return ERROR;
    }
    return SUCCESS;
}

//Adds a new string to the finite state machine
int pm_addstring(pm_t * pm,unsigned char * str, _size_t n){

    if(strlen(str)!= n || pm==NULL){return ERROR;}
    if (pm->newstate > PM_CHARACTERS){return ERROR;}
    int i = 0;
    pm_state_t* curr= pm->zerostate;
    // transition from current with an exist symbol
    while(pm_goto_get(curr, str[i]) && i<n){
        curr= pm_goto_get(curr, str[i]);
        i++;
    }//check all
    while(i<n){
        //init the new state if it doesn't exist
        pm_state_t* state= (pm_state_t*)malloc(sizeof(pm_state_t));
        if(!state)
            return ERROR;
        state->id= pm->newstate;
        printf("Allocating state %d\n", state->id);//allocate the new state
        state->depth= (curr->depth)+1;
        state->fail= NULL;
        state->output= (dbllist_t*)malloc(sizeof(dbllist_t));
        if(!state->output){
            free(state);
            return ERROR;
        }
        dbllist_init(state->output);
        state->_transitions=(dbllist_t*) malloc(sizeof(dbllist_t));
        if(!state->_transitions){
            free(state->output);
            free(state);
            return ERROR;
        }
        //give connect edge from the current state to the new one
        dbllist_init(state->_transitions);
        pm_goto_set(curr, str[i], state);
        pm->newstate= pm->newstate +1;
        curr= state;
        i++;
    }
    // append given string to state's output list
    if(dbllist_append(curr->output, str)== -1){return ERROR;}
    return SUCCESS;
}
//make fail state to DFA
int pm_makeFSM(pm_t *pm){
    dbllist_t* queue= (dbllist_t*)malloc(sizeof(dbllist_t));//from the list to a queue
    if(!queue || !pm)
        return ERROR;
    dbllist_init(queue);
    dbllist_node_t* node= dbllist_head(pm->zerostate->_transitions); // tmp node
    pm_state_t* current= pm->zerostate;//point to the zero state
    pm_state_t* q= NULL;//next state
    pm_state_t* p= NULL;// current state
    // set failure of states with depth 1 to zero state and append them to queue
    while(node){
        q= pm_goto_get(current, *(char*) dbllist_data(node));
        if(q != NULL){ // check next state
            if(dbllist_append(queue, q) == -1){
                free(queue);
                return ERROR;//create node was unsuccessful
            }
            q->fail= pm->zerostate;
        }
        node= dbllist_next(node); //iterator
    }
    // while queue !empty pop current(failure already set) and set next_state's failure
    current=dbllist_delete_head(queue, DBLLIST_LEAVE_DATA);//save the data to current and delete first element in the queue
    while( current != NULL ){
        node= dbllist_head(current->_transitions);
        while(node != NULL){//update all zero state's son's fail state to be zero state.
            q= pm_goto_get(current, *(char*)dbllist_data(node));
            if(q != NULL){
                if(dbllist_append(queue, q) == -1){
                    free(queue);
                    return ERROR;
                }
                p= current->fail;
                // loop until transition is found or NULL state is reached
                while(p!=NULL && pm_goto_get(p, *(char*)dbllist_data(node)) == NULL){
                    p= p->fail;
                }
                //prent the format for failed states
                printf("setting f(%d) = ", q->id);
                //current state is NULL which leads to fail transition - returning to the zero state
                if(p== NULL){
                    q->fail= pm->zerostate;
                    printf("%d\n", q->fail->id);
                }

                else{//transition found
                    q->fail= pm_goto_get(p, *(char*)dbllist_data(node));
                    printf("%d\n",q->fail->id);
                }
            }
              //list Union
            if(dbllist_connect(q->output, q->fail->output)== -1){
                return ERROR;
            }
            node= dbllist_next(node);
        }
        current=dbllist_delete_head(queue, DBLLIST_LEAVE_DATA);;
    }
    dbllist_destroy(queue, DBLLIST_LEAVE_DATA);
    free(queue);
    return SUCCESS;
}
dbllist_t* pm_fsm_search(pm_state_t *state,unsigned char *str,_size_t n)
{
    if(state==NULL || str==NULL)//state = zero state at first
        return NULL;
    int i;
    int j;
    //int h;
    pm_state_t *dest;//destination
    dbllist_t* match=(dbllist_t*)malloc(sizeof(dbllist_t));//create match list
    if(match==NULL)
        return NULL;//create node was unsuccessful
    dbllist_init(match);//start new double list
    for(i=0;i<n;i++)
    {
        dest=pm_goto_get(state,str[i]);
        pm_state_t* lose=state;

        while(dest==NULL)//str didn't exist
        {
            if(lose->depth==0)
            {

                dest=lose;
                break;
            }
            lose=lose->fail;
            if(lose==NULL)
            {
                return NULL;
                dbllist_destroy(match,DBLLIST_FREE_DATA);

            }
            dest= pm_goto_get(lose,str[i]);
        }
        if(dbllist_head(dest->output)!=NULL)
        {
            dbllist_node_t *node=dbllist_head(dest->output);
            for(j=0;j<dbllist_size(dest->output); j++)
            {

                pm_match_t* check_match=(pm_match_t*)malloc(sizeof(pm_match_t));
                if(check_match==NULL)
                    return NULL;
                check_match->pattern=dbllist_data(node);
                check_match->start_pos=i-strlen(check_match->pattern)+1;
                check_match->end_pos=i;
                check_match->fstate=dest;
                dbllist_append(match,check_match);
                node=dbllist_next(node);
            }
        }
        state=dest;
    }
    return match;
}

void pm_destroy(pm_t *finite){
    dbllist_t* queue= (dbllist_t*)malloc(sizeof(dbllist_t));
    if(!queue || !finite){return;}
    dbllist_init(queue);
    dbllist_node_t* node= dbllist_head(finite->zerostate->_transitions);
    pm_state_t* p= finite->zerostate;
    pm_state_t* q= NULL;
    // append each state's transitions to queue then free current state
    while( p != NULL ){
        node= dbllist_head(p->_transitions);
        while(node != NULL){
            q= pm_goto_get(p, *(char*)dbllist_data(node));
            if(q != NULL){
                if(dbllist_append(queue, q) == -1){
                    free(queue);
                    return;
                }
            }
            node= dbllist_next(node);
        }
        dbllist_destroy(p->output, DBLLIST_LEAVE_DATA);
        free(p->output);
        dbllist_destroy(p->_transitions, DBLLIST_FREE_DATA);
        free(p->_transitions);
        free(p);
        p= dbllist_delete_head(queue,DBLLIST_LEAVE_DATA);
    }
    dbllist_destroy(queue, DBLLIST_FREE_DATA);
    free(queue);
}

//private func: get head of the list without deleting the data
void *dbllist_delete_head(dbllist_t *dbllist,dbllist_destroy_t destroy){
    if(dbllist== NULL || dbllist_head(dbllist)==NULL)
        return NULL;
    dbllist_node_t *node= dbllist_head(dbllist);
    dbllist_head(dbllist)= dbllist_next(node);
    if(dbllist_remove(dbllist, node, DBLLIST_LEAVE_DATA)==SUCCESS){
        void* data= dbllist_data(node);
        free(node);
        dbllist_size(dbllist)--;
        if(destroy == DBLLIST_LEAVE_DATA){
            return data;
        }
    }
}
//private func: connect two lists
int dbllist_connect(dbllist_t* first, dbllist_t* second){
    int i;
    int size=dbllist_size(second);
    dbllist_node_t *curr=dbllist_head(second);
    if((first==NULL)||(second==NULL))
        return ERROR;
    for(i=0;i<size;i++)
    {
        if(dbllist_append(first,dbllist_data(curr))==-1)//if addition was unsuccessful
            return ERROR;
        curr=dbllist_next(curr);
    }
    dbllist_destroy(first, DBLLIST_LEAVE_DATA);
    return SUCCESS;
}
