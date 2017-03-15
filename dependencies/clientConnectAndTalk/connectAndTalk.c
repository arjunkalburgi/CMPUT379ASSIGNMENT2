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

	printf("1. Query (?) 2. Write/Clear (@) 3. Exit\nWhich of the above options would you like:  \n");
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

int sanitize(char inputstr[], char outputstr[]) {
	/*
		Return 0 for failure
		Return 1 to continue to send message
	*/

	char var[100];

	if (inputstr[0] == '?') {
		//printf("? --> inquiry about n'th entry\n");
		int len = strlen(inputstr)-1; 
		int i;
		for(i = 1; i < len; i++){
			if(!isdigit((int)inputstr[i])>0){
				printf("dont work, not a digit\n");
				return 0; 
			}
		}
		int entrynum;
		sscanf(inputstr, "?%d\n", &entrynum); 
		//printf("the n value is %d \n", entrynum );
		strcat(inputstr, "\r\n");
		//printf("message is %s \n", inputstr);
		strncpy(outputstr, inputstr, len+1); 
		return 1;
	}

	else if (inputstr[0] == '@'){
		char var2[1000];
		bzero(var2, sizeof(var2));
		//printf("@ --> update n'th entry \n");
		//printf("strlen of inputstr: %zu \n",strlen(inputstr));
		int len = strlen(inputstr)-1;
		int i;
		bzero(var, sizeof(var));
		fgets(var, sizeof(var), stdin);
		bzero(outputstr, sizeof(outputstr));
		strncpy(outputstr, inputstr, len+1); 
		strcat(outputstr, var);
		int entrynum;
		int replacementstrlen;
		char replacementstr[1000];
		sscanf(outputstr, "@%d%c%d\n%c", &entrynum, var2, &replacementstrlen, replacementstr); 
		if(var2 == "p"){
			//printf("correct letter: p\n");
		}
		if(replacementstrlen == 0){
			printf("replacementstrlen is %d: need to clean\n", replacementstrlen);
		}
		if(replacementstrlen != (strlen(var)-1)){
			printf("lengths not equal\n");\
			return 0;
			
		}
		printf("entire message is \n%s \n", outputstr);
		return 1; 
	}

	return 0; 
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
	int entrynum, msglen; 
	char flag; 
	char * firstpart;
	firstpart = strtok (instr,"\n");
	sscanf(firstpart, "!%d%s%d", &entrynum, &flag, &msglen); 

	// CASE 0 ERROR
	if (flag == 'e' && msglen == 0) {
		printf("Server: Entry sucessfully written\n");
		return; 
	}

	// CASE !0 ERROR
	if (flag == 'e' && msglen !=0) {
		printf("Server: Err. %d is not a valid entry index.\n", entrynum);
		return; 
	}

	char entrytext[msglen]; 
	memset(entrytext, 0, msglen);
	char format[30]; 
	sprintf(format, "!%%d%%s%%d %%%dc ", msglen); // "!%d%s%d %(msglen)c "
	sscanf(instr, format, &entrynum, &flag, &msglen, entrytext); 

	// CASE NOT ENCRYPTED DATA 
	if (flag == 'c') {
		printf("Server: Entry #%d contains: %s\n", entrynum, entrytext);
	}
	
	// CASE ENCRYPTED DATA 
	if (flag == 'p') {
		char s[1000] = {0};

		int decoded_byte_len;
		char * strptr = base64decode((void *)s, strlen(s), &decoded_byte_len); // convert to base 256
		memcpy(s, strptr, decoded_byte_len); 
		de_crypt(entrytext, msglen); 
		printf("Server: Entry #%d contains: %s\n", entrynum, entrytext);

		//printf("ERROR OCCURRING HERE--> data: instr, format, entrynum, flag, msglen, replystr\n");
		//printf(data);
		//printf("instr: %s\n",instr );
		//printf("format :%s\n", format);
		//printf("entrynum: %d\n",entrynum );
		//printf("msglen: %d\n", msglen);
		//printf("replystr: %s\n", replystr);
		//printf("flag %d\n", flag);
		//printf("Server: Entry %d contains: '%s'\n", entrynum, replystr);
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