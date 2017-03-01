#include "./dependencies/server.h"

int main() {
	char str[100];
	int	snew, fromlength;
	struct	sockaddr_in	from;

	int sock = create_socket(); 
	connectClients(sock); 
	printf("DONE!\n");

}