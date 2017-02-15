CC=gcc 
cCAT=./dependencies/clientConnectAndTalk/connectAndTalk.c
sCAT=./dependencies/serverConnectAndTalk/connectAndTalk.c
encryp=./dependencies/encryption/cryption.c


all: client server

clean: 
	@rm -f client
	@rm -f server

client: client.c $(cCAT) cryption.o
	$(CC) $< $(cCAT) cryption.o -lcrypto -o client 

server: server.c $(sCAT) 
	$(CC) $< $(sCAT) -o server

cryption.o: $(encryp)
	$(CC) $< $(encryp) -c