#ifndef ALGORITMO_DE_REMPLAZO_H_
#define ALGORITMO_DE_REMPLAZO_H_

#include <commons/collections/list.h>
#include <string.h>
#include "../include/tad_memoria.h"
#include "../include/config_servicio.h"
#include "../include/get_pagina.h"

t_page * busqueda_por_LRU(int proceso_id);
t_page * busquedaPorClockModificado(int proceso_id);

#endif /* algoritmos */