#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void limpa_tela(){
    /*
    Funcao de limpar a tela do terminal. Portatil para Windows, Linux e Mac.
    */
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void limpa_buffer(){
    /*
    Funcao que limpa o buffer do teclado. Util para usar antes de leituras sensiveis ao \n ou apos um scanf.
    Alternativa ao fflush para evitar problema com outros SO alem do Windows.
    */
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}

void espera_enter(){
    /*
    Funcao para travar a tela e esperar o enter do usuario.
    (Por conta do getchar, importante usar um limpa_buffer antes para evitar problemas)
    */
    printf("\nPressione \033[33mENTER\033[m para continuar...");
    getchar();
}

int int_intervalo(char pfrase[], int min, int max){
    /*
    Funcao que le e retorna um inteiro dentro do intervalo estabelecido. Em caso de erro fica no loop ate ler um valor valido.
    parametro pfrase: frase cmoando para o usuario.
    return: retorna um int valido.
    */
    int n, verifica;
    while(1){
        printf("%s", pfrase);
        verifica = scanf("%d", &n);
        if(verifica == 1 && n >= min && n <= max){
            limpa_buffer();
            break;
        }
        limpa_buffer();
        printf("\n\033[31mErro: valor invalido. Digite novamente.\033[m");
        espera_enter();
    }
    return n;
}

int menu_central(){
    /*
    Funcao para mostrar o menu central do programa.
    return: retorna a opcao escolhida do usuario.
    */
    int opc;
    while(1){
        limpa_tela();
        printf("\n1. Inicializar onibus"
        "\n2. Visualizar onibus"
        "\n3. Vender passagem"
        "\n4. Cancelar passagem"
        "\n5. Consultar poltrona"
        "\n6. Passagens vendidas/total"
        "\n7. Sair");
        opc = int_intervalo("\n\033[34mDigite sua opcao: \033[m", 1, 7);
        break;
    }
    return opc;
}

int verifica_arq(FILE *pont_arq){
    /*
    Funcao que verifica a abertura do arquivo. (Usar apos de um fopen)
    parametro pont_arq: ponteiro para o arquivo a se verificar.
    return: retorna 1 em caso de erro, em caso de sucesso retorna 0.
    */
    if(pont_arq == NULL){
        printf("\n\033[31mErro na abertura do arquivo.\033[m");
        return 1;
    }
    return 0;
}