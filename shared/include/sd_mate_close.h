#ifndef SD_MATE_CLOSE_H
#define SD_MATE_CLOSE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <string.h>
#include "TAD.h"

t_package ser_msj_code_mete_close(t_mate_close data_buffer);
t_mate_close des_msj_code_mete_close(t_package paquete);

#endif