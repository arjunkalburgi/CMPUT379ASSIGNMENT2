CC=gcc 
cCAT=./dependencies/clientConnectAndTalk/connectAndTalk.c
sC=./dependencies/serverConnectAndTalk/connect.c
sT=./dependencies/serverConnectAndTalk/talk.c
encryp=./dependencies/encryption/cryption.c
base=./dependencies/encryption/base64enc-dec.c
store=./dependencies/store/store.c
talk=./dependencies/socketTalk/socketTalk.c

all: client server

clean: 
	@rm -f client
	@rm -f server

client: client.c $(cCAT) cryption.o base64enc-dec.o socketTalk.o
	$(CC) $< $(cCAT) cryption.o base64enc-dec.o socketTalk.o -lcrypto -o client 

server: server.c $(sC) $(sT) cryption.o base64enc-dec.o store.o socketTalk.o
	$(CC) $< $(sC) $(sT) cryption.o base64enc-dec.o store.o socketTalk.o -pthread -lcrypto -o server

cryption.o: $(encryp)
	$(CC) $< -c

base64enc-dec.o: $(base)
	$(CC) $< -c

store.o: $(store)
	$(CC) $< -c

socketTalk.o: $(talk) 
	$(CC) $< -c