#ifndef _CONNECTANDTALK_H_
#define _CONNECTANDTALK_H_

#include "../client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../encryption/cryption.h"
#include "../encryption/base64enc-dec.h"

void connectAndTalk(int sock); 
void socket_write(int socket, char str[]);
void socket_read(int socket);
int sanitize(char inputstr[], char outputstr[]);
int client_logic(int socket, char str[]); 

#endif
