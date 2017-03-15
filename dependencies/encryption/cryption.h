#ifndef _CRYPTION_H_
#define _CRYPTION_H_

#include <openssl/evp.h>
#include <string.h>

void do_crypt(char intext[]); 
void de_crypt(char intext[], int test); 

#endif
