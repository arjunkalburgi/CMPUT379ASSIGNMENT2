#include "./talk.h"

void *server_thread(void * arg) {
	// MAKE STORE
	entrystore = newStore(maxstore);

	// SET CONNECTION
	int snew = *(int *) arg;

	// CONNECTION ESTABLISHED
	printf("Connection established \n\n");
	char connest[50];
	sprintf(connest, "CMPUT379 Whiteboard Server v0\n%d\n", maxstore+1); 
	socket_write(snew, connest); 
	
	// COMMUNICATE
	char str[1000] = {0};
	while (1) {
		// READ 
		socket_read(snew, str); 

		// LOGIC 
		if (!server_logic(snew, str)) {
			return (void *)0; 
		}
	}
	free(entrystore); 
}

void socket_read(int socket, char str[]) {
	char s[1000] = {0};
	int num = read (socket, &s, 100);

	if (num < 0) {
		close(socket); 
		printf("Closed\n");
		exit(0);  			
	}

	char * strptr = base64decode((void *)s, strlen(s)); // convert to base 256

	strncpy(s, strptr, sizeof(s)-1); 
	de_crypt(s); // decrypt

	// printf ("I got: %s|\n", s);
	strncpy(str, s, 3); 
}

void socket_write(int socket, char str[]) {
	char s[1000] = {0};
	strncpy(s, str, strlen(str)-1); 

	// client sanitize (formatting)
	do_crypt(s); // encrypt

	char * strptr = base64encode((void *)s, strlen(s)); // convert to base64
	strncpy(s, strptr, sizeof(s)-1); 

	write (socket, &s, strlen(s));
}

int server_logic(int socket, char str[]) {

	// CHECK FOR EXIT
	if (str[0] == 'e') {
		close (socket);
		printf("Closed\n");
		return 0; 
	}

	printf ("The client sent you: %s\n", str);
	
	int entrynum; 
	char replystr[100];
	// int maxstore = 10;
 
	// CASE ? QUERY
	if (str[0] == '?') {
		char entrystr[1000]; 
		sscanf(str, "?%d ", &entrynum); 
	
		// if entrynumber is bad
		if (entrynum > maxstore) {
			printf("entrynum > maxstore (%d > %d)\n", entrynum, maxstore);
			sprintf(replystr, "!%de14\nNo such entry!", entrynum); 
			socket_write(socket, replystr); 
			return 1; 
		} 
	
		// get entry from store
		strncpy(entrystr, entrystore[entrynum].entry, strlen(entrystore[entrynum].entry)); 

		// reply
		sprintf(replystr, "!%dp%d\n%s\n", entrynum, (int) strlen(entrystr), entrystr); 
		socket_write(socket, replystr); 
		return 1; 
	}

	// CASE @ UPDATE
	if (str[0] == '@') {
		int replacementstrlen;
		char replacementstr[1000]; 
		memset(replacementstr, 0, 1000);
		sscanf(str, "@%dp%d\n%999c\n", &entrynum, &replacementstrlen, replacementstr); 
		
		// if entrynumber is bad
		if (entrynum > maxstore) {		
			sprintf(replystr, "!%de14\nNo such entry!", entrynum); 
			socket_write(socket, replystr); 
			return 1; 
		}

		// if replacementstrlen is bad
		if (strlen(replacementstr) != replacementstrlen) {
			// corrupted through socket. reject
		}

		// replace string
		if (entrynum == 0) {
			strncpy(entrystore[entrynum-1].entry, "", strlen(""));
		} else {
			strncpy(entrystore[entrynum-1].entry, replacementstr, strlen(replacementstr));
		}
		
		// reply 
		sprintf(replystr, "!%de0\n\n", entrynum); 
		socket_write(socket, replystr); 
		return 1; 
	}
}
