#include "cpu.h"
#include <stdio.h>

void inicializar_cpu(CPU *cpu, Barramento *bus) {
    inicializar_registradores(&cpu->reg);
    inicializar_uc(&cpu->uc);
    cpu->bus = bus;
}

void executar_ciclo(CPU *cpu, RAM *ram) {
	imprimir_registradores(&cpu->reg);
    while (!cpu->uc.halt) {  // Loop cont�nuo para simular um processador real
        switch(cpu->reg.ICC){
			case 0:
                ciclo_busca(cpu, ram);
                break;
            case 1:
                ciclo_indireto(cpu, ram);
                break;
            case 2:
                ciclo_execucao(cpu, ram);
                break;
            case 3:
                ciclo_interrupcao(cpu, ram);
                break;

			//Fluxo do Ciclo de Instru��o
			
		};
		imprimir_registradores(&cpu->reg);
		//getchar();
    }
}

