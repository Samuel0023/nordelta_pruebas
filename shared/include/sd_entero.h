#ifndef SD_ENTERO_H
#define SD_ENTERO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "TAD.h"
#include <commons/string.h>

t_package ser_entero(int value, op_code cod_operacion);
int des_entero(t_package paquete);

#endif