#include <stdlib.h>
#include <stdio.h>
#include "kd-tree.h"

t_arvore* cria_arvore(int dim, double (*cmp)(const void*, const void*, int)){
    t_arvore* nova = (t_arvore*) malloc(sizeof(t_arvore));

    nova->dim = dim;
    nova->cmp = cmp;
    nova->raiz = NULL;
    return nova;
}

int kd_insere(t_arvore* arv, cidade* reg){
    
    if(!arv->raiz){
        arv->raiz = (t_no*)malloc(sizeof(t_no));
        arv->raiz->reg = reg;
        return EXIT_SUCCESS;
    }

    t_no* no_atual = arv->raiz;
    int k = 0;

    while (no_atual)
    {
        if(arv->cmp(no_atual->reg, reg, k % arv->dim) < 0){
            
            if(no_atual->dir == NULL){
                no_atual->dir = (t_no*)malloc(sizeof(t_no));
                no_atual->dir->reg = reg;
                return EXIT_SUCCESS;
            }
            no_atual = no_atual->dir;
        }
        else{
            if(no_atual->esq == NULL){
                no_atual->esq = (t_no*)malloc(sizeof(t_no));
                no_atual->esq->reg = reg;
                return EXIT_SUCCESS;
            }
            no_atual = no_atual->esq;
        }

        k++;
    }
    return EXIT_FAILURE;
}

cidade** maisProximas(t_arvore* arv, cidade* buscada, int n){
    double distancias[n+1];
    cidade** cMaisProximas = (cidade**) calloc(n+1, sizeof(cidade*));

    for(int i = 0; i <= n; i++){
        distancias[i] = INFINITO;
        cMaisProximas[i] = 0;
    }
    int v = 0;
    int *p = &v;

    procuraProximas(arv, arv->raiz, buscada, 0, p, n, distancias, cMaisProximas);

    return cMaisProximas;
}

void procuraProximas(t_arvore* arv, t_no* no_atual, cidade* buscada, int k, int* tam, int n, double* distancias, cidade** cidadesProximas){
    
    if(!no_atual){
        //printf("\nChegou na folha\n");
        return;
    }
    double dist = distancia(no_atual->reg, buscada);
    
    if(dist != 0 && dist < distancias[n-1]){

        *tam = *tam < n ? *tam + 1 : *tam;

        insereOrdenado(dist, no_atual->reg, distancias, cidadesProximas, n);
    }

    //printf("tam: %d ... ", *tam);
    // for(int i = 0; i < n; i++){
    //     printf("%.1lf ", distancias[i]);
    // }

    cidade* projecao = (cidade*) malloc(sizeof(cidade));

    if(arv->cmp(no_atual->reg, buscada, k % arv->dim) < 0){
        //printf("\nDireito\n");
        
        procuraProximas(arv, no_atual->dir, buscada, k+1, tam, n, distancias, cidadesProximas);

        setProjecao(projecao, no_atual->reg, buscada, k % arv->dim);

        if((no_atual->esq && distancias[n-1] > distancia(buscada, projecao))){
            //printf("Esquerdo mais perto\n");
            procuraProximas(arv, no_atual->esq, buscada, k+1, tam, n, distancias, cidadesProximas);
        }
        else if(*tam < n){
            //printf("Falta cidades, vamo pra esquerda ver");
            procuraProximas(arv, no_atual->esq, buscada, k+1, tam, n, distancias, cidadesProximas);
        }
    }
    else{
        //printf("\nEsquerdo\n");

        procuraProximas(arv, no_atual->esq, buscada, k+1, tam, n, distancias, cidadesProximas);

        setProjecao(projecao, no_atual->reg, buscada, k % arv->dim);
            
        if(no_atual->dir && distancias[n-1] > distancia(buscada, projecao)){
            //printf("Direito mais perto\n");
            procuraProximas(arv, no_atual->dir, buscada, k+1, tam, n, distancias, cidadesProximas);
        }
        else if(*tam < n){
            //printf("Falta cidades, vamo pra direita ver");
            procuraProximas(arv, no_atual->dir, buscada, k+1, tam, n, distancias, cidadesProximas);
        }
    }
    free(projecao);
    
}
void setProjecao(cidade* projecao, cidade* c1, cidade* c2, int k){
    
    switch (k){
    case 0:
        projecao->longitude = c1->longitude;
        projecao->latitude = c2->latitude;
        break;
    case 1:
        projecao->longitude = c2->longitude;
        projecao->latitude = c1->latitude;
        break;
    }
}

void insereOrdenado(double dist, cidade* reg, double* distancias, cidade** cidadesProximas, int n){
    
    distancias[n-1] = dist;
    cidadesProximas[n-1] = reg;

    for(int i = n-1; i > 0; --i){

        if(distancias[i-1] > distancias[i]){
            double aux = distancias[i];
            distancias[i] = distancias[i-1];
            distancias[i-1] = aux;

            cidade* auxC = cidadesProximas[i];
            cidadesProximas[i] = cidadesProximas[i-1];
            cidadesProximas[i-1] = auxC;
        }

    }

}