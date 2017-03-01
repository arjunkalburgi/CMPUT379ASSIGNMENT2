#ifndef _CONNECT_H_
#define _CONNECT_H_

#include "../server.h"
#include <pthread.h>
#include "talk.h"

int create_socket();
void connectClients(int sock);

#endif
