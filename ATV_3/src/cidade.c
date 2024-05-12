#include <math.h>
#include <stdlib.h>
#include "cidade.h"

cidade* cria_cidade(double x, double y){
    cidade* nova = (cidade*) malloc(sizeof(cidade));

    nova->longitude = x;
    nova->latitude = y;

    return nova;
}

double distancia(cidade *c1, cidade *c2){

    return sqrt(pow(c1->longitude - c2->longitude, 2) + pow(c1->latitude - c2->latitude, 2));
}