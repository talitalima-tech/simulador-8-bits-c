#ifndef  REGISTRADORES_H
#define  REGISTRADORES_H

#include <stdint.h> //biblioteca para definir tipos inteiros

typedef struct {
    uint8_t AC, 
			MBR, 
			MAR, 
			PC, 
			IR, 
			Y, 
			Z, 
			OTR, 
			FLAG,
			ICC; // Registradores de usuário, controle e sistema
} Registradores;

void inicializar_registradores(Registradores *reg);
void imprimir_registradores(const Registradores *reg);

#endif
