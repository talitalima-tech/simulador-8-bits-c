#include "registradores.h"
#include <stdio.h>

void inicializar_registradores(Registradores *reg) {
    reg->AC = reg->FLAG = reg->IR = reg->MAR = reg->MBR = reg->OTR = reg->PC = reg->Y = reg->Z = reg->ICC = 0;
}

void imprimir_registradores(const Registradores *reg) {
	printf(" --------------------------\n");
	printf(" PC\t=\t%4X\n", reg->PC);
	printf(" AC\t=\t%4X\n", reg->AC);
	printf(" MBR\t=\t%4X\n", reg->MBR);
	printf(" MAR\t=\t%4X\n", reg->MAR);
	printf(" IR\t=\t%4X\n", reg->IR);
	printf(" Y\t=\t%4X\n", reg->Y);
	printf(" Z\t=\t%4X\n", reg->Z);	
	printf(" OTR\t=\t%4X\n", reg->OTR);
	printf(" --------------------------\n");
}

