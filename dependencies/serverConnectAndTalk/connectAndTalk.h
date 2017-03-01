#ifndef _CONNECTANDTALK_H_
#define _CONNECTANDTALK_H_

#include "../server.h"
#include <pthread.h>
#include "talk.h"

int connect_socket();
void connection_established(); 
void talk(int sock);

#endif
