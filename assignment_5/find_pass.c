#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <openssl/evp.h>

char *p1 = "4a5c2d660232375d25dc141febdaae056ba05e95fe606e88a350929a36a9ea67",
     *p2 = "6f32ebbc1ee9cf3867df5f86f071ee147c6190ac7bfd88330fd8996a0abb512e",
     *p3 = "33c35f8c8515b13ce15324718eccea7fb10e0c8848df3e3e0a7c0e529303828d",
     *p4 = "dc348085d14fefa692adf1e7d97e2d59253c01189359873186d376ebe0f3ad3a";

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
    char a, b, c, d, start = 'a', end = 'z';
    unsigned char *digest;
    char *hash_string;
    char *password = malloc(sizeof(char) * 4);
    const EVP_MD *md = EVP_get_digestbyname("sha256");
    EVP_MD_CTX *mdctx;

    for (a = start; a <= end; a++)
    {
        for (b = start; b <= end; b++)
        {
            for (c = start; c <= end; c++)
            {
                for (d = start; d <= end; d++)
                {
                    sprintf(password, "%c%c%c%c", a, b, c, d);
                    unsigned char md_value[EVP_MAX_MD_SIZE];
                    unsigned int md_len;
                    mdctx = EVP_MD_CTX_new();
                    EVP_DigestInit_ex(mdctx, md, NULL);
                    EVP_DigestUpdate(mdctx, password, strlen(password));
                    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
                    EVP_MD_CTX_free(mdctx);

                    char *hash_string = convert_hash_to_string(md_value);
                    if (check_pass(hash_string))
                    {
                        printf("Pass: %s - hash: %s\n", password, hash_string);
                    }
                }
            }
        }
    }
}

void omp_find_pass()
{
    char a, b, c, d, start = 'a', end = 'z';
    const EVP_MD *md = EVP_get_digestbyname("sha256");

#pragma omp parallel private(a, b, c, d)
    {
#pragma omp for
        for (a = start; a <= end; a++)
        {
            for (b = start; b <= end; b++)
            {
                for (c = start; c <= end; c++)
                {
                    for (d = start; d <= end; d++)
                    {
                        char *password = malloc(sizeof(char) * 4);
                        sprintf(password, "%c%c%c%c", a, b, c, d);
                        EVP_MD_CTX *mdctx;
                        unsigned char md_value[EVP_MAX_MD_SIZE];
                        unsigned int md_len;
                        mdctx = EVP_MD_CTX_new();
                        EVP_DigestInit_ex(mdctx, md, NULL);
                        EVP_DigestUpdate(mdctx, password, strlen(password));
                        EVP_DigestFinal_ex(mdctx, md_value, &md_len);
                        EVP_MD_CTX_free(mdctx);

                        char *hash_string = convert_hash_to_string(md_value);
                        if (check_pass(hash_string))
                        {
                            printf("Pass: %s - hash: %s\n", password, hash_string);
                        }
                    }
                }
            }
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
