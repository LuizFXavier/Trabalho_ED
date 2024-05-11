#include <stdio.h>
#include <stdlib.h>
#include "jsense.h"
#include "cidade.h"
#include "kd-tree.h"
#include "hash.h"

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

void inOrdem(t_no* atual){

    if(!atual){
        return;
    }

    if(atual->esq){
        inOrdem(atual->esq);
    }

    if(atual->dir){
        inOrdem(atual->dir);
    }
    printf("%.1lf, %.1lf\n", atual->reg->latitude, atual->reg->longitude);
}

char* get_key(const void* mun){

    cidade* m = (cidade*)mun;

    return m->codigo_ibge;
}

int main(){

    t_arvore *arv = cria_arvore(2, comparador);
    t_hash *hash = criaHash(100 * 100, get_key);
    
    char itens[9][18];

    JSENSE *j = jse_from_file("test/municipios.json");

    int error;

    for(int i = 0; i < 5570; i++){ // Leitura do JSON
    
        cidade* m = malloc(sizeof(cidade));

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

        m->latitude = tec_string_to_double(jse_get(j, itens[2]), &error);
        m->longitude = tec_string_to_double(jse_get(j, itens[3]), &error);

        m->capital = tec_string_to_int(jse_get(j, itens[4]));
        m->codigo_uf = tec_string_to_int(jse_get(j, itens[5]));

        m->siafi_id = tec_string_to_int(jse_get(j, itens[6]));
        m->ddd = tec_string_to_int(jse_get(j, itens[7]));
        m->fuso_horario = jse_get(j, itens[8]);
        
        insereHash(hash, m);
        kd_insere(arv, m);
    }

    free(j);

    int p = 0;

    char cod[8];
    printf("Insira o código IBGE desejado: ");
    scanf("%s",cod);

    int n;
    printf("insira o número de cidades desejado: ");
    scanf("%d", &n);

    cidade* c = (cidade*)buscaHash(hash, cod);

    cidade** cities = maisProximas(arv, c, n);


    while ((*cities) != NULL)
    {
        printf("%s\n", (*cities)->codigo_ibge);
        cities++;
    }

    apagaHash(hash);

    return EXIT_SUCCESS;
}

