#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash_LL.h"
#define SEED 0x123456

// "Construtor" da hash por lista ligada
t_hash_LL *criaHash_LL(int tam, char *(*get_key)(const void *))
{
    t_hash_LL* nova = (t_hash_LL*)malloc(sizeof(t_hash_LL));

    nova->get_key = get_key;
    nova->max = tam;
    nova->size = 0;
    nova->deletado = (void*) &(nova->max);
    nova->tabela = (t_bucket**) calloc(tam, sizeof(t_bucket*));

    return nova;
}

// Função para liberação de memória de toda a hash
void apagaHash_LL(t_hash_LL* hash){
    if (hash == NULL)
        return;
    
    for(int i = 0; i < hash->max; i++){
         if(*(hash->tabela + i)){
            apagaBuckets(*(hash->tabela + i));
         }
    }
    free(hash->tabela);
    free(hash);
}

// Liberação em cadeia da memória dos buckets
void apagaBuckets(t_bucket* bucket){
    
    if(bucket->prox){
        apagaBuckets(bucket->prox);
    }
    free(bucket->reg);
    free(bucket);
}

int insereHash_LL(t_hash_LL* hash, void * reg){
    uint32_t n = hash_f_LL(hash->get_key(reg), SEED);

    int pos = n % hash->max;

    if(hash->size +1 == hash->max){
        free(reg);
        return EXIT_FAILURE;
    }
    t_bucket* atual = *(hash->tabela + pos);

    // Inserção na lista em posição não ocupada
    if(atual == 0){
        
        t_bucket* new_bucket = (t_bucket*) malloc(sizeof (t_bucket));

        new_bucket->reg = reg;
        new_bucket->prox = NULL;

        hash->tabela[pos] = new_bucket;
        hash->size += 1;

        return EXIT_SUCCESS;
    }

    // Inserção em posição já ocupada, por meio de lista encadeada
    
    while (atual->prox)
    {
        atual = atual->prox;
    }
    t_bucket* new_bucket = (t_bucket*) malloc(sizeof (t_bucket));

    atual->prox = new_bucket;

    new_bucket->reg = reg;
    new_bucket->prox = NULL;

    return EXIT_SUCCESS;
}

void **buscaHash_LL(t_hash_LL *hash, const char *key){

    uint32_t n = hash_f_LL(key, SEED);
    int pos = n % hash->max;

    t_bucket* atual = *(hash->tabela + pos);

    void ** retorno = calloc(10, sizeof(void*)); //Não vai ter 10 cidades com exatamente o mesmo nome
    int t = 0;

    // Busca até o final da lista encaadeada para nomes iguais
    while (atual)
    {
        if(!strcmp(hash->get_key(atual->reg), key)){
            retorno[t] = atual->reg;
            t++;
        }
        atual = atual->prox;
    }

    // Retorno de todas as cidades com mesmo nome
    if(t != 0)
        return retorno;

    return NULL;
}

uint32_t hash_f_LL(const char *str, uint32_t h)
{
    for (; *str; ++str) {
        h ^= *str;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}