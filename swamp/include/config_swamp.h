#ifndef CONFIG_SWAMP_H_
#define CONFIG_SWAMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <string.h>
#include "TAD.h"

typedef struct
{
    char *IP;
    int PUERTO;
    int TAMANIO_SWAP;
    int TAMANIO_PAGINA;
    char *ARCHIVOS_SWAP;
    char *TIPO_ASIGNACION;
    int MARCOS_MAXIMOS;
    int RETARDO_SWAP;

} t_config_swamp;

t_config_swamp *leer_config_swamp(t_config *config);

#endif