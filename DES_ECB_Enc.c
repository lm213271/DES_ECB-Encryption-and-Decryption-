#include <stdlib.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <string.h>
#include <stdint.h>
#include <openssl/evp.h>

int main(int argc, char *argv[])
{
	unsigned char ptxt[2000];
	char ks[100];
	char *outputStr = NULL;
	FILE* keyfile = NULL;
	FILE* inputFile = NULL;
	FILE* outputFile = NULL;

	int i = 0, cmd = 0;
	int len_ptxt, ch;
	int ctxt_len, key_len;
	char c;

	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);	

	// check for command line	
  	while (cmd < argc){
	
		// if cmd line is -k then the next xmd line should be key file
		if (strcmp( "-k" , argv[cmd] ) == 0) {	

			if (argv[cmd + 1] != NULL) {

				keyfile = fopen(argv[cmd+1], "r");
				
				// Check for null
				if (keyfile == NULL) {
					printf("\nError: ks file does not exist.\n");
					exit(0);
				}
				while((c = fgetc(keyfile)) != EOF) {
					strncat(ks, &c, 1);
				}

				fclose(keyfile);
				printf("key: %s\n", ks);
				//printf("key in hex %02x\n", ks);
				
				key_len = strlen(ks) -1;
				if(key_len < 8) {
					printf("\nError: Key MUST be 8 hexadecimal characters\n");
					exit(0);
				}	
			}
			else {
				printf("\nError: ks file does not exist\n");
				exit(0);
			}
		}
		//if cmd line is -i then the next xmd should be input file
		else if (strcmp( "-i" , argv[cmd] ) == 0) {
			if (argv[cmd + 1] != NULL) {
				inputFile = fopen(argv[cmd+1], "r");
				
				// check for null
				if (inputFile == NULL) {
					printf("\nError: input file does not exist\n");
					exit(0);
				}

				while((c = fgetc(inputFile)) != EOF) {
					strncat(ptxt, &c, 1);
				}
		
				fclose(inputFile);
				
				printf("\nContent of input file : %s", ptxt);
				
			}
			else {
				printf("\nError: inout file does not exist\n");
				exit(0);
			}
		}
		// if cmd line is -o then the next cmd line should be output file
		else if (strcmp( "-o" , argv[cmd] ) == 0) {
			if (argv[cmd + 1] != NULL) {
				outputStr = argv[cmd + 1];

				printf("\nThe File for Encryption output: %s\n", outputStr);
			}
			else {
				printf("\nError: output file does not exist\n");
				exit(0);
			}
		}
		cmd++;
	}

	if(ptxt == NULL) {
		printf("\nThere is no such a input file.\n");
		exit(1);
	} 
	if(outputStr == NULL) {
		printf("\nThere is no such a output file.\n");
		exit(0);
	}
	
	len_ptxt = strlen(ptxt) + 1;	
	ctxt_len = len_ptxt + 8;

	if(ks == NULL) {
        printf("\nError: There is no key\n");
        exit(0);
    } 
	else {

		// start the encryption
	
		EVP_EncryptInit_ex(&ctx, EVP_des_ecb(), NULL, ks, NULL);
		
		unsigned char *ctxt = malloc(ctxt_len);
		
		int *len = &len_ptxt;
		
		EVP_EncryptUpdate(&ctx, ctxt, &ctxt_len, (unsigned char *)ptxt, *len);
		
		int num;
		EVP_EncryptFinal_ex(&ctx, ctxt+ctxt_len, &num);

		len_ptxt = *len;
		len_ptxt = ctxt_len + num;		

		outputFile = fopen(outputStr, "wb");
		
		i = 0;
		while (i < len_ptxt) {
			//fprintf(outputFile, "%02x", ctxt[i++]);
			fputc(ctxt[i++], outputFile);
		}
		fclose(outputFile);
	
		EVP_CIPHER_CTX_cleanup(&ctx);

		free(ctxt);
		
	}
	
	return 0;
}
