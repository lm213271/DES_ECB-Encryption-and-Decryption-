
all:
	gcc DES_ECB_Enc.c -o DES_ECB_Enc -lssl -lcrypto
	gcc DES_ECB_Dec.c -o DES_ECB_Dec -lssl -lcrypto
clean:
	rm -rf DES_ECB_Enc
	rm -rf DES_ECB_Dec
