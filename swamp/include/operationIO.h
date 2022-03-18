#ifndef OPERATION_IO_H_
#define OPERATION_IO_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utils_swamp.h"
#include "tad_swamp.h"
#include "server_utils.h"
#include "TAD.h"
#include "sd_messages.h"
#include "sd_lectura_pagina.h"
#include "../include/config_swamp.h"
#include <fcntl.h>
#include <sys/mman.h>

extern int flag_terminar_swamp;
extern t_config_swamp *config;
extern t_list* t_paths;

void* lectura_pagina(PageSwap pagina_swap);
void guardarContenido(PageSwap page_swap);

#endif