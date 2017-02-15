CC=gcc 

all: client server

clean: 
	@rm -f client
	@rm -f server

client: client.c ./dependencies/clientConnectAndTalk/connectAndTalk.c
	$(CC) $< ./dependencies/clientConnectAndTalk/connectAndTalk.c -o client 

server: server.c ./dependencies/serverConnectAndTalk/connectAndTalk.c 
	$(CC) $< ./dependencies/serverConnectAndTalk/connectAndTalk.c -o server
