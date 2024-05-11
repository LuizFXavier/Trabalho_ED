#pragma once
#include <stdlib.h>
#include "cidade.h"
#define INFINITO 0xfffffff

typedef struct no{

    cidade* reg;
    struct no *esq;
    struct no *dir;
}t_no;

typedef struct{

    t_no *raiz;
    int dim;
    double (*cmp)(const void*, const void*, int);
}t_arvore;

t_arvore* cria_arvore(int dim, double (*cmp)(const void*, const void*, int));
int kd_insere(t_arvore* arv, cidade* reg);

void procuraProximas(t_arvore* arv, t_no* no_atual, cidade* buscada, int k, int* tam, int n, double* distancias, cidade** cidadesProximas);
cidade** maisProximas(t_arvore* arv, cidade* buscada, int n);

void insereOrdenado(double dist, cidade* reg, double* distancias, cidade** cidadesProximas, int n);
void setProjecao(cidade* projecao, cidade* c1, cidade* c2, int k);