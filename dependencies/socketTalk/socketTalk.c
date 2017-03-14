#include "./socketTalk.h"

void socket_write(int socket, char str[]) {
	char s[1000] = {0};
	strncpy(s, str, strlen(str)-1); 
	//printf("socket_write : %s\n",str );
	do_crypt(s); // encrypt

	char * strptr = base64encode((void *)s, strlen(s)); // convert to base64
	strncpy(s, strptr, sizeof(s)-1); 

	write (socket, &s, strlen(s));
	//printf("socket_write: %s\n", str);
	//printf("socket_write_s: %s\n", s);

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

	char * strptr = base64decode((void *)s, strlen(s)); // convert to base 256
	
	strncpy(s, strptr, sizeof(s)-1); 
	de_crypt(s); // decrypt
	//printf("socket_read2: %s\n", s);
	strncpy(str, s, strlen(s)); 
	//printf("socket_read3: %s\n", str);

}