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
#define QTD_POLTRONAS 48
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

// Declaracao da struct que define a poltrona
typedef struct{
    int nmr_poltrona;
    float valor;
    int ocupacao; // 0 para livre e 1 para ocupado
} Poltrona;

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

void inic_bus(char nome_arq[], float *pvalor_tot, int *pass_tot){
    /*
    Funcao que inicializa o onibus, liberando todas as poltronas.
    */
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
    *pvalor_tot = 0;
    *pass_tot = 0;
    printf("\nSistema de venda inicializado, poltronas livres e valor arrecadado zerado.");
    espera_enter();
}

void visu_bus(char nome_arq[]){
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
    for(int i = 1; i <= 4; i++){
        int polt_inic;
        // Condicionais para que as linhas estejam alinhadas como definido na proposta
        if(i == 1) polt_inic = 1;
        else if(i == 2) polt_inic = 2;
        else if(i == 3) polt_inic = 4;
        else if(i == 4) polt_inic = 3;
        printf("| ");
        for(int j = 1; j <= QTD_POLTRONAS / 4; j++){
            fread(&p, sizeof(Poltrona), 1, stream_arq);
            if(p.ocupacao == 0){
                printf(" (%d) ", p.nmr_poltrona);
            }
            else{
                printf(" (\033[31mX\033[m) ");
            }
            deslocamento = sizeof(Poltrona) * 3;
            fseek(stream_arq, deslocamento, SEEK_CUR);
        }
        printf(" |\n");
        if(i == 2) printf("\n");
        deslocamento = sizeof(Poltrona) * polt_inic;
        fseek(stream_arq, deslocamento, SEEK_SET);
    }
    /*codigo precisando consertar a impressao das duas linhas de baixo*/
    fclose(stream_arq);
    espera_enter();
}

void compra_pass(char nome_arq[], float *pvalor_tot, int *pass_tot){
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
    stream_arq = fopen(nome_arq, "rb");
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
            fclose(stream_arq);
            printf("\033[32mPoltrona de numero %d vendida com sucesso!\033[m", polt);
            // Aqui e onde altero a poltrona do arquivo para ocupado
            stream_arq = fopen(nome_arq, "rb+");
            if(verifica_arq(stream_arq) == 1){
                espera_enter();
                return;
            }
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
    espera_enter();
}

void cancela_pass(char nome_arq[], float *pvalor_tot, int *pass_tot){
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
    espera_enter();
}

void consulta_polt(char nome_arq[]){
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

int main(){
    // Definindo o nome do arquivo
    char nome_arq_arg[] = "poltronas_onibus.bin";
    int opcao;
    int pass_vendidas = 0;
    float faturamento = 0;
    do{
        opcao = menu_central();
        switch (opcao)
        {
        case 1:
            inic_bus(nome_arq_arg, &faturamento, &pass_vendidas);
            break;
        case 2:
            visu_bus(nome_arq_arg);
            break;
        case 3:
            compra_pass(nome_arq_arg, &faturamento, &pass_vendidas);
            break;
        case 4:
            cancela_pass(nome_arq_arg, &faturamento, &pass_vendidas);
            break;
        case 5:
            consulta_polt(nome_arq_arg);
            break;
        case 6:
            relatorio_venda(&faturamento, &pass_vendidas);
            break;
        }
    } while(opcao != 7);
    printf("\nEncerrando secao. Ate mais!\n");
    return 0;
}
