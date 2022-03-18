#ifndef SD_MEMWRITE_H
#define SD_MEMWRITE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h> //
#include <commons/config.h>
#include <string.h>
#include "TAD.h"

/* MSJ de kernel */
t_package ser_msj_code_memwrite(t_msj_memwrite data);
t_package ser_msj_code_memread(t_msj_memwrite data);

/* MSJ de memoria */
t_msj_memwrite des_msj_code_memwrite(t_package paquete);

#endif