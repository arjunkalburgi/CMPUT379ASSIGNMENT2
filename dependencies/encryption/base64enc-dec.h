/* A BASE-64 ENCODER AND DECODER USING OPENSSL */
#include <openssl/pem.h>
#include <string.h> 

char *base64encode (const void *b64_encode_this, int encode_this_many_bytes);
char *base64decode (const void *b64_decode_this, int decode_this_many_bytes); 
