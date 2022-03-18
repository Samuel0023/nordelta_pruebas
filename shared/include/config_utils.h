#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <netdb.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <string.h>
#include "TAD.h"

typedef enum
{
    SFJ,
    HRRN
} alg_planificacion;

// typedef struct
// {
//     char *IP;
//     int PUERTO;
//     char *IP_MEMORIA;
//     int PUERTO_MEMORIA;
//     alg_planificacion ALGORITMO_PLANIFICACION;

//     t_list *DISPOSITIVOS_IO; //DISPOSITIVOS_IO=[laguna, hierbitas]
//     t_list *DURACIONES_IO;   //DURACIONES_IO=[25000, 1000]

//     int GRADO_MULTIPROGRAMACION;
//     int GRADO_MULTIPROCESAMIENTO;

//     int TIEMPO_DEADLOCK;
//     //char *ARCHIVO_LOG;
// } t_config_kernel;

//TODO :despues borrar
typedef struct
{

    char *IP_MODULO;
    int PUERTO;
    char *PUNTO_MONTAJE;
    int TIEMPO_SINCRONIZACION;
    t_list *POSICIONES_SABOTAJE;
    int BLOCK_SIZE;
    int BLOCKS;
    char *IP_DISCORDIADOR;
    int PUERTO_DISCORDIADOR;
    char *ARCHIVO_LOG;
} t_config_mongo_store;
t_config_mongo_store *leer_config_mongo_store(t_config *config);
//
//t_config_kernel *leer_config_kernel(t_config *kernel);
t_config *leer_config_file(char *);

#endif