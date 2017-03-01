#include "./talk.h"

void *server_thread(void * arg) {
	printf("Connection established :P \n");
	int snew = *(int *) arg;
	char str[100];

	while (1) {
		int num = read (snew, &str, 100);
		// printf("%d\n", num);
		if (num == -1) {
			continue; 
		}

		char * strptr = base64decode((void *)str, strlen(str)); // convert to base 256
		strncpy(str, strptr, sizeof(str)-1); 
		de_crypt(str); // decrypt 
		// server sanitize 
		// server switch (main function) server_function(str, snew);
			// switch to ? or ! or @
		printf ("The client sent you: %s", str);

		if (str[0] == 'e') {
			close (snew);
			printf("Closed\n");
			return (void *)0; 
		}
	}
}
