#include <stdlib.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <string.h>
#include <stdint.h>
#include <openssl/evp.h>

// helper function to read the file
unsigned char *readFile( char mode[] , char name[])
{
	// initializing the valraibles
	FILE *inputfile = NULL;
	unsigned  char *inputs;
	int i = 0, ch;	 
	
	// open the file with the given mode 
	inputfile = fopen(name, mode);
	 
	// check for null
	if (inputfile == NULL)
	{
		printf("Error: file does not exit\n");
		exit(0);
	}

	// allocate memory
	inputs = malloc(1000);

	while ( (ch = fgetc(inputfile)) != EOF )
	{
		inputs[i] = (unsigned char)ch;
		i++;
	}
	inputs[i] = '\0';

	fclose(inputfile);

	return inputs;
}

int main(int argc, char *argv[])
{
	// initilize the varaibles
	unsigned char *ptxt = NULL;
	char ks[100];
	char *inputStr;
	char *outputStr;
	FILE* keyfile = NULL;
	FILE* inputFile = NULL;
	FILE* outputFile = NULL;
	int i = 0, index = 0, cmd = 0;
	int len_ptxt, ch;
	int ctxt_len, key_len;
	char c;

	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);	

	// check the command line
  	while (cmd < argc){
	
		// if the commandline is -k then the next command line should be key file
		if (strcmp( "-k" , argv[cmd] ) == 0) {	

			if (argv[cmd + 1] != NULL) {

				// open key file
				keyfile = fopen(argv[cmd+1], "r");
				// check for null
				if (keyfile == NULL) {
					printf("Error: ks file does not exist.\n");
					exit(0);
				}
				// copy the file content into array key
				while((c = fgetc(keyfile)) != EOF) {
					strncat(ks, &c, 1);
				}

				// close the file
				fclose(keyfile);

				printf("key: %s\n", ks);
				//printf("key in hex %02x\n", ks);
				
				// if the length of key is less than 8 then exit the porgram
				key_len = strlen(ks) -1;
				if(key_len < 8) {
					printf("Error: Key MUST be 8 hexadecimal characters\n");
					exit(0);
				}	
			}
			else {
				printf("Error: ks file does not exist\n");
				exit(0);
			}
		}
		
		else if (strcmp( "-i" , argv[cmd] ) == 0) {
			if (argv[cmd + 1] != NULL) {
				
				inputStr = argv[cmd+1];
				printf("\nCipher text file: %s\n", inputStr);

			}

				/*
				inputFile = fopen(argv[cmd+1], "rb");
				
				// check for null
				if (inputFile == NULL) {
					printf("Error: input file does not exist\n");
					exit(0);
				}

				while((c = fgetc(inputFile)) != EOF) {
					strncat(ptxt, &c, 1);
				}
		
				fclose(inputFile);
				*/
				//printf("input file : %s", ptxt);
				
			else {
				printf("Error: inout file does not exist\n");
				exit(0);
			}
		}
		// if commandline is -o then the next command line should be output file
		else if (strcmp( "-o" , argv[cmd] ) == 0) {
			if (argv[cmd + 1] != NULL) {
				outputStr = argv[cmd + 1];
				printf("\nPlain text file: %s\n", outputStr);
	
			}
			else {
				printf("Error: output file does not exist\n");
				exit(0);
			}
		}
		cmd++;
	}
	
	ptxt = readFile("rb", inputStr);
	
	/*
	ptxt = malloc(1000);

	inputFile = (inputStr, "rb");
	while((ch = fgetc(inputFile)) == EOF) {
		ptxt[index++] = (unsigned char)ch;
	}
	ptxt[index] = '\0';
	fclose(inputFile);
	*/

	if(ptxt == NULL) {
		printf("There is no such a input file.\n");
		exit(1);
	}
	 
	if(outputStr == NULL) {
		printf("There is no such a output file.\n");
		exit(0);
	}
	
	//printf("\nlength of plan text = %d\n", len_ptxt);

	if(ks == NULL) {
        printf("Error: There is no key\n");
        exit(0);
    } 
	else {
		
            
    	//printf("\nlength of plan text = %d\n", len_ptxt);

		len_ptxt = 984;
		ctxt_len = len_ptxt; 

		// starting decryption
		EVP_DecryptInit_ex(&ctx, EVP_des_ecb(), NULL, ks, NULL);	
	
		int *len = &len_ptxt;
		char *ctxt = malloc(ctxt_len + 1);
		
		//printf("len_ptxt: %d\n", len_ptxt);
		
		EVP_DecryptUpdate(&ctx, (char *)ctxt, &ctxt_len, ptxt, *len);
		
		int num;

		EVP_DecryptFinal_ex(&ctx, (char *)ctxt+ctxt_len, &num);

		len_ptxt = *len;
		len_ptxt = ctxt_len + num;		
		
	
		//ctxt = des_ecb_dec(ptxt, &len_ptxt, &ctx);
		outputFile = fopen(outputStr, "wb");	
	
		i = 0;
		while (i < len_ptxt) {
			//fprintf(outputFile, "%s", ctxt[i]);
			fputc(ctxt[i], outputFile);
			i++;
		}
		fclose(outputFile);
		
	
		EVP_CIPHER_CTX_cleanup(&ctx);

		free(ctxt);
		
	}
	
	return 0;
}
