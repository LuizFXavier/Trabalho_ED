#include <stdio.h>
#include <stdlib.h>
#include "jsense.h"
#include "hash.h"

typedef struct{
    char* codigo_ibge;
    char * nome;
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char * fuso_horario;
}municipio;

char* get_key(const void* mun){

    municipio* m = (municipio*)mun;

    return m->codigo_ibge;
}

int main(){

    t_hash * h = criaHash(100 * 200, get_key);

    char itens[9][18];

    JSENSE *j = jse_from_file("test/municipios.json");

    int error;

    for(int i = 0; i < 5570; i++){ // Leitura do JSON
    
        municipio* m = malloc(sizeof(municipio));

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
        
        insereHash(h, m);
    }

    free(j);
    
    char cod[8];
    printf("Insira o código IBGE desejado: ");
    scanf("%s",cod);

    municipio* jk = (municipio*)buscaHash(h, cod);

    if(!jk){
        printf("Cidade não encontrada!\n");
    }
    else{
        printf("\nCódigo IBGE: %s\n", jk->codigo_ibge);
        printf("Nome: %s\n", jk->nome);
        printf("Código UF: %d\n", jk->codigo_uf);
        printf("DDD: %d\n", jk->ddd);
        printf("Capital: %d\n", jk->capital);
        printf("Latitude: %lf\n", jk->latitude);
        printf("Longitude: %lf\n", jk->longitude);
        printf("Siafi ID: %d\n", jk->siafi_id);
        printf("Fuso horário: %s\n", jk->fuso_horario);

    }


    apagaHash(h);

    return 0;
}