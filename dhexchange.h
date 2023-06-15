#ifndef DIFFIE_HELLMAN_EXCHANGE_H
#define DIFFIE_HELLMAN_EXCHANGE_H

#include <inttypes.h>

#define DH_KEY_LENGTH	(16)

typedef unsigned char uint8_t;

typedef unsigned char DH_KEY[DH_KEY_LENGTH];

/*--------------------------------------------------------------------------*/
typedef union _uint128_t {
	struct {
		uint64_t low;
		uint64_t high;
	};
	unsigned char byte[DH_KEY_LENGTH];
} uint128_t;

void setPrime(uint128_t prime);
void setInversePrime(uint128_t iprime);

void DH_generate_key_pair(DH_KEY public_key, DH_KEY private_key, const uint8_t* key, const int len);
void DH_generate_key_secret(DH_KEY secret_key, const DH_KEY my_private, const DH_KEY another_public);

#endif
