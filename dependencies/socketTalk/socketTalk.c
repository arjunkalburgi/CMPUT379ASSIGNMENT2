#include "./socketTalk.h"

void socket_write(int socket, char str[]) {
	char s[1000] = {0};
	strncpy(s, str, strlen(str)-1); 
	
	write (socket, &s, strlen(s));
}

void socket_write_encode(int socket, char str[], char encryptstr[]) {
	char s[1000] = {0};

	// Encrypt and Encode the message
	do_crypt(encryptstr); 
	char * strptr = base64encode((void *)encryptstr, strlen(encryptstr)); // convert to base64
	bzero(encryptstr, strlen(encryptstr)); 
	strncpy(encryptstr, strptr, sizeof(encryptstr)-1); 

	// Make and send string
	sprintf(s, "%s%d\n%s\n", str, (int)strlen(encryptstr), encryptstr); 

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
	int test;
	char * strptr = base64decode((void *)s, strlen(s), &test); // convert to base 256
	//len of s is wrong for some reason, taking too many chars
	memcpy(s, strptr, test); 
	de_crypt(s, test); // decrypt
	printf("socket_read2: %s\n", s);
	
	strncpy(str, s, strlen(s)); 
	//printf("socket_read3: %s\n", str);
	//printf("len s %zu\n", strlen(str));
}