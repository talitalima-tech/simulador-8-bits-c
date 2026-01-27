#ifndef BARRAMENTO_H
#define BARRAMENTO_H

#include <stdint.h> //biblioteca para definir tipos inteiros
#include "ram.h"

typedef struct {
    uint16_t endereco;
    uint16_t dado;
    char controle[10];  // "READ", "WRITE", etc.
} Barramento;

void enviar_dado(Barramento *bus, RAM *ram, uint16_t endereco, uint16_t dado, const char *controle);

#endif

