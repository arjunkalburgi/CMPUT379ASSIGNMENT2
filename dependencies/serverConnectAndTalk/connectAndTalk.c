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
	// 	outnum = htonl (number);
	// 	// write (snew, &outnum, sizeof (outnum));
	// 	close (snew);
	// 	number++;
	// }
	while (1) {
		fromlength = sizeof (from);
		snew = accept (sock, (struct sockaddr*) & from, & fromlength);
		read (snew, &str, sizeof (str));

		//decrypt 
		//server sanitize 
		//server switch (main function) server_function(str, snew);
			// switch to ? or ! or @

		fprintf (stderr, "The client sent you: %s", str);

		if (str[0] == 'e') {
			break; 
		}
	}
	close (snew);
}
