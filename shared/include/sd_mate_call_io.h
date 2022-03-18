#ifndef SD_MATE_CALL_IO_H
#define SD_MATE_CALL_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <string.h>
#include "TAD.h"

t_package ser_msj_code_mete_call_io(t_mate_call_io data_buffer);
t_mate_call_io des_msj_code_mete_call_io(t_package paquete);

#endif