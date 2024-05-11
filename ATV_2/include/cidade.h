#pragma once

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
}cidade;

cidade* cria_cidade(double x, double y);

double distancia(cidade *c1, cidade *c2);