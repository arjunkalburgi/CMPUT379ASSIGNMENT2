#include "cryption.h"

void do_crypt(char intext[]) {
	char outbuf[sizeof(intext)] = {0};
	int outlen, tmplen, i;
	/* Bogus key and IV: we'd normally set these from
	* another source.
	*/
	unsigned char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	unsigned char iv[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	// char intext[] = "some Crypto Text";
	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);

	if(!EVP_EncryptUpdate(&ctx, outbuf, &outlen, intext, strlen(intext))) {
		/* Error */
		// return 0;
	}

	/* Buffer passed to EVP_EncryptFinal() must be after data just
	* encrypted to avoid overwriting it.
	*/
	if(!EVP_EncryptFinal_ex(&ctx, outbuf + outlen, &tmplen)) {
		/* Error */
		// return 0;
	}
	outlen += tmplen;
	EVP_CIPHER_CTX_cleanup(&ctx);

	/*for (i=0; i<=(outlen-1); i++){
		printf("%d", outbuf[i]);
	}

	printf("\n");*/
	strncpy(intext, outbuf, outlen); 
}

void de_crypt(char intext[], int test) {
	char outbuf[sizeof(intext)];
	int outlen, tmplen, i;
	/* Bogus key and IV: we'd normally set these from
	* another source.
	*/
	unsigned char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	unsigned char iv[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	// char intext[] = "some Crypto Text";
	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	EVP_DecryptInit_ex(&ctx, EVP_aes_256_cbc(), NULL, key, iv);
	printf("num decoded bytes: %zu\n",strlen(intext));
	printf("test: %d\n",test);

	if(!EVP_DecryptUpdate(&ctx, outbuf, &outlen, intext, test)) {
		/* Error */
		printf("ERROR IN de_crypt 56\n");
		return;
	}
	// printf("outlen 1 %d\n", outlen);

	/* Buffer passed to EVP_EncryptFinal() must be after data just
	* encrypted to avoid overwriting it.
	*/
	if(!EVP_DecryptFinal_ex(&ctx, outbuf + outlen, &tmplen)) {
		/* Error */
		printf("ERROR IN de_crypt 65\n"); 
		return;
	}
	outlen += tmplen;
	// printf("outlen 2 %d\n", outlen);
	EVP_CIPHER_CTX_cleanup(&ctx);

	/*for (i=0; i<=(outlen-1); i++){
		printf("%d", outbuf[i]);
	}

	printf("\n");*/
	bzero(intext, test);
	strncpy(intext, outbuf, outlen); 
}