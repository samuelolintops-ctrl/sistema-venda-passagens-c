#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "sistema_bus.h"

Poltrona busca_polt_ind(FILE *p_arq, int pnmr_polt){
    /*
    Funcao que faz uma busca de poltrona pelo seu numero. (Necessario abrir o arquivo no modo rb antes de usar essa funcao)
    parametro p_arq: ponteiro do arquivo.
    parametro pnmr_polt: numero da poltrona.
    return: retorna a struct da poltrona.
    */
    Poltrona assento;
    long desloc;
    desloc = sizeof(Poltrona) * (pnmr_polt - 1);
    fseek(p_arq, desloc, SEEK_SET);
    fread(&assento, sizeof(Poltrona), 1, p_arq);
    return assento;
}

void inic_bus(float *pvalor_tot, int *pass_tot){
    /*
    Funcao que inicializa o onibus, liberando todas as poltronas.
    */
    limpa_tela();
    printf("\n--- Operando Inicializar Onibus ---\n");
    char op;
    printf("ATENCAO: A operacao a seguir reseta uma possivel operacao em andamento. " 
    "Deseja prosseguir? (S/N) ");
    scanf(" %c", &op);
    limpa_buffer();
    if(op == 'N' || op == 'n'){
        printf("\nOperacao cancelada.");
        espera_enter();
        return;
    }
    FILE *stream_arq;
    Poltrona p;
    stream_arq = fopen(nome_arq, "wb");
    if((verifica_arq(stream_arq)) == 1) return;
    for(int i = 0; i < QTD_POLTRONAS; i++){
        p.nmr_poltrona = i + 1;
        p.valor = VALOR_PASS;
        p.ocupacao = 0;
        fwrite(&p, sizeof(Poltrona), 1, stream_arq);
    }
    fclose(stream_arq);
    stream_arq = fopen(nome_arq_fat, "w");
    fprintf(stream_arq, "%f %d", 0.0, 0);
    if((verifica_arq(stream_arq)) == 1) return;
    fclose(stream_arq);
    *pvalor_tot = 0;
    *pass_tot = 0;
    printf("\nSistema de venda inicializado, poltronas livres e valor arrecadado zerado.");
    espera_enter();
}

void visu_bus(){
    /*
    Funcao que exibe o status do onibus no formato exigido:
    (1) (5)...
    (2) (X)...

    (4) (8)...
    (3) (7)...
    X indica que a poltrona ja foi vendida.
    */
    limpa_tela();
    printf("\n--- Operando Visualizar Onibus ---\n");
    FILE *stream_arq;
    Poltrona p;
    int cont = 0;
    long deslocamento;
    stream_arq = fopen(nome_arq, "rb");
    if(verifica_arq(stream_arq) == 1){
        printf("\nFalha na abertura dos dados, inicialize o onibus e tente novamente.");
        espera_enter();
        return;
    }
    for(int i = 0; i < 4; i++){
        int polt_inic;
        // Condicionais para que as linhas estejam alinhadas como definido na proposta
        if(i == 0) polt_inic = 0;
        else if(i == 1) polt_inic = 1;
        else if(i == 2) polt_inic = 3;
        else if(i == 3) polt_inic = 2;
        printf("| ");
        for(int j = 0; j < QTD_POLTRONAS; j += 4){
            if((polt_inic + j) < QTD_POLTRONAS){
                deslocamento = (sizeof(Poltrona) * polt_inic) + (sizeof(Poltrona) * j);
                fseek(stream_arq, deslocamento, SEEK_SET);
                fread(&p, sizeof(Poltrona), 1, stream_arq);
                if(p.ocupacao == 0){
                    printf(" (%d) ", p.nmr_poltrona);
                }
                else{
                    printf(" (\033[31mX\033[m) ");
                }
            }
        }
        printf(" |\n");
        if(i == 1) printf("\n");
    }
    /*codigo precisando consertar a impressao das duas linhas de baixo*/
    fclose(stream_arq);
    espera_enter();
}

