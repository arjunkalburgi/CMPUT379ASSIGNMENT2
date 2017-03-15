#ifndef _STORE_H_
#define _STORE_H_

#include <stdlib.h>

typedef struct _store {
    char *flag; 
    char entry[1000];
} store;

store* newStore(int max); 

#endif
