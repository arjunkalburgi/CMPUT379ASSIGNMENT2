#ifndef _CONNECTANDTALK_H_
#define _CONNECTANDTALK_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../encryption/cryption.h"

#define	 MY_PORT  8080

void connectAndTalk(int sock); 

#endif
