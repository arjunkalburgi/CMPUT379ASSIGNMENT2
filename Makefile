CC=gcc 

all: client server

clean: 
	@rm -f client
	@rm -f server

client: client.c ./client_files/connectAndTalk.c
	$(CC) $< ./client_files/connectAndTalk.c -o client 

server: server.c ./server_files/connectAndTalk.c 
	$(CC) $< ./server_files/connectAndTalk.c -o server
