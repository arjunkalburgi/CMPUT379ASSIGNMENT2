#include "./connectAndTalk.h"

void connect_and_talk() {
	int	sock, number;

	struct	sockaddr_in	server;

	struct	hostent		*host;

	host = gethostbyname ("localhost");

	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}

	char str[1000] = {0};
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

	// wait for connection established message
	socket_read(sock);
	
	while (1) {
		// WRITE
		fgets(str, sizeof(str), stdin); // blocks
		socket_write(sock, str);

		// READ
		// socket_read(sock); // blocks

		sleep(2);
	}
}

void socket_write(int socket, char str[]) {
	char str_buf[1000] = {0};
	strncpy(str_buf, str, strlen(str)-1); 

	if (!sanitize(str_buf)) {
		close(socket); 
		exit(0); 
	}

	do_crypt(str_buf); // encrypt

	char * strptr = base64encode((void *)str_buf, strlen(str_buf)); // convert to base64
	strncpy(str_buf, strptr, sizeof(str_buf)-1); 

	write (socket, &str_buf, strlen(str_buf));
}

void socket_read(int socket) {
	char s[1000] = {0}; 

	int num = read (socket, &s, 100);

	char * strptr = base64decode((void *)s, strlen(s)); // convert to base 256
	
	strncpy(s, strptr, sizeof(s)-1); 
	de_crypt(s); // decrypt

	printf ("Server: %s\n", s);
	
	/*int messagesize; 
	char s[100] = {0}; 

	int num = read (socket, &s, 100);

	char * strptr = base64decode((void *)s, strlen(s)); // convert to base 256
	strncpy(s, strptr, sizeof(s)-1); 
	de_crypt(s); // decrypt

	// CASE: NOT EMPTY
		// !12p30
			// s[0] = '!'; 
			// if s[indexOf('p')] => works 
			// s[indexOf('p')] - s[indexOf('0')] = messagesize
		// message
		// print the message
			

	// CASE: EMPTY 
		// !12e0
		//
		// print it's empty (read spec)

		// !47e14
		// No such entry
		// print no such entry
	
	// CASE: START 
		// CMPUT379 Whiteboard Server v0
		// 38

		*/
}

int sanitize(char buffer[]) {
	/*
		Return 0 for failure
		Return 1 to continue to send message
	*/

	// CHECK FOR EXIT
	if (buffer[0] == 'e') {
		printf("TY\n");
		return 0; 
	}

	// CHECK FOR "?" MESSAGE
	// if proper format return 1; else return 0
	return 1;
	// CHECK FOR "@" MESSAGE
	// if proper format return 1; else return 0

	printf("Something went wrong and we were not able to verify your message. Program will exit."); 
	return 0; 
}