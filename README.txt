# After "make":
	To run server: ./wbs379 8080 -n entries
	To run client: ./wbc379 localhost 8080

Will be prompted on client to enter (1,2,or 3) for 3 possible inputs:
	1: to Query(?)
		a. then requests Entry# (within limit stated at top of program)
			Should return what ever is stored in that entry #
	2: to Write/Clear(@)
		a. then requests Entry# (within limit stated at top of program)
		b. requests message you wish to store
		c. requests if you wish to encrypt it or not
			Should return ![entry#]p0 upon success!
	3: to Exit
		a. will exit the client thread

Did not implement:
	keyfile
	statefile
	  

HOW IT WORKS (MAKEFILE): 
	Run Server: ./wbs379 8080 -n entries
	Run Client: ./wbc379 localhost 8080

	The Makefile is all abstracted by each dependency (/dependencies) except for clientConnectAndTalk and serverConnectAndTalk as they contain the main and vital functionality. 

HOW IT DOESN'T WORK (MAKEFILE): 
	In the server, -n entries and -f statefile options are actually not implemented due to time constraints.
	In the client, the keyfile is disregarded. 

HOW IT WORKS (CLIENTCONNECTANDTALK): 
	Main function: connectAndTalk() - This function sets up the socket and contains the while loop to communicate with the server. 

	Other important functions: client_logic_write(), client_logic_read() - abstractions of the logic required to read and write through the socket. 

	Minor functions: orderly_exit() - signal handler for exiting the client. 

HOW IT DOESN'T WORK (CLIENTCONNECTANDTALK): 
	String parsing is pretty messy and for the longest time we didn't understand the proper format of the string through the socket. Would have loved bullet form deliverables instead of a written spec sheet. 

	The last character gets cut off :/ 

HOW IT WORKS (SERVERCONNECTANDTALK): 
	Main functions: connectClients(), server_thread() - These functions contain the creation of threads and the main thread functionality. 

	Other important functions: server_logic(), create_socket() - server_logic is an abstraction of all the logic for what the server needs to do. create_socket makes the socket. 

	Minor functions: shutdownserver() - signal handler for killing the server. 

HOW IT DOESN'T WORK (SERVERCONNECTANDTALK): 
	Due to time constraints and an initial misunderstanding, each thread has it's own list of entries instead of them being stored on the process and shared. To fix this would require locks/semaphores which we did not have time to implement. 

	This became complicated when approaching the implementation of saving entries. Do you have a file for each thread? This functionality therefore wasn't implemented either. 

	The server logic in connectClients() is supposed to be daemonized but instead it runs in the main process. It acts like a daemon though since that's the only thing it does. 

	Also wrong with connectClients() is that there is no logic in place to handle the case that 100 (NUM_THREADS) threads have connected to the server. Even if it's the same client exiting and reconnecting. This is not worth implementing with the time left, but you would fix it by dynamically allocating the pthread_t thread variable and polling the threads to check on which ones have terminated so you can clear them. 
 
HOW IT WORKS (STORE): 
	Typedef struct and a function to allocate memory. 

HOW IT DOESN'T WORK (STORE): 
	It's a pretty useless abstraction. 

HOW IT WORKS (SOCKETTALK): 
	Main functions: socket_write_encode() - socket_write_encode() has some logic for creating the string to send to the server. It abstracts some work and keeps encryption/64coding out of client_logic. 

	Other functions: socket_write(), socket_read() - both pretty useless wrappers. Don't do much. 

HOW IT DOESN'T WORK (SOCKETTALK): 
	works

HOW IT WORKS (ENTRYPTION): 
	This code is mostly taken from lab material provided in CMPUT 379 LAB. 
