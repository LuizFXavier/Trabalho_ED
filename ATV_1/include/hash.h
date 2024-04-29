#pragma once
#include <stdint.h>

typedef struct hash
{
    void** tabela;
    int size;
    int max;
    void* deletado;
    char *(*get_key)(const void*);
}t_hash;

t_hash* criaHash(int tam, char *(*get_key)(const void*));
int insereHash(t_hash* hash, void * bucket);
void apagaHash(t_hash* hash);
int removeHash(t_hash *hash, const char* key);
void * buscaHash(t_hash *hash, const char* key);
uint32_t hash_f(const char* str, uint32_t h);
uint32_t hash_f2(const char* str, uint32_t h);