#include "barramentos.h"

#include <string.h>

void enviar_dado(Barramento *bus, RAM *ram, uint16_t endereco, uint16_t dado, const char *controle) {
    bus->endereco = endereco&0x00FF;
    bus->dado = dado&0x00FF;
    strcpy(bus->controle, controle);
    if(strcmp(bus->controle,"READ")==0) bus->dado = ler_memoria(ram, bus->endereco&0x000F);
    if(strcmp(bus->controle,"WRITE")==0) escrever_memoria(ram, bus->endereco, bus->dado);
}

int ler_dado(Barramento *bus, int endereco) {
    if (bus->endereco == endereco) {
        return bus->dado;
    }
    return -1; // Endereço inválido
}

