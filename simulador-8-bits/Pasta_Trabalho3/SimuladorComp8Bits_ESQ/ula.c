#include "ula.h"
#include "cpu.h"
#include "registradores.h"
#include <stdio.h>
//Somador Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 334
static void somadorCompleto (uint8_t A, uint8_t B, uint8_t Cin, uint8_t * co, uint8_t * s){
	*co = (A&B) | (A&Cin) | (B&Cin);
	//*s = A ^ B ^ Cin;
	*s = (~A & ~B & Cin) | (~A & B & ~Cin) | (A & B & Cin) | (A & ~B & ~Cin);
}

//O somador bin�rio da ULA trata os dois n�meros como inteiros sem sinal.
//Entradas: Registrados A, B e cin(CarryIn), Sa�da: s(soma) e co(CarryOut)
static void somador8bits(uint8_t A, uint8_t B, uint8_t cin, uint8_t * co, uint8_t * s){
	*co = cin;
	uint8_t soma;
	for (int i=0;i<8;i++){
		somadorCompleto(bit_get(A,0),bit_get(B,0), *co, co,&soma);
		A = A>>1;//deslocando os bits para direita a cada intera��o
		B = B>>1;
		if(soma==1)bit_set(*s,i);
		else bit_clr(*s,i);
	}
}

//Realiza o complemento de 2 do Registrador A e grava em A
static void complementador(uint8_t *A){

	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 285
	//Invertendo os bits
    for(int i=0;i<8;i++){
        bit_inv(*A,7-i);
	}

	uint8_t resultado=0,carry=0;
	//Chamando a função somador8bits para soma 1
	somador8bits(*A, 1, 0, &carry, &resultado);
	//Passando o resultado para A
	*A=resultado;

}

//Adi��o de A e B e grava resultado em A
void ULA_ADD(int8_t *A, int8_t * B, int8_t * overflow){
	CPU *cpu;
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287

    // Soma A com B
    uint8_t carryOut;
    uint8_t resultado;
    somador8bits((uint8_t)(*A), (uint8_t)(*B), 0, &carryOut, &resultado);

	// Verificar overflow
    if ( (*A + *B)>128 || (*A + *B)<-127){
        *overflow = 1;
    }
	else {
        *overflow = 0;
    }

    // Adiciona o resultado em A
    *A = (int8_t)resultado;

	cpu->reg.FLAG=*overflow;//Atualiza a FLAG

}

//Subtra��o de A e B e grava resultado em A
void ULA_SUB(int8_t *A, int8_t * B, int8_t * overflow){
	CPU *cpu;
	//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 287
	// Calcula o complemento de dois de B

    complementador((uint8_t *)B);  // Chama a funcao complementador

    // Soma A com o complemento de B
    uint8_t carryOut;
    uint8_t resultado;
    somador8bits((*A), (*B), 0, &carryOut, &resultado);

	// Verificar overflow
    if ( (*A + *B)>128 || (*A + *B)<-127){
        *overflow = 1;
    }
	else {
        *overflow = 0;
    }

    // Adiciona o resultado em A'
    *A = (int8_t)resultado;

	cpu->reg.FLAG=*overflow;//Atualiza a FLAG
}

