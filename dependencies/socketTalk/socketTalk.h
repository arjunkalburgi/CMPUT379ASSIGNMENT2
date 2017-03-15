#ifndef _socketTalk_H_
#define _socketTalk_H_

#include "../encryption/cryption.h"
#include "../encryption/base64enc-dec.h"

void socket_write(int socket, char str[]); 
void socket_write_encode(int socket, char str[], char encryptstr[]); 
void socket_read(int socket, char str[]); 


#endif