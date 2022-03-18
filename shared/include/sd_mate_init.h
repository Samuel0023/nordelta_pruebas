#ifndef SD_MATE_INIT_H
#define SD_MATE_INIT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "TAD.h"
#include <commons/string.h>
#include "sd_entero.h"

t_carpincho_init des_carpincho_init(t_package paquete);
t_package ser_respuesta_id(int pid);
int des_respuesta_id(t_package paquete);

#endif