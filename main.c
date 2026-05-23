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
*/

#include <stdio.h>
#include <stdlib.h>
#define QTD_POLTRONAS 46
#define VALOR_PASS 35.90

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

int int_positivo(char pfrase[]){
    /*
    Funcao que le e retorna um inteiro positivo. Em caso de erro fica no loop ate ler um valor valido.
    */
    int n, verifica;
    while(1){
        printf("\n%s", pfrase);
        verifica = scanf("%d", &n);
        if(verifica == 1 && n > 0){
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
        opc = int_positivo("\nDigite sua opcao: ");
        if(opc <= 7) break;
        printf("\n\033[31mErro: valor invalido. Digite novamente.\033[m");
        espera_enter();
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

// Declaracao da struct que define a poltrona
typedef struct{
    int nmr_poltrona;
    float valor;
    int ocupacao; // 0 para livre e 1 para ocupado
} Poltrona;

// Definindo o nome do arquivo
char nome_arq[] = "poltronas_onibus.bin";

void inic_bus(){
    limpa_tela();
    printf("\n--- Operando Inicializar Onibus ---\n");
    char op;
    printf("ATENCAO: A operacao a seguir reseta uma possivel operacao em andamento." 
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
    printf("\nSistema de venda inicializado e poltronas livres.");
    espera_enter();
}

void visu_bus(){
    limpa_tela();
    printf("--- Operando Visualizar Onibus ---");
    //FILE *stream_arq;
    /*codigo*/
    espera_enter();
}

void compra_pass(){
    limpa_tela();
    printf("--- Operando Compra de Passagem ---");
    /*codigo*/
    espera_enter();
}

void cancela_pass(){
    limpa_tela();
    printf("--- Operando Cancela Passagem ---");
    /*codigo*/
    espera_enter();
}

void consulta_polt(){
    limpa_tela();
    printf("--- Operando Consultar Poltrona ---");
    /*codigo*/
    espera_enter();
}

void relatorio_venda(){
    limpa_tela();
    printf("--- Operando Relatorio de Venda ---");
    /*codigo*/
    espera_enter();
}

int main(){
    int opcao;
    do{
        opcao = menu_central();
        switch (opcao)
        {
        case 1:
            inic_bus();
            break;
        case 2:
            visu_bus();
            break;
        case 3:
            compra_pass();
            break;
        case 4:
            cancela_pass();
            break;
        case 5:
            consulta_polt();
            break;
        case 6:
            relatorio_venda();
            break;
        }
    } while(opcao != 7);
    printf("\nEncerrando secao. Ate mais!\n");
    return 0;
}