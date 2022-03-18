#ifndef SERVER_H_
#define SERVER_H_
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utils_swamp.h"
#include "tad_swamp.h"
#include "server_utils.h"
#include "sd_entero.h"
#include "TAD.h"
#include "sd_messages.h"
#include "sd_lectura_pagina.h"
#include "operationIO.h"
#include "../include/config_swamp.h"
#include <fcntl.h>
#include <sys/mman.h>
extern int flag_terminar_swamp;
extern t_config_swamp *config;
void iterator(t_log *logger, char *value);
void iniciar_servidor_swamp(char *ip, char *puerto);

#endif /* SERVER_H_ */