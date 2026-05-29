#ifndef SISTEMA_BUS_H
#define SISTEMA_BUS_H

#include <stdio.h>
#define QTD_POLTRONAS 46
#define VALOR_PASS 37.90
// Definindo o nome do arquivo das poltronas e do faturamento
#define NOME_ARQ "poltronas_onibus.bin"
#define NOME_ARQ_FAT "caixa_onibus.bin"

// Declaracao da struct que define a poltrona
typedef struct{
    int nmr_poltrona;
    float valor;
    int ocupacao; // 0 para livre e 1 para ocupado
} Poltrona;

Poltrona busca_polt_ind(FILE *p_arq, int pnmr_polt);
void inic_bus(float *pvalor_tot, int *pass_tot);
void visu_bus();
void compra_pass(float *pvalor_tot, int *pass_tot);
void cancela_pass(float *pvalor_tot, int *pass_tot);
void consulta_polt();
void relatorio_venda(float *pvalor_tot, int *pass_tot);

#endif // SISTEMA_BUS_H