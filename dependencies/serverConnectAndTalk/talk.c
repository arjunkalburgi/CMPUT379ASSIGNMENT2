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
			break; 
		}
	}
	free(entrystore); 

	// THIS SHOULD HAVE A SEMAPHORE AROUND IT
	int threadNum = snew; 
	for (threadNum; threadNum<activeThreads; threadNum++) {
		thread[threadNum] = thread[threadNum+1]; 
	}
	activeThreads--;
}

int server_logic(int socket, char str[]) {

	// CHECK FOR EXIT
	if (str[0] == 'E') {
		close (socket);
		printf("Closed\n");
		return 0; 
	}
	char dest[100];
	memset(dest, '\0', sizeof(dest));
	strcpy(dest, str);

	// CHECK FOR SHUTDOWN (SIGTERM)
	if (str[0] == 'S') {
		activeThreads = -1;
		printf("SIGTERM\n");
		return 1; 
	}

	printf ("The client sent you: \n%s\n", str);
	//printf("beginning of server_logic, str: \n%s\n", str);
	//printf("dest test: \n%s\n", dest);
	int entrynum; 
	char replystr[100];
	// int maxstore = 10;
 
	// CASE ? QUERY
	if (str[0] == '?') {
		// get vars
		char entrystr[1000]; 
		sscanf(str, "?%d ", &entrynum); 
		//printf("OG str ?: \n%s\n", str);
		// if entrynumber is bad
		if (entrynum > maxstore) {
			//printf("entrynum > maxstore (%d > %d)\n", entrynum, maxstore);
			sprintf(replystr, "!%de14\nNo such entry!", entrynum); 
			//printf("replystr ?1: %s\n ", replystr);
			socket_write(socket, replystr); 
			return 1; 
		} 
		//printf("entrynum: %d\n", entrynum); //entrynum is right
		//printf("str len entrystore[entrynum].entry: %zu\n",strlen(entrystore[entrynum-1].entry));
		// get entry from store, this is not getting anything
		strcpy(entrystr, entrystore[entrynum-1].entry); 
		//printf("entry store: %s\n", entrystore[entrynum-1].entry);
		//printf("entrystr: %s\n", entrystr );
		//this is where we are encountering some sort of error, entrystr is not decrypted, or not the right message
		// reply
		//printf("entrystr ERROR HERE ?2: %s\n ", entrystr);

		sprintf(replystr, "!%dp%d\n%s\n", entrynum, (int) strlen(entrystr), entrystr); 
		//printf("replystr ?2: %s\n ", replystr);
		socket_write(socket, replystr); 
		return 1; 
	}

	// CASE @ UPDATE
	if (str[0] == '@') {
		// get vars
		char * firstpart;
		int replacementstrlen;
		
		firstpart = strtok (str,"\n");
		//printf("str test: \n%s\n", str);
		strcpy(str, dest);
		//printf("dest test2: \n%s\n", dest);
		//printf("str test: \n%s\n", str);


		//printf("OG str @, not receiving 2nd line: \n%s\n", str);
		sscanf(firstpart, "@%dp%d", &entrynum, &replacementstrlen); 
		
		char replacementstr[replacementstrlen]; 
		memset(replacementstr, 0, replacementstrlen);
		
		char format[30]; 
		sprintf(format, "@%%dp%%d %%%dc\n", replacementstrlen); 
		sscanf(str, format, &entrynum, &replacementstrlen, replacementstr); 
		//printf("ERROR OCCURRING HERE--> data: str, format, entrynum, replacementstrlen, replacementstr\n");
		//printf(data);
		//printf("str: %s\n",str );
		//printf("format :%s\n", format);
		//printf("entrynum: %d\n",entrynum );
		//printf("replacementstrlen: %d\n", replacementstrlen);
		//printf("replacementstr: %s\n", replacementstr);
		
		// if entrynumber is bad
		if (entrynum > maxstore) {		
			sprintf(replystr, "!%de14\nNo such entry!", entrynum); 
			//printf("replystr @1: %s\n ", replystr);
			socket_write(socket, replystr); 
			return 1; 
		}

		// if replacementstrlen is bad
		if (strlen(replacementstr) != replacementstrlen) {
			// corrupted through socket. reject
		}

		// replace string
		if (replacementstrlen == 0) {
			strncpy(entrystore[entrynum-1].entry, "", strlen("")); 
		} else {
			//printf("replacementstr: %s\n replacementstr len: %zu\n", replacementstr, strlen(replacementstr));
			strncpy(entrystore[entrynum-1].entry, replacementstr, strlen(replacementstr));
			//printf("entrystore[entrynum].entry: %s\n", entrystore[entrynum].entry);
			//printf("entrystore[entrynum-1].entry: %s\n", entrystore[entrynum-1].entry);
		}
		
		// reply 
		sprintf(replystr, "!%de0\n\n", entrynum); 
		//printf("replystr @2: %s\n ", replystr);
		socket_write(socket, replystr); 
		return 1; 
	}
}
