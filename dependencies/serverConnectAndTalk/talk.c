#include "./talk.h"

void *server_thread(void * arg) {
	// MAKE STORE
	entrystore = newStore(maxstore);

	// SET CONNECTION
	int snew = *(int *) arg; 

	// SIGTERM 
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = shutdownserver;
	sigaction(SIGTERM, &action, NULL);
	// signal(SIGTERM, orderly_exit);

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

	// CLOSE THREAD
	free(entrystore); 
	activeThreads--;
}

int server_logic(int socket, char str[]) {
	// Return 0 to exit thread 
	// Return 1 to continue

	int entrynum, replacementstrlen; 
	char replystr[1000]; 

	// CHECK FOR EXIT
	if (str[0] == 'E') {
		close (socket);
		printf("Closed\n");
		return 0; 
	}

	// CASE ? QUERY 
	if (str[0] == '?') {

		sscanf(str, "?%d\n\n", &entrynum); 

		if (entrynum > maxstore) {
			sprintf(replystr, "!%de14\nNo such entry!", entrynum); 
			socket_write(socket, replystr); 
			return 1; 
		} 

		char entrystr[1000]; 
		strcpy(entrystr, entrystore[entrynum-1].entry); 
		sprintf(replystr, "!%dp%d\n%s\n", entrynum, (int) strlen(entrystr), entrystr); 
		socket_write(socket, replystr); 
		return 1; 
	}

	// CASE @ UPDATE 
	if (str[0] == '@') {
		char saved_str[1000]; 
		strcpy(saved_str, str); 

		char *flag; 

		char *firstpart = strtok(str, "\n"); 
		sscanf(firstpart, "@%d%1c%d", &entrynum, flag, &replacementstrlen); 

		// CASE ENTRYNUM ERROR 
		if (entrynum > maxstore) {
			sprintf(replystr, "!%de14\nNo such entry!", entrynum); 
			socket_write(socket, replystr); 
			return 1; 
		} 

		// CASE CLEAR 
		if (replacementstrlen == 0) {
			strncpy(entrystore[entrynum-1].entry, "", strlen("")); 
			entrystore[entrynum-1].flag = '\0'; 
		} 

		// CASE REPLACE 
		else {
			char replacementstr[replacementstrlen]; 
			char format[30]; 
			sprintf(format, "@%%d%%1c%%d\n%%%dc\n", replacementstrlen); 
			sscanf(str, "@%d%1c%d\n%c\n", &entrynum, flag, &replacementstrlen, replacementstr); 

			// CORRUPTED DATA 
			if (strlen(replacementstr) != replacementstrlen) {
				printf("Data was corrupted.\n");
				shutdownserver(); 
			}

			strncpy(entrystore[entrynum-1].entry, replacementstr, replacementstrlen);
			entrystore[entrynum-1].flag = flag; 
		}

		sprintf(replystr, "!%de0\n\n", entrynum); 
		socket_write(socket, replystr); 
		return 1; 
	}
}

void shutdownserver() {
	printf("Bye\n");
	exit(0); 
}