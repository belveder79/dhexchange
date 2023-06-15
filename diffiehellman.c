#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include "dhexchange.h"
#include <assert.h>

/*--------------------------------------------------------------------------*/
static void
_print_key(const char* name, const DH_KEY key) {
	int i;

	printf("\"%s\": \"",name);
	for (i = DH_KEY_LENGTH-1; i>=0; i--) {
		printf("%02x", key[i]);
	}
	printf("\"");
}

void stringToHex(unsigned char* hexString, const char* string, int stringLength)
{
    int uChar = 0;
    int y = DH_KEY_LENGTH - 1;
    for (int x = 0; x < stringLength; x+=2, y--)
    {
        sscanf(&string[x], "%02x", &uChar);
        hexString[y] = (unsigned char)uChar;
    }
}

/*--------------------------------------------------------------------------*/
int 
main(int argc, char* argv[])
{
	uint8_t getSecret = 0;
	uint8_t prime[16]; int len = 0;
	// parse prime
	if(argc > 1) {
		const char* primestr = argv[1];
		len = strlen(primestr);
		if(len != 32)
		{
			printf("Error! Prime size mismatch!\n");
			return -1;
		}
		stringToHex((unsigned char*)(&prime[0]), primestr, len);
		
		uint64_t* low = (uint64_t*)&prime[0];
		uint64_t* high = (uint64_t*)&prime[8]; 
		uint128_t newP = { *low, *high };
		uint128_t newIprime = {0xFFFFFFFFFFFFFFFF-*low + 1, 0xFFFFFFFFFFFFFFFF-*high };
		setPrime(newP); setInversePrime(newIprime);
	}

	DH_KEY alice_private, alice_public;
	DH_KEY bob_public;
	DH_KEY alice_secret;

	// parse public and private, and public of other
	if(argc > 4) 
	{
		getSecret = 1;
		const char* privatestr = argv[2];
		len = strlen(privatestr);
		if(len != 32)
		{
			printf("Error! Private size mismatch!\n");
			return -1;
		}
		stringToHex((unsigned char*)(&alice_private[0]), privatestr, len);

		const char* publicstr = argv[3];
		len = strlen(publicstr);
		if(len != 32)
		{
			printf("Error! Public size mismatch!\n");
			return -1;
		}
		stringToHex((unsigned char*)(&alice_public[0]), publicstr, len);

		const char* otherpublicstr = argv[4];
		len = strlen(otherpublicstr);
		if(len != 32)
		{
			printf("Error! Other Public size mismatch!\n");
			return -1;
		}
		stringToHex((unsigned char*)(&bob_public[0]), otherpublicstr, len);

		DH_generate_key_secret(alice_secret, alice_private, bob_public);
	}
	else
	{
		DH_generate_key_pair(alice_public, alice_private, NULL, 0);
	}

	time_t seed;
	time(&seed);
	srand((unsigned int)seed);

	printf("{");
	_print_key("alice_private", alice_private); printf(",");
	_print_key("alice_public", alice_public);	
	// _print_key("bob_private", bob_private)
	if(getSecret) {
		printf(",");
		_print_key("bob_public", bob_public); printf(",");
		_print_key("alice_secret", alice_secret);
	}
	printf("}");

	return 0;
}
