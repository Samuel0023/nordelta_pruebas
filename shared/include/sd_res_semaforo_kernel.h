#ifndef SD_RES_MATE_SEM_H
#define SD_RES_MATE_SEM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <string.h>
#include "TAD.h"

/* respuesta de kernel segun la operacio de semaforo */
t_package ser_res_mate_sem_init(int code_res);
t_package ser_res_mate_sem_wait(int code_res);
t_package ser_res_mate_sem_post(int code_res);
t_package ser_res_mate_sem_destroy(int code_res);

int des_respuesta_kernel_sem(t_package paquete);

#endif