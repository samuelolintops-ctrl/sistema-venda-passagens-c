/*
EXERCICIO (Credito 2 - 10,0 pontos) => Proposta do projeto

Venda de passagens:
Imagine um sistema de venda de passagens que pode ter um menu com as seguintes funcoes
(I) - Inicializar onibus: Todas as poltronas livres e 0 vendidas
(V) - Visualizar onibus (formato onibus): Mostrar layout do onibus com livres e ocupadas
(E) - Vender passagem: Marcar poltrona
(C) - Cancelar passagem: Liberar poltrona
(S) - Consultar poltrona: livre/ocupada; janela/corredor
(P) - Passagens vendidas/total: emite um relatorio c/ total de vendas do onibus

Criterios:
1. O menu e as funcionalidades precisam ser por funcoes
2. Numero maximo integrantes: 3
3. Data submissao: 09/06

Planejamento projeto
- Usar um arquivo poltronas_onibus.bin para guardar as poltronas
- Cada poltrona sera uma struct:
    typedef Struct{
        float valor;
        int ocupacao;
    }Poltona;
- Funcoes do projeto:
    Modulo util:
    - void limpa_tela();
    - void limpa_buffer();
    - void espera_enter();
    - int int_positivo(char pfrase[]);
    - void mostra_menu();
    - int verifica_arq(FILE *pont_arq);
    Modulo venda_passagem:
    - void inic_bus();
    - void visu_bus();
    - void compra_pass();
    - void cancela_pass();
    - void consulta_polt();
    - void relatorio_venda();

    Para rodar o código:
    gcc main.c util.c sistema_bus.c -o m_programa
    ./m_programa

    O que falta:
    - Documentar funcoes;
    - Implementar confirmação de compra
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
