#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"
#define SEED 0x123456

// "Construtor" da hash
t_hash* criaHash(int tam, char *(*get_key)(const void*)){
    t_hash* nova = (t_hash*)malloc(sizeof(t_hash));

    nova->get_key = get_key;
    nova->max = tam;
    nova->size = 0;
    nova->deletado = (void*) &nova->max;
    nova->tabela = (void**) calloc(tam, sizeof(void*));

    return nova;
}

void apagaHash(t_hash* hash){
    if (hash == NULL)
        return;
    
    for(int i = 0; i < hash->max; i++){
        if(hash->tabela[i] != hash->deletado)
            free((hash->tabela[i])); 
    }
    free(hash->tabela);
    free(hash);
}

int insereHash(t_hash* hash, void * bucket){
    uint32_t n = hash_f(hash->get_key(bucket), SEED);
    uint32_t k = hash_f2(hash->get_key(bucket), SEED);

    int pos = n % hash->max;

    //Falha se a hash estiver cheia
    if(hash->size +1 == hash->max){
        free(bucket);
        return EXIT_FAILURE;
    }

    //Busca de posição livre, utilizando hash duplo para o tratamento de colisões

    for(int t = 0; hash->tabela[pos] != hash->deletado && hash->tabela[pos]; t++){
        
        pos = (n + t * k) % hash->max;
    }

    hash->tabela[pos] = (void*)bucket;
    hash->size += 1;

    return EXIT_SUCCESS;
}
int removeHash(t_hash *hash, const char* key){

    uint32_t n = hash_f(key, SEED);
    uint32_t k = hash_f2(key, SEED);

    int pos = n % hash->max;
    int t = 0;

    while (hash->tabela[pos] != 0)
    {
        if(!strcmp(hash->get_key((char*)hash->tabela[pos]), key)){
            hash->tabela[pos] = hash->deletado;

            return EXIT_SUCCESS;
        }
        t+=1;
        pos = (n + t * k) % hash->max;
    }
    return EXIT_FAILURE;
}

void * buscaHash(t_hash *hash, const char* key){
    uint32_t n = hash_f(key, SEED);
    uint32_t k = hash_f2(key, SEED);

    int pos = n % hash->max;
    int t = 0;

    while (hash->tabela[pos] != 0)
    {
        if(hash->tabela[pos] == hash->deletado){

        }
        else if(!strcmp(hash->get_key((char*)hash->tabela[pos]), key)){
            return (void *)(hash->tabela[pos]);
        }
        t+=1;
        pos = (n + t * k) % hash->max;
    }

    return NULL;
}

// Funcão de hashing primária
uint32_t hash_f(const char* str, uint32_t h){
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

// Funcão de re-hashing
uint32_t hash_f2(const char* str, uint32_t h){
    for(;*str; ++str){
        h = ((h << 5) + h) + *str;
    }
    return h;    
}