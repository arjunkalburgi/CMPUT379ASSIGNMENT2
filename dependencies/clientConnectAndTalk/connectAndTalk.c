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
	
	while(1) {
		int check = 1;
		if(fgets(str, sizeof(str), stdin)){
			
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
						
						int j = 0;
						int k = i;
						while(isdigit((int)str[k+1])){
							j++;
							k++;
						}
						char len_of_update[j-1];
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

							printf("the length of the update message is %s \n",len_of_update);

							printf("message is %s \n", str);
						}

						
					}
				}

			}
		}
		if(!check){
			continue;
		}

		

		// client sanitize (formatting)
		do_crypt(str); // encrypt
		char * strptr = base64encode((void *)str, strlen(str)); // convert to base64
		strncpy(str, strptr, sizeof(str)-1); 
		write (s, &str, sizeof (str));

		// read
		// read (s, &number, sizeof (number));

		sleep(2);
	}
}