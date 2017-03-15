#ifndef _CONNECT_H_
#define _CONNECT_H_

#include <netinet/in.h>
#include <pthread.h>
#include "talk.h"

extern int maxstore; 
extern uint16_t MY_PORT;
extern char start_method[7]; 
int activeThreads; 
uint16_t NUM_THREADS; 
pthread_t thread[10]; //NUM_THREADS

int create_socket();
void connectClients(int sock);

#endif
