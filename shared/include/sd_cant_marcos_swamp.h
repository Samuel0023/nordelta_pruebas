#ifndef SD_CANT_MARCOS_SWAMP_H
#define SD_CANT_MARCOS_SWAMP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <string.h>
#include "TAD.h"
#include "socket_client.h"

t_package ser_cant_marcos_swamp(t_cant_marcos_swamp data_buffer);
void peticion_cant_marcos_swamp(int conexionSWAMP);
int respuesta_cant_marcos_swamp(int conexionSWAMP);

extern int conexionSWAMP;

#endif