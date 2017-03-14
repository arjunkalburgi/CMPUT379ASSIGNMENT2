#include "./connectAndTalk.h"

void connect_and_talk() {
	int	number;

	struct	sockaddr_in	server;

	struct	hostent		*host;

	host = gethostbyname ("localhost");

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
	signal(SIGINT, orderly_exit);

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
	char str[1000] = {0};
	bzero(str, strlen(str));
	printf("?10, @9p4 or Exit\n");
	fgets(str, sizeof(str), stdin); // blocks

	if(str[0] == 'E') {
		orderly_exit(); 
	}

	char outputstr[1000]; 
	if (!sanitize(str, outputstr)) {
		return 0; 
	}
	//printf("og outputstr: %s\n", outputstr);
	socket_write(sock, outputstr); 
	bzero(outputstr, strlen(outputstr));
}

int sanitize(char inputstr[], char outputstr[]) {
	/*
		Return 0 for failure
		Return 1 to continue to send message
	*/

	char var[100];

	if (inputstr[0] == '?') {
		printf("? --> inquiry about n'th entry\n");
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
		printf("@ --> update n'th entry \n");
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

	char dest[100];
	memset(dest, '\0', sizeof(dest));
	strcpy(dest, instr);
	// now instr is the server's message
	printf ("Server: \n%s\n", instr);

	// CASE: START (CONNECTION ESTABLISHED)
	if (!connectionestablished) {
		sscanf(instr, "CMPUT379 Whiteboard Server v0\n%d\n", &maxstore); 
		printf("The maximum entrynumber in the store is %d\n", maxstore);
		connectionestablished = 1; 
		return; 
	}

	int entrynum, msglen; 
	char flag; 
	
	// analyze the string
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
	
	// CASE PASS 
	if (flag == 'p') {
		char replystr[msglen]; 
		//hack-ey strcpy, still dont fix here though
		strcpy(instr, dest);
		memset(replystr, 0, msglen);
		char format[30]; 
		sprintf(format, "!%%d%%s%%d %%%dc ", msglen); // "!%d%s%d %(msglen)c "
		sscanf(instr, format, &entrynum, &flag, &msglen, replystr); 
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