#include "./socketTalk.h"

void socket_write(int socket, char str[]) {
	char s[1000] = {0};
	strncpy(s, str, strlen(str)-1); 
	
	write (socket, &s, strlen(s));
}

void socket_write_encode(int socket, char str[], char encryptstr[]) {
	char s[1000] = {0};
	char enc_s[1000] = {0};

	// Encrypt and Encode the message
	sprintf(enc_s, "CMPUT379 Whiteboard Encrypted v0\n%s", encryptstr); 
	do_crypt(enc_s); 
	char * strptr = base64encode((void *)enc_s, strlen(enc_s)); // convert to base64
	bzero(enc_s, strlen(enc_s)); 
	strncpy(enc_s, strptr, sizeof(enc_s)-1); 

	// Make and send string
	sprintf(s, "%s%d\n%s\n", str, (int)strlen(enc_s), enc_s); 

	write (socket, &s, strlen(s));
}

void socket_read(int socket, char str[]) {
	char s[1000] = {0}; 
	//printf("socket_read: %s\n", str);

	//printf("socket_read : %s\n",str );
	
	int num = read (socket, &s, 100);
	if (num < 0) {
		close(socket); 
		printf("Socket broke, this is no longer safe. Run again\n");
		exit(0);  			
	}

	strncpy(str, s, strlen(s)); 
	//printf("socket_read3: %s\n", str);
	//printf("len s %zu\n", strlen(str));
}