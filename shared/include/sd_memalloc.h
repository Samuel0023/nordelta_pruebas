#ifndef SD_MEMALLOC_H
#define SD_MEMALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <string.h>
#include "TAD.h"

/* MSJ de kernel */
t_package ser_msj_code_memalloc(t_msj_memalloc data_alloc);
/* MSJ de memoria */
t_msj_memalloc des_msj_code_memalloc(t_package paquete);

#endif