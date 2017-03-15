#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include "serverConnectAndTalk/connect.h"

int maxstore; 
uint16_t MY_PORT;
char start_method[7]; 

#endif