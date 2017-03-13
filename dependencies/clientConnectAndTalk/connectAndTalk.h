#ifndef _CONNECTANDTALK_H_
#define _CONNECTANDTALK_H_

#include "../client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../socketTalk/socketTalk.h"

int maxstore, connectionestablished; 

void connectAndTalk(int sock); 
int sanitize(char inputstr[], char outputstr[]);
int client_logic_write(int socket, char str[]); 
void client_logic_read(int socket); 

#endif
