#include "./connectAndTalk.h"

void connect_and_talk() {
	int	s, number;

	struct	sockaddr_in	server;

	struct	hostent		*host;

	host = gethostbyname ("localhost");

	if (host == NULL) {
		perror ("Client: cannot get host description");
		exit (1);
	}

	char str[1000];
	printf("start\n");

	while (1) {

		s = socket (AF_INET, SOCK_STREAM, 0);

		if (s < 0) {
			perror ("Client: cannot open socket");
			exit (1);
		}

		bzero (&server, sizeof (server));
		bcopy (host->h_addr, & (server.sin_addr), host->h_length);
		server.sin_family = host->h_addrtype;
		server.sin_port = htons (MY_PORT);

		if (connect (s, (struct sockaddr*) & server, sizeof (server))) {
			perror ("Client: cannot connect to server");
			exit (1);
		}

		fgets(str, sizeof(str), stdin);
		// client sanitize (formatting)
		do_crypt(str); // encrypt
		char * strptr = base64encode((void *)str, strlen(str)); // convert to base64
		strncpy(str, strptr, sizeof(str)-1); 
		write (s, &str, sizeof (str));

		// read
		// read (s, &number, sizeof (number));

		if (str[0] == 'e') {
			break; 
		}

		sleep (2);
	}
	close (s);
}