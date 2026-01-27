#ifndef  OPCODES_H
#define  OPCODES_H


// ========================================================
// --- OPCODE das instruções ---
#define   NOP   0x00                           //no operation
#define   LDI   0x01<<4                        //carga imediata no acumulador
#define   LDD   0x02<<4                        //carga direta no acumulador
#define   LDA   0x03<<4                        //carga indireta no acumulador
#define   STA   0x04<<4                        //armazena acumulador na RAM
#define   ADD   0x05<<4                        //adição do AC com operando endereçado diretamente
#define   SUB   0x06<<4                        //subtração do AC com operando endereçado diretamente
#define   MUL   0x07<<4                        //Multiplicação do AC com operando endereçado diretamente
#define   DIV   0x08<<4                        //Divisão do AC com operando endereçado diretamente
#define   OUT   0x0E<<4                        //carrega conteúdo do acumulador no registrador de saída
#define   HLT   0x0F<<4                        //halt, "congela" o processamento


#endif
