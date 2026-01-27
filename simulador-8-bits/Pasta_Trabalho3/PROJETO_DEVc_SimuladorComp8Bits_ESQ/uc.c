#include "uc.h"
#include "cpu.h"
#include <stdio.h>

void inicializar_uc(UnidadeControle *uc) {
    uc->halt = 0;
}


//Função 1
void ciclo_busca(CPU *cpu, RAM * ram){
	printf(" >>> Ciclo de Busca\n");
	cpu -> reg.MAR = cpu -> reg.PC; // MAR recebe o valor de PC
	cpu -> reg.PC++; // PC é incrementado
	enviar_dado(cpu->bus, ram, cpu->reg.MAR, 0, "READ"); // busca o operando no endereço MAR
	cpu -> reg.MBR = cpu -> bus -> dado & 0x00FF; // armazena o dado lido no MBR                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	cpu -> reg.IR = cpu -> reg.MBR; // carrega no IR
	if((cpu -> reg.IR & 0xF0) == LDA){ // define o estado ICC com base na instrução carregada
		cpu -> reg.ICC = 1;
	}
	else{
		cpu -> reg.ICC = 2;
	}
}

//função 2
void ciclo_indireto(CPU *cpu, RAM * ram){
	cpu -> reg.MAR = cpu -> reg.IR & 0x0F; // MAR recebe o 4 bits menos significativos de IR
	enviar_dado(cpu->bus, ram, cpu->reg.MAR, 0, "READ"); // busca o operador no endereço MAR
	cpu ->reg.MBR = cpu -> bus -> dado & 0x00FF; // armazena o dado lido no MBR
	cpu -> reg.IR = (cpu -> reg.IR & 0xF0) | (cpu -> reg.MBR & 0x0F);
	cpu  -> reg.ICC = 2;
}

//função 3
int ciclo_execucao(CPU *cpu, RAM *ram) {
    uint8_t instrucao = cpu->reg.IR & 0xF0; // Verifica a operação que está armazenada nos 4 bits mais significativos de IR
	if (instrucao == LDI){ 
		printf(">>> LDI executando\n");
		ldi_exec(cpu);
	} else if (instrucao == LDD) {
		printf(">>> LDD executando\n");
		ldd_exec(cpu, ram);
	} else if (instrucao == STA) {
		printf(">>> STA executando\n");
		sta_exec(cpu, ram);
	} else if (instrucao == ADD) {
		printf(">>> ADD executando\n");
		add_exec(cpu, ram);
	} else if (instrucao == SUB) {
		printf(">>> SUB executando\n");
		sub_exec(cpu, ram);
	} else if (instrucao == MUL) {
		printf(">>> MUL executando\n");
		mul_exec(cpu, ram);
	} else if (instrucao == DIV) {
		printf(">>> DIV executando\n");
		div_exec(cpu, ram);
	} else if (instrucao == OUT) {
		printf(">>> OUT executando\n");
		out_exec(cpu, ram);
	} else if (instrucao == HLT) {
		printf(">>> HALT processor\n");
		cpu->uc.halt = 1;
		return 0;
	} else {
		printf("Instrução desconhecida: %hhu" , instrucao);
	}
		
    return 1;
}

//função 4
void ciclo_interrupcao(CPU *cpu, RAM *ram){
	if (cpu->reg.FLAG == 2) {
		printf(">>>ULA->DIVISAO POR ZERO");
	} else if (cpu->reg.FLAG == 1) {
		printf(">>>ULA->OVERFLOW");
	}
	cpu->reg.MBR = cpu->reg.PC; // Armazenar o valor do contador de programa (PC) no MBR
	cpu->reg.MAR = 0x0F; // Configurar MAR com o valor 0x0F (endereço de interrupção)
	enviar_dado(cpu->bus, ram, cpu->reg.MAR, cpu->reg.MBR, "WRITE"); // Escrever o dado no endereço 0x0F (escreve o valor de PC na memória)
	// Realizar a busca de instruções até encontrar o código HALT
	while ((cpu->reg.IR & 0xF0) != cpu->uc.halt) {
		ciclo_busca(cpu, ram);
	}
}

