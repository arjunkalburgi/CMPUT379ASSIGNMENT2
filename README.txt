After "make":
To run server: ./wbs379 8080 -n entries
To run client: ./wbc379 localhost 8080

can enter 4 different inputs:
	?[entry_number] : this will return the message at the specified entry number
	@[entry_number][p or c][message_length] \n [message]: this will return ![entry_number]e0\n\n
			note: as of right now, we encrypted the full message (including the "?12" and "@12p3\nhey"), as we misinterpreted the 
			specifications. This leads to more errors from socket read and write, as decrypting (?/@/!/numbers) causes errors
	Exit: it will close the thread 
	SIGTERM: signal from linux, it terminates server, and closes socket, so exits client.

Did not implement:
	keyfile
	statefile
	  