#ifndef MATE_INIT_H_
#define MATE_INIT_H_

#include "tad_kernel.h"
#include "global.h"
#include <socket_client.h>
#include <sd_mate_init.h>

int inicializar_carpincho_nuevo(double estimacion_anterior_config, int socket_carpincho);
void enviar_id(int socket, int id_carpincho);

#endif