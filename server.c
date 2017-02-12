#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	MY_PORT	2222

/* ---------------------------------------------------------------------
 This	is  a sample server which opens a stream socket and then awaits
 requests coming from client processes. In response for a request, the
 server sends an integer number  such	 that  different  processes  get
 distinct numbers. The server and the clients may run on different ma-
 chines.
 --------------------------------------------------------------------- */
int connect() {
	int	sock, snew, fromlength, number, outnum;

	struct	sockaddr_in	master, from;

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

	while (1) {
		fromlength = sizeof (from);
		snew = accept (sock, (struct sockaddr*) & from, & fromlength);
		if (snew < 0) {
			perror ("Server: accept failed");
			exit (1);
		}
	}

	printf("Client is now connected.\n");

	return snew; 
}

void communicate(int s, char str[]) {
	while (1) {
		snew = accept (sock, (struct sockaddr*) & from, & fromlength);
		read (snew, &str, sizeof (str));
		fprintf (stderr, "The client sent you %s\n", str);

		if (str[0] = '1') {
			break; 
		}
	}

	close (snew);
	// 	// write (snew, &outnum, sizeof (outnum));
}

int main() {
	int	s;
	char str[100];

	s = connect(); 

	communicate(s, str); 
}
