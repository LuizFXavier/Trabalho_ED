#pragma once
#include <stdint.h>

typedef struct balde
{
    void * reg;
    struct balde* prox;

}t_bucket;

typedef struct hash_LL
{
    t_bucket** tabela;
    int size;
    int max;
    void* deletado;
    char *(*get_key)(const void*);

}t_hash_LL;

t_hash_LL* criaHash_LL(int tam, char *(*get_key)(const void*));
int insereHash_LL(t_hash_LL* hash, void * bucket);
void apagaHash_LL(t_hash_LL* hash);
void apagaBuckets(t_bucket* bucket);
int removeHash_LL(t_hash_LL *hash, const char* key);
void ** buscaHash_LL(t_hash_LL *hash, const char* key);
uint32_t hash_f_LL(const char* str, uint32_t h);
uint32_t hash_f2_LL(const char* str, uint32_t h);