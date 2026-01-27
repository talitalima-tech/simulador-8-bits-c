#ifndef  ULA_H
#define  ULA_H

#include "bit.h"
#include <stdint.h> //biblioteca para definir tipos inteiros

//uint8_t - Inteiro sem sinal de 8 bits
//uint16_t - Inteiro sem sinal de 16 bits

//int8_t - Inteiro com sinal de 8 bits
//int16_t - Inteiro com sinal de 16 bits

static void somadorCompleto (uint8_t A, uint8_t B, uint8_t Cin, uint8_t * co, uint8_t * s);
static void somador8bits( uint8_t A, uint8_t B, uint8_t cin,  uint8_t * co, uint8_t * s);
static void complementador(uint8_t *A);

//Operações com números Inteiros de 8 bits com sinal (-128 até 127)
void ULA_ADD(int8_t *A, int8_t * B, int8_t * overflow);
void ULA_SUB(int8_t *A, int8_t * B, int8_t * overflow);
void ULA_MUL(int8_t *A, int8_t *Q, int8_t * M, int8_t * overflow);
void ULA_DIV(int8_t *A, int8_t *Q, int8_t * M, int8_t * overflow);


#endif

