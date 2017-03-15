#include "./connectAndTalk.h"

void connect_and_talk() {
	int	number;

	struct	sockaddr_in	server;

	struct	hostent		*host;

	host = gethostbyname (hostname); //'localhost'

	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}

	printf("start\n");

	sock = socket (AF_INET, SOCK_STREAM, 0);

	if (sock < 0) {
		perror ("Client: cannot open socket");
		exit (1);
	}

	bzero (&server, sizeof (server));
	bcopy (host->h_addr, & (server.sin_addr), host->h_length);
	server.sin_family = host->h_addrtype;
	server.sin_port = htons (MY_PORT);

	if (connect (sock, (struct sockaddr*) & server, sizeof (server))) {
		perror ("Client: cannot connect to server");
		exit (1);
	}

	// set up exit signal
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = orderly_exit;
	sigaction(SIGINT, &action, NULL);
	// signal(SIGINT, orderly_exit);

	// wait for connection established message
	connectionestablished = 0; 
	client_logic_read(sock);

	while (1) {
		// WRITE
		if (!client_logic_write()) {
			printf("Try again\n");
			continue;
		}

		// READ
		client_logic_read(sock); // blocks

		sleep(2);
	}
}

int client_logic_write() {
	// Return 0 for try again
	// Return 1 for read

	char outputstr[1000] = {0};
	int action, entrynum, enc; 
	char msg[900] = {0};

	printf(" 1. Query (?) 2. Write/Clear (@) 3. Exit\nWhich of the above options would you like:  \n");
	scanf("%1d", &action); 
	
	if (action <= 1 && action >= 3) {
		printf("Please chose an option out of 1, 2 or 3.\n");
		return 0; 
	}

	if (action == 3) {
		orderly_exit(); 
	}

	printf("What entry would you like to act on (1-%d):\nEntry # : ", maxstore); 
	scanf("\n%d", &entrynum); 

	if (action == 1) {
		sprintf(outputstr, "?%d\n\n", entrynum); 
		socket_write(sock, outputstr); 
		return 1; 
	}

	printf("Write your message for entry %d. (Hit enter to clear the entry)\n Message: ", entrynum);
	scanf("\n%s", msg); 
	printf(" Would you like to encrypt your message? (1 for yes): ");
	scanf("\n%d", &enc); 

	if (enc == 1) {
		sprintf(outputstr, "@%dp", entrynum); 
		socket_write_encode(sock, outputstr, msg); 
		bzero(outputstr, strlen(outputstr));
		printf("enc 1 ...");
		return 1; 
	}
	printf("enc not1...");
	sprintf(outputstr, "@%dc%d\n%s\n", entrynum, (int) strlen(msg), msg); 
	socket_write(sock, outputstr); 
	bzero(outputstr, strlen(outputstr));
	return 1; 
}

void client_logic_read(int sock) {
	char instr[1000] = {0}; 
	socket_read(sock, instr); // blocks until read 

	printf ("Server enc: \n%s\n", instr);

	// CASE: START (CONNECTION ESTABLISHED)
	if (!connectionestablished) {
		sscanf(instr, "CMPUT379 Whiteboard Server v0\n%d\n", &maxstore); 
		printf("The maximum entrynumber in the store is %d\n", maxstore);
		connectionestablished = 1; 
		return; 
	}

	// analyze the string
	int entrynum= 0;
	int msglen = 0; 
	char flag[1000]; 
	char replacementstring[1000];
	char * firstpart;
	printf("instr: %s\n", instr);
	sscanf(instr, "!%d%c%d\n%s", &entrynum, flag, &msglen, replacementstring); 
	printf("entrynumREAL: %d\n", entrynum);
	printf("flag: %s\n", flag);
	printf("msglen: %d\n", msglen);
	// CASE 0 ERROR
	if ((strcmp(flag,"e")==0) && msglen == 0) {
		printf("Server: Entry sucessfully written\n");
		return; 
	}
	// CASE !0 ERROR
	if ((strcmp(flag,"e")==0) && msglen !=0) {
		printf("Server: Err. %d is not a valid entry index.\n", entrynum);
		return; 
	}

	char entrytext[msglen]; 
	memset(entrytext, 0, msglen);
	char format[30]; 
	sprintf(format, "!%%d%%s%%d %%%dc ", msglen); // "!%d%s%d %(msglen)c "

	sscanf(instr, format, &entrynum, &flag, &msglen, entrytext); 
	
	// CASE NOT ENCRYPTED DATA 
	if (strcmp(flag,"c")==0) {
		printf("Server: Entry #%d contains: %s\n", entrynum, entrytext);
	}
	
	// CASE ENCRYPTED DATA 
	if (strcmp(flag,"c")==0) {
		char s[1000] = {0};

		int decoded_byte_len;
		char * strptr = base64decode((void *)s, strlen(s), &decoded_byte_len); // convert to base 256
		memcpy(s, strptr, decoded_byte_len); 
		de_crypt(entrytext, msglen); 
		printf("Server: Entry #%d contains: %s\n", entrynum, entrytext);
		return; 
	}
}

void orderly_exit() {
	char exat[4]; 
	sprintf(exat, "Exit");
	socket_write(sock, exat); 
	close(sock);
	printf("\nThank You but Please Come backkkkkkkkkkkk\n");
	exit(0); 
}

void server_shutdown() {
	char shut[7]; 
	sprintf(shut, "SIGTERM");
	socket_write(sock, shut); 
	close(sock); 
	printf("You have shut down the server\n");
	printf("Bye\n");
	exit(0); 
}