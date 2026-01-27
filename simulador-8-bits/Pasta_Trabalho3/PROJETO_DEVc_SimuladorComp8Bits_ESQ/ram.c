#include "ram.h"

void escrever_memoria(RAM *ram, uint8_t endereco, uint8_t dado) {
    if (endereco >= 0 && endereco < RAM_TAMANHO) {
        ram->memoria[endereco] = dado;
    }
}

uint8_t ler_memoria(RAM *ram, uint8_t endereco) {
    if (endereco >= 0 && endereco < RAM_TAMANHO) {
        return ram->memoria[endereco];
    }
    return -1; // Endereço inválido
}

void inicializar_memoria(RAM *ram) {
    memset(ram->memoria, 0x00, RAM_TAMANHO);
}

