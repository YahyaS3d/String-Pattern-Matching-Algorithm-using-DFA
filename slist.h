/**

  @file
  
  @brief Implementation of a very simple double linked list.
  
*/

#ifndef DBLLIST_H
#define DBLLIST_H

//! The definition of a double linked list node
struct dbllist_node
{
	void *data; // Pointer to data of this node
	struct dbllist_node *next; // Pointer to next node on list
	struct dbllist_node *prev; // Pointer to previous node on list
};

//! double linked list node type
typedef struct dbllist_node dbllist_node_t;

//! The definition of a double linked list
struct dbllist
{
	dbllist_node_t *head; // Pointer to head of list
	dbllist_node_t *tail; // Pointer to tail of list
	unsigned int size; // The number of elements in the list
};

//! double linked list type
typedef struct dbllist dbllist_t;


// you have to use these macros, do not use the inner variables of the list!!
//! Macro to get the head node of a list l
#define dbllist_head(l) l->head
//! Macro to get the tail node of a list l
#define dbllist_tail(l) l->tail
//! Macro to get the size of a list l
#define dbllist_size(l) l->size
//! Macro to get the next node of l
#define dbllist_next(n) n->next
//! Macro to get the prev node of l
#define dbllist_prev(n) n->prev
//! Macro to get the data of node l
#define dbllist_data(n) n->data

//! Specifies whether dbllist_destroy should deallocate or not stored elements
typedef enum { DBLLIST_LEAVE_DATA = 0, DBLLIST_FREE_DATA } dbllist_destroy_t;

/** Initialize a double linked list
	\param list - the list to initialize */
void dbllist_init(dbllist_t *);

/** Destroy and de-allocate the memory hold by a list
	\param list - a pointer to an existing list
	\param dealloc flag that indicates whether stored data should also be de-allocated */
void dbllist_destroy(dbllist_t *,dbllist_destroy_t);


/** Append data to list (add as last node of the list)
	\param list - a pointer to a list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure */
int dbllist_append(dbllist_t *,void *);

/** Prepend data to list (add as first node of the list)
	\param list - a pointer to list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure
*/
int dbllist_prepend(dbllist_t *,void *);

/** \brief Remove the specific node from the list. 
	\param to a pointer to the list
	\param pointer to the node that should be removed.
	\param dealloc flag that indicates whether to de-allocated the data in the node 
	\return 0 on success, or -1 on failure
*/

int dbllist_remove(dbllist_t *, dbllist_node_t* ,dbllist_destroy_t);
#endif
