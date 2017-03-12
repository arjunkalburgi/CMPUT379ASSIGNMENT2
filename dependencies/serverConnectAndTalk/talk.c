#include "./talk.h"

void *server_thread(void * arg) {
	char *connectionestablished = "Connection established";
	printf("Connection established \n\n");
	int snew = *(int *) arg;
	socket_write(snew, connectionestablished); 
	char str[1000] = {0};
	// char *string = malloc(100);

	while (1) {
		// READ 
		socket_read(snew, str); 

		// LOGIC 
		if (!server_logic(snew, str)) {
			return (void *)0; 
		}
		
	}
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
	int maxstore = 12;
 
	// CASE ? QUERY
	if (str[0] == '?') {
		sscanf(str, "?%d[^.]", &entrynum); 
	
		// if entrynumber is bad
		if (entrynum > maxstore) {
			char reply[100];
			sprintf(reply, "!%de14\nNo such entry!", entrynum); 
			socket_write(socket, reply); 
			return 1; 
		} 
	
		// get entrynum from store
		// strncpy(entrystr, store[entrynum].entry, strlen(store[entrynum].entry)); 

		// create reply string
		// int replystrlen = strlen(entrystr); 
		// sprintf(replystr, "!%dp%d\n%s\n", entrynum, replystrlen, replystr); 
		// socket_write(socket, replystr); 
		// return 1; 
	}

	// CASE @ UPDATE
	if (str[0] == '@') {
		int replacementstrlen;
		char replacementstr[1000]; 
		sscanf(str, "@%dp%d\n%s\n", &entrynum, &replacementstrlen, &replacementstr); 
		
		char reply[100];
		// if entrynumber is bad
		if (entrynum > maxstore) {		
			sprintf(reply, "!%de14\nNo such entry!", entrynum); 
			socket_write(socket, reply); 
			return 1; 
		}

		// if replacementstrlen is bad
		if (strlen(replacementstr) != replacementstrlen) {
			// corrupted through socket. reject
		}

		// replace string
		if (entrynum == 0) {
			store[entrynum].entry = "";
		} else {
			store[entrynum].entry = replacementstr; 
		}
		
		// reply 
		// sprintf(replystr, "!%de0\n\n", entrynum); 
		// socket_write(socket, replystr); 
		// return 1; 
	}
	

}
