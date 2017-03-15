#ifndef _CONNECTANDTALK_H_
#define _CONNECTANDTALK_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include "../socketTalk/socketTalk.h"

extern uint16_t MY_PORT;
extern char hostname[20]; 
extern char keyfile_path[100];
int sock, maxstore, connectionestablished; 

extern void orderly_exit(); 
void connectAndTalk(); 
int sanitize(char inputstr[], char outputstr[]);
int client_logic_write(); 
void client_logic_read(int sock); 
void orderly_exit(); 
void server_shutdown(); 

#endif
