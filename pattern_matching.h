/**

  @file
  
  @brief implementation of pattern matching algorithm.
    
  */

#include "slist.h"

#ifndef PATTERN_MATCHING_H
#define PATTERN_MATCHING_H

typedef unsigned int pm_int_t;
typedef unsigned int _size_t;

#define PM_CHARACTERS 256



/* Represents a single labeled edge. */
struct pm_labeled_edge {
	unsigned char label;  //the character "on" this edge, is the arrow
	struct pm_state *state; //the destination state  "NEXT"
};
typedef struct pm_labeled_edge pm_labeled_edge_t;


/* Represents a single state (node). */
struct pm_state
{
	pm_int_t id; //S0 S1 S2
	pm_int_t depth; // what line is it
	dbllist_t* output;  //list of patterns //mallock list// char*
	struct pm_state * fail; //failure function, pointer to the state that we should go when there is no transition  
	dbllist_t* _transitions; //list of edges all the next passes  //regular Node
};
typedef struct pm_state pm_state_t;

/* this is what you should create when you search the string and detect a match */
struct pm_match
{
	char* pattern;  //the matched pattern
	int start_pos;  //where does it start in the text
	int end_pos; //where does it end in the text
	pm_state_t* fstate; //what is the final state
};
typedef struct pm_match pm_match_t;


/* the FSM */
struct pm
{
	pm_int_t newstate;  //the id of the next state (i.e. the number of states allocated so far in the FSM)
	pm_state_t *zerostate; //pointer to the root
};

typedef struct pm pm_t;


/* Initializes the fsm parameters (the fsm itself should be allocated).  Returns 0 on success, -1 on failure. 
*  this function should init zero state
*/
int pm_init(pm_t *);///////////////////////////////////////////////////////

/* Adds a new string to the fsm, given that the string is of length n. 
   Returns 0 on success, -1 on failure.*/
int pm_addstring(pm_t *,unsigned char *, _size_t n);

/* Finalizes construction by setting up the failrue transitions, as
   well as the goto transitions of the zerostate. 
   Returns 0 on success, -1 on failure.*/
int pm_makeFSM(pm_t *);


/* Set a transition arrow from this from_state, via a symbol, to a
   to_state. will be used in the pm_addstring and pm_makeFSM functions.
   Returns 0 on success, -1 on failure.*/   
int pm_goto_set(pm_state_t *from_state,unsigned char symbol,pm_state_t *to_state);

/* Returns the transition state.  If no such state exists, returns NULL. 
   will be used in pm_addstring, pm_makeFSM, pm_fsm_search, pm_destroy functions. */
pm_state_t* pm_goto_get(pm_state_t *state,unsigned char symbol);



/* Search for matches in a string of size n in the FSM. 
   if there are no matches return empty list */
dbllist_t* pm_fsm_search(pm_state_t *,unsigned char *,_size_t);



/* Destroys the fsm, deallocating memory. */
void pm_destroy(pm_t *);


#endif


