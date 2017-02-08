CC=gcc 

all: client server

clean: 
	@rm -f client
	@rm -f server

client: client.c 
	$(CC) $< -o client 

server: server.c 
	$(CC) $< -o server
