#include "computador.h"

void inicializar_computador(Computador *comp) {
    inicializar_cpu(&comp->cpu, &comp->bus);
    inicializar_memoria(&comp->ram);
}

void executar_programa(Computador *comp) {
    executar_ciclo(&comp->cpu, &comp->ram);
}

