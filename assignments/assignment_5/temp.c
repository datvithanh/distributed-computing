#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <openssl/sha.h>
#include <stdbool.h>

typedef unsigned char byte;

int matches(byte *a, byte *b)
{
    for (int i = 0; i < 32; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

byte *StringHashToByteArray(const char *s)
{
    byte *hash = (byte *)malloc(32);
    char two[3];
    two[2] = 0;
    for (int i = 0; i < 32; i++)
    {
        two[0] = s[i * 2];
        two[1] = s[i * 2 + 1];
        hash[i] = (byte)strtol(two, 0, 16);
    }
    return hash;
}

void printResult(byte *password, byte *hash)
{
    char sPass[5];
    memcpy(sPass, password, 4);
    sPass[4] = 0;
    printf("%s => ", sPass);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        printf("%02x", hash[i]);
    printf("\n");
}

int main(int argc, char **argv)
{
    byte x,y,z,t;

    byte *one = StringHashToByteArray("4a5c2d660232375d25dc141febdaae056ba05e95fe606e88a350929a36a9ea67");
    byte *two = StringHashToByteArray("6f32ebbc1ee9cf3867df5f86f071ee147c6190ac7bfd88330fd8996a0abb512e");
    byte *three = StringHashToByteArray("33c35f8c8515b13ce15324718eccea7fb10e0c8848df3e3e0a7c0e529303828d");
    byte *four = StringHashToByteArray("dc348085d14fefa692adf1e7d97e2d59253c01189359873186d376ebe0f3ad3a");
    bool findOne = false, findTwo = false, findThree = false, findFour = false;

    {
        #pragma omp parallel for \
	    shared(findOne, findTwo, findThree, findFour, one, two, three, four) \
	    private(x, y, z, t)
        for (x = 0; x < 123; x++)
        {
            for (y = 0; y < 123; y++)
                for (z = 0; z < 123; z++)
                    for (t = 0; t < 123; t++)
                    {
			byte password[4] = {x, y, z, t};
			byte *hash = (byte *) malloc(32);
                        hash = SHA256(password, 4, hash);

			if (!findOne && matches(one, hash)) {
			    printResult(password, hash);
			    findOne = true;
			}
			else if (!findTwo && matches(two, hash)) {
			    printResult(password, hash);
			    findTwo = true;
			}
			else if (!findThree && matches(three, hash)) {
			    printResult(password, hash);
			    findThree = true;
			}
			else if (!findFour && matches(four, hash)) {
			    printResult(password, hash);
			    findFour = true;
			}

			free(hash);
			if (findOne && findTwo && findThree && findFour) {
                            break;
			}
                    }
        }
    }

    free(one);
    free(two);
    free(three);
    free(four);

    return 0;
}
