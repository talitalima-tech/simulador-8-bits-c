#include "computador.h"
#include "opcodes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define CAMINHO_ARQUIVO "../programas/instrucoes.txt"

void boot_system(Computador * meuComputador);
void carregar_operandos(const char *nome_arquivo, Computador *meuComputador);
void carregar_operacoes(const char *nome_arquivo, Computador *meuComputador);
uint8_t obter_opcode(const char *instrucao);
void mostrar_memoria(Computador *meuComputador);

int main() {
    Computador meuComputador;   
	boot_system(&meuComputador); 
	system("pause");
	return 0;
}

void boot_system(Computador * meuComputador){
	/*Cabe�alho*/
	printf("\t\t\t=========================================================\n");
    printf("\t\t\tSIMULADOR DE COMPUTADOR DE 8 BITS - v0.1  by Prof. Reuber\n");
    printf("\t\t\t=========================================================\n");
    
    int i=0;
    printf("\nCarregando Programa na Memoria RAM");
    while(i++<3){
    	printf(" . ");  	
	}
	printf("\n\n");
	
	inicializar_computador(meuComputador);
		
	 // Carregar os operandos do arquivo
    carregar_operandos(CAMINHO_ARQUIVO, meuComputador);    
    // Carregar as opera��es do arquivo
    carregar_operacoes(CAMINHO_ARQUIVO, meuComputador);	
	mostrar_memoria(meuComputador);
	
	executar_programa(meuComputador);
	
	mostrar_memoria(meuComputador);	
}


// Fun��o para carregar os operandos na mem�ria
void carregar_operandos(const char *nome_arquivo, Computador *meuComputador) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[256];
    uint8_t endereco = 9;  // Come�amos a carregar os operandos a partir do endere�o 9

    // Carregar operandos at� encontrar uma linha vazia ou o in�cio das opera��es
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Ignora linhas vazias e coment�rios
        if (linha[0] == '\n' || linha[0] == '#') {
            continue;
        }

        // Verifica se encontrou uma linha indicando o in�cio das opera��es
        if (strstr(linha, "Opera��es") != NULL) {
            break; // Sai do loop quando encontrar a parte de opera��es
        }

        uint8_t operando;
        if (sscanf(linha, "%hhd", &operando) == 1) {
            escrever_memoria(&meuComputador->ram, endereco++, operando);
        } 
    }

    fclose(arquivo);
}

// Fun��o para carregar as opera��es e endere�os na mem�ria
void carregar_operacoes(const char *nome_arquivo, Computador *meuComputador) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[256];
    uint8_t endereco = 0;  // As opera��es come�am a ser carregadas a partir do endere�o 0

    // Carregar opera��es at� o fim do arquivo
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Ignora linhas vazias e coment�rios
        if (linha[0] == '\n' || linha[0] == '#') {
            continue;
        }

        uint8_t operando;
        char instrucao[10];  // Para armazenar o nome da opera��o (ex: "LDD", "ADD", etc.)

        // A primeira parte da linha � a opera��o e a segunda parte � o operando
        if (sscanf(linha, "%s %hhx", instrucao, &operando) == 2) {
            uint8_t opcode = obter_opcode(instrucao);
            if (opcode != 0) {
                escrever_memoria(&meuComputador->ram, endereco++, opcode | operando);
            } else {
                printf("Instru��o inv�lida: %s\n", instrucao);
            }
        } 
    }

    fclose(arquivo);
}

// Fun��o para obter o opcode com base no texto
uint8_t obter_opcode(const char *instrucao) {
    if (strcmp(instrucao, "NOP") == 0) return NOP;
    if (strcmp(instrucao, "LDI") == 0) return LDI;
    if (strcmp(instrucao, "LDD") == 0) return LDD;
    if (strcmp(instrucao, "LDA") == 0) return LDA;
    if (strcmp(instrucao, "STA") == 0) return STA;
    if (strcmp(instrucao, "ADD") == 0) return ADD;
    if (strcmp(instrucao, "SUB") == 0) return SUB;
    if (strcmp(instrucao, "MUL") == 0) return MUL;
    if (strcmp(instrucao, "DIV") == 0) return DIV;
    if (strcmp(instrucao, "OUT") == 0) return OUT;
    if (strcmp(instrucao, "HLT") == 0) return HLT;
    return 0;  // Retorna 0 se a instru��o n�o for reconhecida
}


void mostrar_memoria(Computador *meuComputador) {
    // Imprimir o cabe�alho da tabela
    printf("Memoria RAM:\n");
    printf("+-----+--------+\n");
    printf("| End. | Valor  |\n");
    printf("+-----+--------+\n");
    
    // Imprimir os dados da mem�ria
    int i=0;
    while(i<16) {
        printf("|  %2d | 0x%02X |\n", i, ler_memoria(&meuComputador->ram,(uint8_t) i));
        i++;
    }

    printf("+-----+--------+\n");
}

