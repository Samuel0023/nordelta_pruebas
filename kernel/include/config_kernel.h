#ifndef CONFIG_KERNEL_H_
#define CONFIG_KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <string.h>
#include "TAD.h"
#include "config_utils.h"

typedef struct
{
    char* dispositivo;      //DISPOSITIVOS_IO=[laguna, hierbitas]
    int *duration;          //DURACIONES_IO=[25000, 1000]
} t_device_config;

typedef struct
{
    char *IP;
    int PUERTO;
    char *IP_MEMORIA;
    int PUERTO_MEMORIA;
    alg_planificacion ALGORITMO_PLANIFICACION;
    float ALPHA;

    t_list * devices; //[t_device_config]

    int GRADO_MULTIPROGRAMACION;
    int GRADO_MULTIPROCESAMIENTO;

    int TIEMPO_DEADLOCK;
    int ESTIMACION_INICIAL;
    char *ARCHIVO_LOG;
} t_config_kernel;

t_config_kernel *leer_config_kernel(t_config *config);
void free_config_kernel(t_config_kernel *config_kernel);
void show_config_kernel(t_config_kernel *config);

#endif