//função 5
void ldi_exec(CPU *cpu){
    cpu->reg.AC = cpu->reg.IR & 0x0F;
    cpu->reg.ICC = 0;
}

//função 6
void ldd_exec(CPU *cpu, RAM *ram){
	cpu -> reg.MAR = cpu -> reg.IR & 0x0F; // configurar MAR com os 4 bits mais baixos do IR(endereço de memória)
	enviar_dado(cpu->bus, ram, cpu->reg.MAR, 0, "READ"); // enviar o endereço (MAR) para o barra. e ler o dado da mem.
	cpu -> reg.AC = cpu -> bus -> dado & 0x00FF; // carregar o dado lido na memória do acumulador AC;
	cpu -> reg.ICC = 0; // resetar o indicador de condição(ICC) para 0
}

//função 7
void sta_exec(CPU *cpu, RAM *ram){
    cpu->reg.MAR = cpu->reg.IR & 0x0F;
    enviar_dado(cpu->bus, ram, cpu->reg.MAR, cpu->reg.AC, "WRITE");
    cpu->reg.ICC = 0;
}

//função 8
void add_exec(CPU *cpu, RAM *ram){
	cpu->reg.MAR = cpu->reg.IR & 0x0F; //Configura MAR com os 4 bits mais baixos do IR (endereço de memória)
	enviar_dado(cpu->bus, ram, cpu->reg.MAR, 0, "READ");// Envia o endereço (MAR) para o barramento e ler o dado da memória
	if (cpu->bus->dado != -1){//Se cpu->bus->dado for diferente -1 o enderençõ é válido ,logo continua a execução
		cpu->reg.MBR = cpu->bus->dado & 0x00FF;// Carrega o dado lido no MBR, mantendo apenas os 8 bits menos significativos
		ULA_ADD( (int8_t *)&cpu->reg.AC, (int8_t *)&cpu->reg.MBR, (int8_t *)&cpu->reg.FLAG);// Chama a função ULA_ADD e realiza a operação de adição entre o AC e o MBR, atualizando a FLAG
		if(cpu->reg.FLAG == 1){// Se a FLAG indicar overflow (valor 1)
			cpu->reg.ICC = 3;//A ICC é ajustada para 3
		}
		else{//Se a FLAG não indicar overflow (valor 0)
			cpu->reg.ICC = 0;//A ICC é ajustada para 0
		}
	}
}

//função 9
void sub_exec(CPU *cpu, RAM *ram){
	cpu->reg.MAR = cpu->reg.IR & 0x0F; //Configurar MAR com os 4 bits mais baixos do IR (endereço de memória)
	enviar_dado(cpu->bus, ram, cpu->reg.MAR, 0, "READ");// Enviar o endereço (MAR) para o barramento e ler o dado da memória
	if (cpu->bus->dado != -1){//Se bus->dado for diferente -1 o enderenço é válido,logo continua a execução
		cpu->reg.MBR = cpu->bus->dado & 0x00FF;// Carrega o dado lido no MBR, mantendo apenas os 8 bits menos significativos
		ULA_SUB( (int8_t *)&cpu->reg.AC, (int8_t *)&cpu->reg.MBR, (int8_t *)&cpu->reg.FLAG);// Chama a função ULA_SUB e realiza a operação de substração entre o AC e o MBR, atualizando a FLAG
		if (cpu->reg.FLAG == 1){// Se a FLAG indicar overflow (valor 1)
			cpu->reg.ICC = 3;//A ICC é ajustada para 3
		}else {//Se a FLAG não indicar overflow (valor 0)
			cpu->reg.ICC = 0;//A ICC é ajustada para 0
		}
	}
}