void compra_pass(float *pvalor_tot, int *pass_tot){
    /*
    Funcao que opera a venda de passagens. Apos venda de uma poltrona desocupada muda no arquivo de operacao para ocupada.
    */
    limpa_tela();
    printf("\n--- Operando Compra de Passagem ---\n");
    FILE *stream_arq;
    Poltrona p;
    long deslocamento;
    float valor_venda;
    char sair;
    int polt;
    int polt_livre = 0;
    stream_arq = fopen(nome_arq, "rb+");
    if(verifica_arq(stream_arq) == 1){
        espera_enter();
        return;
    }
    // Loop enquanto nao se achauma poltrona desocupada
    while(polt_livre == 0){
        polt = int_intervalo("\nDigite a poltrona que quer comprar: ", 1, QTD_POLTRONAS);
        p = busca_polt_ind(stream_arq, polt);
        // Aqui e onde ocorre a venda
        if(p.ocupacao == 0){
            valor_venda = p.valor;
            p.ocupacao = 1;
            polt_livre = 1;
            printf("\033[32mPoltrona de numero %d vendida com sucesso!\033[m", polt);
            // Aqui e onde altero a poltrona do arquivo para ocupado
            deslocamento = sizeof(Poltrona) * (polt - 1);
            fseek(stream_arq, deslocamento, SEEK_SET);
            fwrite(&p, sizeof(Poltrona), 1, stream_arq);
            *pvalor_tot = *pvalor_tot + valor_venda;
            (*pass_tot)++;
            break;
        }
        // Caso o assento digitado ja foi vendido:
        printf("\nAssento indisponivel, deseja repetir operacao com outro assento? (S/N) ");
        scanf(" %c", &sair);
        limpa_buffer();
        if(sair == 'N' || sair == 'n') break;
    }
    fclose(stream_arq);
    // Gravando faturamento
    stream_arq = fopen(nome_arq_fat, "w");
    fprintf(stream_arq, "%f %d", *pvalor_tot, *pass_tot);
    fclose(stream_arq);
    espera_enter();
}

void cancela_pass(float *pvalor_tot, int *pass_tot){
    limpa_tela();
    printf("\n--- Operando Cancela Passagem ---\n");
    FILE *stream_arq;
    Poltrona p;
    char sair;
    int polt;
    long deslocamento;
    float valor_retorno;
    stream_arq = fopen(nome_arq, "rb+");
    if(verifica_arq(stream_arq) == 1){
        espera_enter();
        return;
    }
    while(1){
        polt = int_intervalo("\nDigite o numero da poltrona para cancelar venda: ", 1, QTD_POLTRONAS);
        p = busca_polt_ind(stream_arq, polt);
        if(p.ocupacao == 1){
            // liberando o assento
            valor_retorno = p.valor;
            p.ocupacao = 0;
            deslocamento = sizeof(Poltrona) * (polt - 1);
            fseek(stream_arq, deslocamento, SEEK_SET);
            fwrite(&p, sizeof(Poltrona), 1, stream_arq);
            printf("\n\033[32mPoltrona liberada com sucesso!\033[m");
            *pvalor_tot = *pvalor_tot - valor_retorno;
            (*pass_tot)--;
            break;
        }
        printf("\nAssento livre, deseja repetir operacao com outro assento? (S/N) ");
        scanf(" %c", &sair);
        limpa_buffer();
        if(sair == 'N' || sair == 'n') break;
    }
    fclose(stream_arq);
    // Gravando faturamento
    stream_arq = fopen(nome_arq_fat, "w");
    fprintf(stream_arq, "%f %d", *pvalor_tot, *pass_tot);
    fclose(stream_arq);
    espera_enter();
}

void consulta_polt(){
    /*
    Funcao que procura por um assento e mostra se esta livre ou ocupada e se e janela ou corredor.
    */
    limpa_tela();
    printf("\n--- Operando Consultar Poltrona ---\n");
    FILE *stream_arq;
    Poltrona p;
    int nmr_polt;
    stream_arq = fopen(nome_arq, "rb");
    if(verifica_arq(stream_arq) == 1){
        espera_enter();
        return;
    }
    nmr_polt = int_intervalo("\nDigite o numero do assento: ", 1, QTD_POLTRONAS);
    p = busca_polt_ind(stream_arq, nmr_polt);
    fclose(stream_arq);
    // Pares sao corredor e impares sao janela
    if((p.nmr_poltrona % 2) == 0) printf("\nPoltrona: corredor");
    else printf("\nPoltrona: janela");
    // 1 ocupado e 0 desocupado
    if(p.ocupacao == 1) printf("\nSituacao: \033[31mOCUPADO\033[m");
    else printf("\nSituacao: \033[32mLIVRE\033[m");
    espera_enter();
}

void relatorio_venda(float *pvalor_tot, int *pass_tot){
    /*
    Emite um relatorio financeiro de quantas passagens foram vendidas e o valor total arrecadado.
    */
    limpa_tela();
    printf("\n--- Operando Relatorio de Venda ---\n");
    printf("\nTotal de passagens vendidas: %d.", *pass_tot);
    printf("\nTotal arrecadado durante a operacao: RS%.2f.", *pvalor_tot);
    espera_enter();
}