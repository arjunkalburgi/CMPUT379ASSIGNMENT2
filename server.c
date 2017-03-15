#include "./dependencies/server.h"

int main(int argc, char const *argv[]) {
	
	if (argc != 4) {
		printf("Please call like so: 'wbs379 portnumber {-f statefile | -n entries'}\n");
		return 0; 
	}

	strncpy(start_method, argv[3], 7); 
	if (sscanf(argv[1], "%" SCNx16, &MY_PORT) != 1) {
		printf("Please call like so: 'wbs379 portnumber {-f statefile | -n entries'\n");
		return 0; 
	}
	
	maxstore = 20; 

	int sock = create_socket(); 
	connectClients(sock); 
	printf("DONE!\n");
}