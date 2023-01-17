
CREATED BY: Yahya Saad
EX1 – String Pattern Matching Algorithm
==Files==
slist.c
This file present a implementation of a double linked list. The methods of linked list which used to apply for the finite-state machine that composed of set of spesific words.

pattern-matching.c
This file contains matches multiple patterns simultaneously, by first constructing DFA and use it to representing the patterns set, and then, with this DFA on its disposal, processing the text in a single pass:locates elements of a finite set of strings and uses slist.c to implement the usage of the definition of Deterministic Finite Automaton.

README.txt 
This file; more info about pattern matching algorithm that has been used in different way.

==General information== 
- every memory allocation that used in main must be freed also in main- to free all allcoted states.
- 2 private methods that used in pattern-matching.c: 
1- void *dbllist_delete_head(dbllist_t *,dbllist_destroy_t) : deleteing first elemnte in double linkedlist saving it data in order to use it later.
2- int dbllist_connect(dbllist_t*, dbllist_t*) : Union 2 lists - used to connect the fail list + output list.
- printf situation: no message to the stderr - every error returns -1 only ( fatal error exit(-1)), many of the printf dropped- the only reason for it was only for checking some error situation and checking results.
