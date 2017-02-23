#ifndef _CONNECTANDTALK_H_
#define _CONNECTANDTALK_H_

#include "../server.h"
#include "../encryption/cryption.h"
#include "../encryption/base64enc-dec.h"

int connect_socket();
void connection_established(int sock); 
void talk(int sock);

#endif
