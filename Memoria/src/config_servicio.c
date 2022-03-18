#include "config_servicio.h"

char * get_ip(){
    char * ip = config_get_string_value(config,"IP");
    return ip;
}

char * get_puerto_swamp(){
    char * puerto = config_get_string_value(config,"PUERTO_SWAMP");
    return puerto;
}

char * get_ip_swamp(){
    char * ip = config_get_string_value(config,"IP_SWAMP");
    return ip;
}

char * get_puerto(){
    char * puerto = config_get_string_value(config,"PUERTO");
    return puerto;
}

int get_tamanio(){
    int * puerto = config_get_int_value(config,"TAMANIO");
    return puerto;
}

int get_tamanio_pagina(){
    int tamanio_pagina = config_get_int_value(config,"TAMANIO_PAGINA");
    return tamanio_pagina;
}

char * get_tipo_asignacion(){
    char * value = config_get_string_value(config,"TIPO_ASIGNACION");
    return value;
}

int get_marcos_por_carpincho(){
    int value = config_get_int_value(config,"MARCOS_POR_CARPINCHO");
    return value;
}

char * get_algoritmo_reemplazo_mmu(){
    char * value = config_get_string_value(config,"ALGORITMO_REEMPLAZO_MMU");
    return value;
}

int get_cantidad_entradas_tlb(){
    int value = config_get_int_value(config,"CANTIDAD_ENTRADAS_TLB");
    return value;
}

char * get_algoritmo_reemplazo_tlb(){
    char * value = config_get_string_value(config,"ALGORITMO_REEMPLAZO_TLB");
    return value;
}

int get_retardo_acierto_tlb(){
    int value = config_get_int_value(config,"RETARDO_ACIERTO_TLB");
    return value;
}

int get_retardo_fallo_tlb(){
    int value = config_get_int_value(config,"RETARDO_FALLO_TLB");
    return value;
}

char* get_path_dump_tlb(){
    char * value = config_get_string_value(config,"PATH_DUMP_TLB");
    return value;
}

void show_values_config(){
    printf("IP: %s\n", get_ip());
    printf("Puerto: %s\n", get_puerto());
    printf("Tamanio: %d\n", get_tamanio());
    printf("Tamanio pagina: %d\n", get_tamanio_pagina());
    printf("Tipo asignacion: %s\n", get_tipo_asignacion());
    printf("Marcos por carprincho: %d\n", get_marcos_por_carpincho());
    printf("Algoritmo reemplazo MMU: %s\n", get_algoritmo_reemplazo_mmu());
    printf("Cantidad entradas TLB: %d\n", get_cantidad_entradas_tlb());
    printf("Algoritmo reemplazo TLB: %s\n", get_algoritmo_reemplazo_tlb());
    printf("Retardo acierto TLB: %d\n", get_retardo_acierto_tlb());
    printf("Retardo fallo TLB: %d\n", get_retardo_fallo_tlb());
    printf("Path dump tlb: %s\n", get_path_dump_tlb());
}