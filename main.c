/*
EXERCICIO (Credito 2 - 10,0 pontos)
    Para rodar o código:
    gcc main.c util.c sistema_bus.c -o m_programa
    ./m_programa
*/

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "sistema_bus.h"

int main(){
    int opcao, pass_vendidas;
    float faturamento;
    // Passando os valores de faturamento para variaveis
    FILE *arq_fat;
    arq_fat = fopen(NOME_ARQ_FAT, "a+");
    if(verifica_arq(arq_fat) == 1){
        espera_enter();
        return 1;
    }
    fseek(arq_fat, 0, SEEK_SET);
    fscanf(arq_fat, "%f %d", &faturamento, &pass_vendidas);
    fclose(arq_fat);
    do{
        opcao = menu_central();
        switch (opcao)
        {
        case 1:
            inic_bus(&faturamento, &pass_vendidas);
            break;
        case 2:
            visu_bus();
            break;
        case 3:
            compra_pass(&faturamento, &pass_vendidas);
            break;
        case 4:
            cancela_pass(&faturamento, &pass_vendidas);
            break;
        case 5:
            consulta_polt();
            break;
        case 6:
            relatorio_venda(&faturamento, &pass_vendidas);
            break;
        }
    } while(opcao != 7);
    printf("\nEncerrando secao. Ate mais!\n");
    return 0;
}