//Multiplica��o de Q(8bits) com M(8bits) gera resultado de 16bits que est� em A(8bits) e Q(8bits)
void ULA_MUL(int8_t *A, int8_t *Q, int8_t * M, int8_t * overflow){
	CPU *cpu;
	*overflow = 0;
	int8_t Q_m1 = 0; // cria o q-1(bit à direira de q0)
	int8_t resultado = 0;
	int8_t carryOut = 0; 
	int8_t verifica_bit_Q_0;
	int8_t verifica_bit_A_0;
	int8_t verifica_bit_A_7;
	*A = 0;

	for(int i = 0; i<8; i++){ // Simula o contador

		// n de bits = n de operações
		verifica_bit_Q_0 = bit_get(*Q,0);
		verifica_bit_A_0 = bit_get(*A, 0);
		verifica_bit_A_7 = bit_get(*A, 7);
		if(verifica_bit_Q_0 == 0 && Q_m1 == 1){
			somador8bits((*A),(*M), 0, &carryOut, &resultado); // soma A + M e retorna o resultado
			*A = resultado; // guarda o resultado em A

			//deslocar
			verifica_bit_Q_0 = bit_get(*Q,0);
			verifica_bit_A_0 = bit_get(*A, 0);
			verifica_bit_A_7 = bit_get(*A, 7);
			Q_m1 = verifica_bit_Q_0;
			if(verifica_bit_A_0 == 1){
				*Q = *Q>>1;
				bit_set(*Q, 7);
				if(verifica_bit_A_7 == 1){
					*A = *A>>1;
					bit_set(*A, 7);
				}
				else{
					*A = *A>>1;
					bit_clr(*A, 7);
				}
			}
			else{
				*Q = *Q>>1;
				bit_clr(*Q, 7);
				if(verifica_bit_A_7 == 1){
					*A = *A>>1;
					bit_set(*A, 7);
				}
				else{
					*A = *A>>1;
					bit_clr(*A, 7);
				}
			}
		}

		else if(verifica_bit_Q_0 == 1 && Q_m1 == 0){

			// chama o complementador para calcular -M
			complementador((int8_t *)M); // calcula e retorna -M
			somador8bits((*A),(*M), 0, &carryOut, &resultado); // soma A +(-M) e retorna o resultado
			*A = resultado; // guarda o resultado em A
			complementador((int8_t *)M); // calcula e retorna M novamente para M

			//deslocar
			verifica_bit_Q_0 = bit_get(*Q,0);
			verifica_bit_A_0 = bit_get(*A, 0);
			verifica_bit_A_7 = bit_get(*A, 7);
			Q_m1 = verifica_bit_Q_0;
			if(verifica_bit_A_0 == 1){
				*Q = *Q>>1;
				bit_set(*Q, 7);
				if(verifica_bit_A_7 == 1){
					*A = *A>>1;
					bit_set(*A, 7);
				}
				else{
					*A = *A>>1;
					bit_clr(*A, 7);
				}
			}
			else{
				*Q = *Q>>1;
				bit_clr(*Q, 7);
				if(verifica_bit_A_7 == 1){
					*A = *A>>1;
					bit_set(*A, 7);
				}
				else{
					*A = *A>>1;
					bit_clr(*A, 7);
				}
			}
		}

		else{
			// caso onde verifica_bit_Q_0 = 0 e Q_m1 = 0, ou, verifica_bit_Q_0 = 1 e Q_m1 = 1
			// apenas deslocar
			verifica_bit_Q_0 = bit_get(*Q,0);
			verifica_bit_A_0 = bit_get(*A, 0);
			verifica_bit_A_7 = bit_get(*A, 7);
			Q_m1 = verifica_bit_Q_0;
			if(verifica_bit_A_0 == 1){
				*Q = *Q>>1;
				bit_set(*Q, 7);
				if(verifica_bit_A_7 == 1){
					*A = *A>>1;
					bit_set(*A, 7);
				}
				else{
					*A = *A>>1;
					bit_clr(*A, 7);
				}
			}
			else{
				*Q = *Q>>1;
				bit_clr(*Q, 7);
				if(verifica_bit_A_7 == 1){
					*A = *A>>1;
					bit_set(*A, 7);
				}
				else{
					*A = *A>>1;
					bit_clr(*A, 7);
				}
			}
		}
	}
	cpu->reg.FLAG=*overflow;//Atualiza a FLAG
}
//Baseado no livro Willian Stallings 10ed, cap�tulo 10, pag. 296
void ULA_DIV(int8_t *A, int8_t *Q, int8_t * M, int8_t * overflow){
	CPU *cpu;
	*overflow = 0;
	int8_t carryOut, resultado, comp_M, aux;
	//guarda o sinal de Q e M
	int sinal_quo = 1, sinal_div = 1, sinal_Q;
	
	if(*Q<0){
		sinal_quo = -1;
	}
	if(*M<0){
		sinal_div = -1;
	}

	*A = 0;

	//Convertendo Q e M para valores positivos
	if (*Q<0) {
		complementador((int8_t *)Q);
	}
	if (*M<0) {
		complementador((int8_t *)M);
	}
	//Analisando os casos simples
	
	for(int i = 0; i<8; i++){
		// verificar se M =0, pois não existe divisão por 0
		if(*M == 0){
			*A = *Q;
			*Q = 1;
			break;
		}
		if(bit_get(*Q, 7) == 1){
			*A = *A<<1;
			bit_set(*A, 0);
			*Q = *Q<<1;
		}
		else{
			*A = *A<<1;
			*Q = *Q<<1;
		}
		complementador((int8_t *)M); // torna o M negativo
		somador8bits((*A), (*M), 0, &carryOut, &resultado); // faz A = A - M ou A = A +(-M)
		*A = resultado; // guarda em A
		if(*A<0){ 
			bit_clr(*Q, 0);
			complementador((int8_t *)M); // torna M positivo novamente
			somador8bits((*A), (*M), 0, &carryOut, &resultado); // faz A = A + M (restaura)
			*A = resultado;
		}
		else{
			complementador((int8_t *)M); // torna o M positivo, caso nao caia no if
			bit_set(*Q, 0);
		}
	}
	if(sinal_quo == -1){
		complementador((int8_t *)A);
	}
	if(sinal_quo != sinal_div){
		complementador((int8_t *)Q);
	}
	cpu->reg.FLAG=*overflow;//Atualiza a FLAG
}


