#include "./connect.h"

int create_socket() {
	int	sock;

	struct	sockaddr_in	master, from;
	printf("start\n");

	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Server: cannot open master socket");
		exit (1);
	}

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int))<0)
		error("setsockopt(SO_REUSEADDR) failed");

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

void connectClients(int sock) {
	int	fromlength;
	struct sockaddr_in from;
	// pthread_t thread[NUM_THREADS];
	NUM_THREADS = 10;
	int sock_cpy = sock; 
		
	fd_set readfds;
	struct timeval tv;
	int retval;

	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&readfds);
	FD_SET(sock_cpy, &readfds);

	/* Wait up to five seconds. */
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	activeThreads = 0; 
	while (activeThreads!=-1) {

		retval = select(sock_cpy+1, &readfds, NULL, NULL, &tv);
		if (!retval) {
			// no clients in 5s  
			continue; 
		} else if (retval) {

			fromlength = sizeof (from);
			int snew = accept (sock, (struct sockaddr*) & from, & fromlength); // look for client to connect 
			// make a client thread 
			if (activeThreads<NUM_THREADS) {
				pthread_create(&thread[activeThreads], NULL, server_thread, (void *) &snew);
				activeThreads++; 
			}
		}
	}	

	printf("Goodbye\n");
	exit(0);
}