#ifndef UC_H
#define UC_H

// Declara��o antecipada para evitar erro de tipo desconhecido
struct CPU;

#include "ram.h"
#include "opcodes.h"

typedef struct {
    int halt;  // Indica se o processador deve parar
} UnidadeControle;

void inicializar_uc(UnidadeControle *uc);
void ciclo_busca(struct CPU *cpu, RAM * ram);
void ciclo_indireto(struct CPU *cpu, RAM * ram);
int ciclo_execucao(struct CPU *cpu, RAM *ram);
void ciclo_interrupcao(struct CPU *cpu, RAM *ram);
void ldi_exec(struct CPU *cpu);
void ldd_exec(struct CPU *cpu, RAM *ram);
void sta_exec(struct CPU *cpu, RAM *ram);
void add_exec(struct CPU *cpu, RAM *ram);
void sub_exec(struct CPU *cpu, RAM *ram);
void mul_exec(struct CPU *cpu, RAM *ram);
void div_exec(struct CPU *cpu, RAM *ram);
void out_exec(struct CPU *cpu, RAM *ram);


#endif

