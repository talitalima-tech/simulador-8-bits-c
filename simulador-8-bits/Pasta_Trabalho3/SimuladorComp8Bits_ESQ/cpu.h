#ifndef CPU_H
#define CPU_H

#include "registradores.h"
#include "ula.h"
#include "uc.h"
#include "barramentos.h"

typedef struct CPU {
    Registradores reg;
    UnidadeControle uc;
    Barramento *bus;
} CPU;

void inicializar_cpu(CPU *cpu, Barramento *bus);
void executar_ciclo(CPU *cpu, RAM *ram);

#endif

