#ifndef COMPUTADOR_H
#define COMPUTADOR_H

#include "cpu.h"
#include "ram.h"

typedef struct {
    CPU cpu;
    RAM ram;
    Barramento bus;
} Computador;

void inicializar_computador(Computador *comp);
void executar_programa(Computador *comp);

#endif

