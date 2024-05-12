#include <stdlib.h>
#include <stdio.h>
#include "kd-tree.h"

// "Construtor" da KD-tree
t_arvore* cria_arvore(int dim, double (*cmp)(const void*, const void*, int)){
    t_arvore* nova = (t_arvore*) malloc(sizeof(t_arvore));

    nova->dim = dim;
    nova->cmp = cmp;
    nova->raiz = NULL;
    return nova;
}

int kd_insere(t_arvore* arv, cidade* reg){
    
    //Primeira inserção na árvore
    if(!arv->raiz){
        arv->raiz = (t_no*)malloc(sizeof(t_no));
        arv->raiz->reg = reg;
        return EXIT_SUCCESS;
    }

    t_no* no_atual = arv->raiz;
    int k = 0;

    // Os eixos são avaliados alternadamente para garantir a inserção correta na KD-tree
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

void KD_destroi(t_arvore* arv){
    destroi_No(arv->raiz);
    free(arv);
}
//Percurso pós ordem para liberação de memória
void destroi_No(t_no* no){
    
    if(no->esq){
        destroi_No(no->esq);
    }
    if(no->dir){
        destroi_No(no->dir);
    }
    free(no->reg);
    free(no);
}

cidade** maisProximas(t_arvore* arv, cidade* buscada, int n){

    // Declaração das variáveis utilizadas para comparação
    double distancias[n+1];
    cidade** cMaisProximas = (cidade**) calloc(n+1, sizeof(cidade*));

    for(int i = 0; i <= n; i++){
        distancias[i] = INFINITO;
        cMaisProximas[i] = 0;
    }
    int v = 0;
    int *p = &v;

    // Busca das N cidades mais próximas daquela informada pelo usuário
    procuraProximas(arv, arv->raiz, buscada, 0, p, n, distancias, cMaisProximas);

    return cMaisProximas;
}

void procuraProximas(t_arvore* arv, t_no* no_atual, cidade* buscada, int k, int* tam, int n, double* distancias, cidade** cidadesProximas){
    
    // Indica que o nó anterior na recursão era uma folha, logo a recursão deve voltar
    if(!no_atual){
        return;
    }
    double dist = distancia(no_atual->reg, buscada);
    
    // Distâncias menores são armazenadas de forma ordenada. Se a distância for zero, ela não é armazenada
    if(dist != 0 && dist < distancias[n-1]){

        *tam = *tam < n ? *tam + 1 : *tam;

        insereOrdenado(dist, no_atual->reg, distancias, cidadesProximas, n);
    }

    cidade* projecao = (cidade*) malloc(sizeof(cidade));


    //Busca no lado direito da árvore caso a cidade procurada esteja à leste (ou norte) da atual
    if(arv->cmp(no_atual->reg, buscada, k % arv->dim) < 0){
        
        procuraProximas(arv, no_atual->dir, buscada, k+1, tam, n, distancias, cidadesProximas);

        setProjecao(projecao, no_atual->reg, buscada, k % arv->dim);

        // Caso a maior distância armazenada até então seja menor que a distância até a projeção, procurar no outro lado da árvore
        if((no_atual->esq && distancias[n-1] > distancia(buscada, projecao))){
            
            procuraProximas(arv, no_atual->esq, buscada, k+1, tam, n, distancias, cidadesProximas);
        }

        // Caso o total de cidades ainda não tenha sido alcançado, procurar mais do outro lado da árvore
        else if(*tam < n){
            
            procuraProximas(arv, no_atual->esq, buscada, k+1, tam, n, distancias, cidadesProximas);
        }
    }
    else{

        procuraProximas(arv, no_atual->esq, buscada, k+1, tam, n, distancias, cidadesProximas);

        setProjecao(projecao, no_atual->reg, buscada, k % arv->dim);
            
        if(no_atual->dir && distancias[n-1] > distancia(buscada, projecao)){
            
            procuraProximas(arv, no_atual->dir, buscada, k+1, tam, n, distancias, cidadesProximas);
        }
        else if(*tam < n){
            
            procuraProximas(arv, no_atual->dir, buscada, k+1, tam, n, distancias, cidadesProximas);
        }
    }
    free(projecao);
    
}

//Função que define a projeção de uma cidade c1 sobre a linha produzida por c2 no plano
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

// Função que insere o novo elemento encontrado na posição adequada, garantindo a ordenação das distâncias
void insereOrdenado(double dist, cidade* reg, double* distancias, cidade** cidadesProximas, int n){
    
    distancias[n-1] = dist;
    cidadesProximas[n-1] = reg;

    // As cidades são ordenadas junto com o vetor das distâncias
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