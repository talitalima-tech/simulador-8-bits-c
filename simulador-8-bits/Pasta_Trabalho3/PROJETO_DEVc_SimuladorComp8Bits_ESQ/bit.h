#ifndef  BIT_H
#define  BIT_H

#define bit_get(reg, bit)  (((reg) & (1<<(bit)))>0) ? 1 : 0  //verifica qual valor do bit na posição desejada
#define bit_set(reg, bit)  ((reg) |= (1<<(bit)))           //Coloca o bit para 1 na posição desejada
#define bit_clr(reg, bit)  ((reg) &= ~(1<<(bit)))          //Coloca o bit para 0 na posição desejada
#define bit_inv(reg, bit)  ((reg) ^= (1<<(bit)))           //Inverte o bit na posição desejada

#endif
