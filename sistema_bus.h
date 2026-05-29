#ifndef SISTEMA_BUS_H
#define SISTEMA_BUS_H

#include <stdio.h>
#define QTD_POLTRONAS 46
#define VALOR_PASS 37.90

// Declaracao da struct que define a poltrona
typedef struct{
    int nmr_poltrona;
    float valor;
    int ocupacao; // 0 para livre e 1 para ocupado
} Poltrona;

// Definindo o nome do arquivo das poltronas
char nome_arq[] = "poltronas_onibus.bin";
// Definindo o nome do arquivo do faturamento
char nome_arq_fat[] = "faturamento.txt";

Poltrona busca_polt_ind(FILE *p_arq, int pnmr_polt);
void inic_bus(float *pvalor_tot, int *pass_tot);
void visu_bus();
void compra_pass(float *pvalor_tot, int *pass_tot);
void cancela_pass(float *pvalor_tot, int *pass_tot);
void consulta_polt();
void relatorio_venda(float *pvalor_tot, int *pass_tot);

#endif // SISTEMA_BUS_H