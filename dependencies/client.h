#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <strings.h>
#include "clientConnectAndTalk/connectAndTalk.h"

uint16_t MY_PORT;
char hostname[20]; 
char keyfile_path[100];

#endif
