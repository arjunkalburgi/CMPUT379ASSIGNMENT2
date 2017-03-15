#include "./dependencies/client.h"

int main(int argc, char const *argv[]) {

	if (argc < 3 || argc > 4) {
		printf("Please call like so: 'wbc379 hostname(localhost) portnumber [keyfile]'\n");
		return 0; 
	}

	if (argc == 4) {
		strncpy(keyfile_path, argv[3], strlen(argv[3])); 
	}

	strncpy(hostname, argv[1], strlen(argv[1])); 
	if (sscanf(argv[2], "%" SCNx16, &MY_PORT) != 1) {
		printf("Please call like so: 'wbc379 hostname portnumber [keyfile]'\n");
		return 0; 
	}

	connect_and_talk(); 
}
