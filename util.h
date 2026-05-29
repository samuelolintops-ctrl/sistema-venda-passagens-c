#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

void limpa_tela();
void limpa_buffer();
void espera_enter();
int int_intervalo(char pfrase[], int min, int max);
int menu_central();
int verifica_arq(FILE *pont_arq);

#endif // UTIL_H