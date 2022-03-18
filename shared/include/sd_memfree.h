#ifndef SD_MEMFREE_H
#define SD_MEMFREE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h> //
#include <commons/config.h>
#include <string.h>
#include "TAD.h"

/* MSJ de kernel */
t_package ser_msj_code_memfree(t_msj_memfree data_alloc);

/* MSJ de memoria */
t_msj_memfree des_msj_code_memfree(t_package paquete);

#endif