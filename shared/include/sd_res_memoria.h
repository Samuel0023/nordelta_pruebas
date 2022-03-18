#ifndef SD_RES_MEMORIA_H
#define SD_RES_MEMORIA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <string.h>
#include "TAD.h"

/* respuesta de memoria */
t_package ser_res_memalloc(int code_res);
t_package ser_res_memfree(int code_res);
t_package ser_res_memwrite(int code_res);
t_package ser_res_memread(int code_res);

int des_respuesta_memoria(t_package paquete);

#endif