//função 10
void mul_exec(CPU *cpu, RAM *ram){
	cpu->reg.MAR = cpu->reg.IR & 0x0F; // Configurar MAR com os 4 bits mais baixos do IR (endereço de memória)
	enviar_dado(cpu->bus, ram, cpu->reg.MAR, 0, "READ"); // Enviar o endereço (MAR) para o barramento e ler o dado da memória
	cpu -> reg.MBR = cpu->bus->dado & 0x00FF;// Carregar o dado lido no MBR, mantendo apenas os 8 bits menos significativos
	cpu->reg.Y = cpu -> reg.AC; // Salvar o valor do AC em Y (registro temporário)
	ULA_MUL((int8_t *)&cpu->reg.Z, (int8_t *)&cpu->reg.Y, (int8_t *)&cpu->reg.MBR, (int8_t *)&cpu->reg.FLAG);// Realizar a operação de multiplicação entre Y e MBR, armazenando o resultado em Z e Y, e atualizando a FLAG
	cpu->reg.AC = cpu->reg.Y;// Restaurar o valor de Y no AC após a multiplicação
	if (cpu->reg.FLAG == 1) {// Se a FLAG indicar erro (valor 1), ajustar o ICC para 3
		cpu->reg.ICC = 3;  
	} else {
		cpu->reg.ICC = 0;  
	}
}

//função 11
void div_exec(CPU *cpu, RAM *ram){
// Configurar MAR com os 4 bits mais baixos do IR (endere�o de mem�ria)
    cpu->reg.MAR = cpu->reg.IR & 0x0F;

    // Enviar o endere�o (MAR) para o barramento e ler o dado da mem�ria
    enviar_dado(cpu->bus, ram, cpu->reg.MAR, 0, "READ");

    // Carregar o dado lido no MBR, mantendo apenas os 8 bits menos significativos
    cpu->reg.MBR = cpu->bus->dado & 0x00FF;

    // Verificar se o divisor (MBR) � zero
    if (cpu->reg.MBR == 0x00) {
        // Se o divisor for zero, gerar erro de divis�o por zero
        cpu->reg.FLAG = 2;  
        cpu->reg.ICC = 3;   
        printf(">>>ERRO! DIVIS�O POR ZERO\n");
    } else {
        // Se o divisor n�o for zero, executar a divis�o
        cpu->reg.Y = cpu->reg.AC;  // Adicionar o valor do AC em Y
        
        // Realizar a opera��o de divis�o entre Y e MBR, armazenando o resultado em Ze Y, e atualizando a FLAG
        ULA_DIV((int8_t *)&cpu->reg.Z, (int8_t *)&cpu->reg.Y, (int8_t *)&cpu->reg.MBR, (int8_t *)&cpu->reg.FLAG);

        // Restaurar o valor de Y no AC ap�s a divis�o
        cpu->reg.AC = cpu->reg.Y;

        // Se a FLAG indicar erro (valor 1), ajustar o ICC para 3
        if (cpu->reg.FLAG == 1) {
            cpu->reg.ICC = 3;  
        } else {
            cpu->reg.ICC = 0;  
        }
    }
}


void out_exec(CPU *cpu, RAM * ram){
	cpu->reg.OTR = cpu->reg.AC;
	cpu->reg.PC-=2;
	ciclo_busca(cpu, ram);	
	if((cpu->reg.IR&0xF0)==MUL)
		printf("DISPLAY\tMUL: REG Z: %d\tREG OTR:%d\tZOTR: %d\n",(int8_t)cpu->reg.Z, (int8_t)cpu->reg.OTR,(int16_t) (((cpu->reg.Z & 0x00FF) << 8) | (cpu->reg.OTR & 0x00FF)));
	else if((cpu->reg.IR&0xF0)==DIV)
		printf("DISPLAY\tDIV: Resto em REG Z: %d\tQuociente em REG OTR:%d\n",(int8_t)cpu->reg.Z, (int8_t)cpu->reg.OTR);
	else
		printf(" DISPLAY: %d\n", (int8_t) cpu->reg.OTR);
	
	cpu->reg.PC++;
	cpu->reg.ICC = 0;
}

