#include "./connectAndTalk.h"

int connect_socket() {
	int	sock;

	struct	sockaddr_in	master, from;
	printf("start\n");

	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Server: cannot open master socket");
		exit (1);
	}

	master.sin_family = AF_INET; //UNIX
	master.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY;
	master.sin_port = htons (MY_PORT);

	if (bind (sock, (struct sockaddr*) &master, sizeof (master))) {
		perror ("Server: cannot bind master socket");
		exit (1);
	}

	listen (sock, 5);
	return sock; 
}

void connection_established(int sock) {
	char str[25] = "connection_established";
	write(sock, &str, sizeof(str));
}

void talk(int sock) {
	char str[100];
	int	snew, fromlength;
	struct	sockaddr_in	from;

	// while (1) {
	// 	fromlength = sizeof (from);
	// 	snew = accept (sock, (struct sockaddr*) & from, & fromlength);
	// 	if (snew < 0) {
	// 		perror ("Server: accept failed");
	// 		exit (1);
	// 	}
	// 	// write (snew, &sendingthing, sizeof (sendingthing));
	// 	close (snew);
	// }
	while (1) {
		fromlength = sizeof (from);
		snew = accept (sock, (struct sockaddr*) & from, & fromlength);
		int num = read (snew, &str, 100);

		char * strptr = base64decode((void *)str, strlen(str)); // convert to base 256
		strncpy(str, strptr, sizeof(str)-1); 
		de_crypt(str); // decrypt 
		printf ("The client sent you: %s", str);
		// server sanitize 
		// server switch (main function) server_function(str, snew);
			// switch to ? or ! or @

		if (str[0] == 'e') {
			break; 
		}
	}
	close (snew);
}
