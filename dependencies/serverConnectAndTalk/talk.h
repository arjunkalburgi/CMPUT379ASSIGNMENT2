#ifndef _TALK_H_
#define _TALK_H_

//#include "connectAndTalk.h"
#include "../encryption/cryption.h"
#include "../encryption/base64enc-dec.h"
#include "../store/store.h"

extern int maxstore; 
store* entrystore; 

void *server_thread(void * arg); 
void socket_read(int socket, char str[]); 
void socket_write(int socket, char str[]); 
int server_logic(int socket, char str[]);

#endif
