#ifndef _TALK_H_
#define _TALK_H_

//#include "connectAndTalk.h"
#include "../encryption/cryption.h"
#include "../encryption/base64enc-dec.h"
#include "../store/store.h"
#include "../socketTalk/socketTalk.h"

extern int maxstore; 
extern int activeThreads; 
extern pthread_t thread[];

store* entrystore; 

void *server_thread(void * arg); 
int server_logic(int socket, char str[]);

#endif
