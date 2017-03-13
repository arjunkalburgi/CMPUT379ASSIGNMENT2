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
		bzero(str, sizeof(str));
		socket_read(snew, str); 

		// LOGIC 
		if (!server_logic(snew, str)) {
			return (void *)0; 
		}
	}
	free(entrystore); 
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
		//printf("replystr: %s\n", replystr);

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
