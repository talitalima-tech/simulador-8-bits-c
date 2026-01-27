#ifndef RAM_H
#define RAM_H

#include <stdint.h> //biblioteca para definir tipos inteiros
#include <string.h> 
#define RAM_TAMANHO 16  // Memória de 16 bytes

typedef struct {
    uint8_t memoria[RAM_TAMANHO];
} RAM;

void escrever_memoria(RAM *ram, uint8_t endereco, uint8_t dado);
uint8_t ler_memoria(RAM *ram, uint8_t endereco);
void inicializar_memoria(RAM * ram);


#endif

