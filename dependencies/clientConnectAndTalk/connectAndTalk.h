#ifndef _CONNECTANDTALK_H_
#define _CONNECTANDTALK_H_

#include "../client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../encryption/cryption.h"
#include "../encryption/base64enc-dec.h"

void connectAndTalk(int sock); 

#endif
