#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "serverConnectAndTalk/connect.h"

#define	MY_PORT	8080
#define NUM_THREADS 10
int maxstore; 

#endif
