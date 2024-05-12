#include <stdio.h>
#include <stdlib.h>
#include "jsense.h"
#include "cidade.h"
#include "kd-tree.h"
#include "hash.h"
#include "hash_LL.h"

#define NUM_CIDADES 5570

// Inicialização de funções
double comparador(const void* c1, const void* c2, int k);
char* get_key1(const void* mun);
char* get_key2(const void* mun);

int main(){

    //Inicialização das estruturas de dados

    t_arvore *arv = cria_arvore(2, comparador);
    t_hash *hash1 = criaHash(100 * 100, get_key1);
    t_hash_LL *hashLL = criaHash_LL(100 * 100, get_key2);

    //Inicialização de variáveis para leitura do arquivo JSON
    
    char itens[9][18];

    JSENSE *j = jse_from_file("test/municipios.json");

    int error;

    // Leitura do JSON

    for(int i = 0; i < NUM_CIDADES; i++){
    
        cidade* m = (cidade*) malloc(sizeof(cidade));
        cidade_Res* res = (cidade_Res*) malloc(sizeof(cidade_Res));

        sprintf(itens[0], "[%d].codigo_ibge",i);
        sprintf(itens[1], "[%d].nome",i);
        sprintf(itens[2], "[%d].latitude",i);
        sprintf(itens[3], "[%d].longitude",i);
        sprintf(itens[4], "[%d].capital",i);
        sprintf(itens[5], "[%d].codigo_uf",i);
        sprintf(itens[6], "[%d].siafi_id",i);
        sprintf(itens[7], "[%d].ddd",i);
        sprintf(itens[8], "[%d].fuso_horario",i);

        m->codigo_ibge = jse_get(j, itens[0]);
        m->nome = jse_get(j, itens[1]);

        res->codigo_ibge = jse_get(j, itens[0]);
        res->nome = jse_get(j, itens[1]);
        res->ddd = tec_string_to_int(jse_get(j, itens[7]));

        m->latitude = tec_string_to_double(jse_get(j, itens[2]), &error);
        m->longitude = tec_string_to_double(jse_get(j, itens[3]), &error);

        m->capital = tec_string_to_int(jse_get(j, itens[4]));
        m->codigo_uf = tec_string_to_int(jse_get(j, itens[5]));

        m->siafi_id = tec_string_to_int(jse_get(j, itens[6]));
        m->ddd = tec_string_to_int(jse_get(j, itens[7]));
        m->fuso_horario = jse_get(j, itens[8]);
        
        insereHash(hash1, m);
        insereHash_LL(hashLL, res);
        kd_insere(arv, m);
    }

    //Leitura de nome da cidade
    char nomeC[100];
    printf("Insira o nome da cidade: ");
    fgets(nomeC, 100, stdin);
    
    if ((strlen(nomeC) > 0) && (nomeC[strlen (nomeC) - 1] == '\n'))
        nomeC[strlen (nomeC) - 1] = '\0';


    //Busca de cidades por nome
    cidade_Res** cidRes = (cidade_Res**) buscaHash_LL(hashLL, nomeC);
    
    //Contagem de cidades encontradas
    int t = 0;

    while (*(cidRes + t)) 
    {
        t++;
    }

    if(t == 0){
        printf("Cidade não encontrada!\n");
        return EXIT_FAILURE;
    }

    //Seleção de opção quando há cidades com mesmo nome
    int opt = 0;
    if(t > 1){
        for(int i = 0; i < t; i++){
            printf("%d) %s %d\n", i, (*(cidRes + i))->nome, (*(cidRes + i))->ddd);
        }
        char c[3];
        printf("Escolha entre essas cidades: ");

        fgets(c,3,stdin);
        opt = strtol(c, NULL, 0);

        if(opt < 0 || opt > t-1){

            printf("Número inválido escolhido!\n");
            return EXIT_FAILURE;
        }
    }

    // Especificação do número de cidades 
    int n;
    char c[3];

    printf("Número de cidades: ");
    fgets(c,3,stdin);
    n = strtol(c, NULL, 0);

    if(n < 1 ){
        printf("Número inválido escolhido!\n");
            return EXIT_FAILURE;
    }

    // Procura das N cidades mais próximas 
    cidade* cid = (cidade*) buscaHash(hash1, cidRes[opt]->codigo_ibge);

    cidade** cities = maisProximas(arv, cid, n);

    printf("\n");


    // Impressão das N cidades encontradas
    while ((*cities) != NULL)
    {
        printf("Nome: %s\n", (*cities)->nome);
        printf("Código IBGE: %s\n", (*cities)->codigo_ibge);
        printf("DDD: %d\n", (*cities)->ddd);
        printf("Capital: %d\n", (*cities)->capital);
        printf("Longitude: %lf\n", (*cities)->longitude);
        printf("Latitude: %lf\n", (*cities)->latitude);
        printf("Código UF: %d\n", (*cities)->codigo_uf);
        printf("Siafi ID: %d\n", (*cities)->siafi_id);
        printf("Fuso horário: %s\n\n", (*cities)->fuso_horario);
        cities++;
    }

    // Liberação de memória
    free(j);
    apagaHash(hash1);
    apagaHash_LL(hashLL);
    KD_destroi(arv);

    return EXIT_SUCCESS;
}


// Comparador da KD-tree
double comparador(const void* c1, const void* c2, int k){

    switch (k)
    {
    case 0:
        return ((cidade*)c1)->longitude - ((cidade*)c2)->longitude;
        break;
    
    case 1:
        return ((cidade*)c1)->latitude - ((cidade*)c2)->latitude;
        break;

    default:
        printf("Dimensão inválida no comparador");
        break;
    }

}

// Função utilizada na Hash de hashing duplo
char* get_key1(const void* mun){

    cidade* m = (cidade*)mun;

    return m->codigo_ibge;
}

// Função utilizada na Hash de lista ligada
char* get_key2(const void* mun){

    cidade_Res* m = (cidade_Res*)mun;

    return m->nome;
}