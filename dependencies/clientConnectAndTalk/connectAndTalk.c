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
	/*


			if (str[0] == '?') {
				printf("? --> inquiry about n'th entry\n");
				int len = strlen(str) - 2; 
				//printf("%d is length\n", len);
				//char inquired_line[len-1];
				int i;
				for(i = 1; i < len; i++){
					if(isdigit((int)str[i])>0){
						//printf("%d str is good so far\n", i);
					}
					else{
						printf("dont work, not a digit\n");
						check = 0;
						break;
					}
				}
				char substring[len];
				memcpy(substring, &str[1], len);

				printf("the n value is %s \n",substring );
			}

			else if ((str[0] == 'e')&&(str[1]=='n')&&(str[2]=='d')) {
				printf("TY\n");
				close (s);
				return; 
			}
			else if (str[0] == '@'){
				printf("@ --> update n'th entry \n");
				int len = strlen(str)-2;
				int i;
				for(i = 1; i < len; i++){
					if(isdigit((int)str[i])>0){
						//printf("%d digit so far\n", i);
					}
					else if(str[i] == 'p'){
						char substring[i];
						memcpy(substring, &str[1], i-1);
						//printf("the n value is %s \n",substring );

						if(str[i+1] == '0'){
							printf("clean?\n");
							fgets(str, sizeof(str), stdin);
							memcpy(str, "clean", 5);
							break;
						}
						
						int j = 0;
						int k = i;
						while(isdigit((int)str[k+1])){
							j++;
							k++;
						}
						char len_of_update[j];
						memcpy(len_of_update, &str[i+1], j);
						//printf("the length of the update message is %s \n",len_of_update);
						
						if(str[k+1] != '\n'){
							check = 0;
							printf("no new line after number, doesnt work\n");
							break;
						}

						else{
							fgets(str, sizeof(str), stdin);

							printf("the n'th value is %s \n",substring );

							printf("the length of the update message should be %s \n",len_of_update);

							printf("the actual length of the update message is %zu \n",strlen(str)-1);

							printf("message is %s \n", str);
						}

						
					}
				}

			}
		}
		if(!check){
			continue;
		}


	*/

}
