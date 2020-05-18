#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <openssl/evp.h>

#define START 0
#define END 61

char *p1 = "4a5c2d660232375d25dc141febdaae056ba05e95fe606e88a350929a36a9ea67",
     *p2 = "6f32ebbc1ee9cf3867df5f86f071ee147c6190ac7bfd88330fd8996a0abb512e",
     *p3 = "33c35f8c8515b13ce15324718eccea7fb10e0c8848df3e3e0a7c0e529303828d",
     *p4 = "dc348085d14fefa692adf1e7d97e2d59253c01189359873186d376ebe0f3ad3a";

char pool[62] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

char *convert_hash_to_string(unsigned char *hash)
{
    char *result = malloc(sizeof(char) * 65);
    char *temp = malloc(sizeof(char) * 2);
    for (int i = 0; i < 32; i++)
    {
        sprintf(temp, "%02x", hash[i]);
        strcat(result, temp);
    }
    result[64] = 0;
    free(temp);
    return result;
}

int check_pass(char *hash_string)
{
    if (strcmp(hash_string, p1) == 0)
        return 1;
    if (strcmp(hash_string, p2) == 0)
        return 2;
    if (strcmp(hash_string, p3) == 0)
        return 3;
    if (strcmp(hash_string, p4) == 0)
        return 4;
    return 0;
}

void find_pass()
{
    int a, b, c, d;
    unsigned char *digest;
    char *hash_string;
    char *password = malloc(sizeof(char) * 4);
    const EVP_MD *md = EVP_get_digestbyname("sha256");
    EVP_MD_CTX *mdctx;

    for (a = START; a <= END; a++)
    {
        for (b = START; b <= END; b++)
        {
            for (c = START; c <= END; c++)
            {
                for (d = START; d <= END; d++)
                {
                    sprintf(password, "%c%c%c%c", pool[a], pool[b], pool[c], pool[d]);
                    unsigned char *md_value = malloc(sizeof(unsigned char) * EVP_MAX_MD_SIZE);
                    unsigned int md_len;
                    mdctx = EVP_MD_CTX_new();
                    // Set up digest context to use a digest type
                    EVP_DigestInit_ex(mdctx, md, NULL);
                    // Hash data into the digest context
                    EVP_DigestUpdate(mdctx, password, strlen(password));
                    // Retrieve the digest value from context and store it in "md_value"
                    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
                    EVP_MD_CTX_free(mdctx);

                    char *hash_string = convert_hash_to_string(md_value);
                    if (check_pass(hash_string))
                    {
                        printf("Pass: %s - hash: %s\n", password, hash_string);
                    }
                    free(password);
                    free(md_value);
                    free(hash_string);
                }
            }
        }
    }
}

void omp_find_pass()
{
    int a, b, c, d;
    const EVP_MD *md = EVP_get_digestbyname("sha256");

#pragma omp parallel private(a, b, c, d)
    {
#pragma omp for
        for (a = START; a <= END; a++)
        {
            for (b = START; b <= END; b++)
            {
                for (c = START; c <= END; c++)
                {
                    for (d = START; d <= END; d++)
                    {
                        char *password = malloc(sizeof(char) * 4);
                        sprintf(password, "%c%c%c%c", pool[a], pool[b], pool[c], pool[d]);
                        EVP_MD_CTX *mdctx;
                        unsigned char *md_value = malloc(sizeof(unsigned char) * EVP_MAX_MD_SIZE);
                        unsigned int md_len;
                        mdctx = EVP_MD_CTX_new();
                        // Set up digest context to use a digest type
                        EVP_DigestInit_ex(mdctx, md, NULL);
                        // Hash data into the digest context
                        EVP_DigestUpdate(mdctx, password, strlen(password));
                        // Retrieve the digest value from context and store it in "md_value"
                        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
                        EVP_MD_CTX_free(mdctx);

                        char *hash_string = convert_hash_to_string(md_value);
                        if (check_pass(hash_string))
                        {
                            printf("Pass: %s - hash: %s\n", password, hash_string);
                        }
                        free(password);
                        free(md_value);
                        free(hash_string);
                    }
    }
}

int main(int argc, char const *argv[])
{
    clock_t begin, end;
    omp_set_num_threads(4);

    begin = clock();
    find_pass();
    end = clock();
    printf("Normal time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);

    begin = clock();
    omp_find_pass();
    end = clock();
    printf("OpenMP time taken: \t%f\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
