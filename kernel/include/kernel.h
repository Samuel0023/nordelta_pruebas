#ifndef KERNEL
#define KERNEL_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include "../include/config_utils.h"
#include "../include/config_kernel.h"
#include <commons/collections/list.h>
#include "../include/tad_kernel.h"
#include "utils.h"
#include "planificadores.h"
#include "../include/tad_kernel.h"
#include "server_utils.h"
#include <pthread.h>
#include "server.h"


void inicializacion_estructuras();
void liberar_memoria();

extern t_list *list_NEW;
extern t_list *list_READY;
extern t_list *list_EXEC;
extern t_list *list_SUSPENDED_READY;
extern t_list *list_SUSPENDED_BLOCKED;
extern t_list *list_BLOCKED;
extern t_list *lista_EXIT;

extern pthread_mutex_t MX_NEW;
extern pthread_mutex_t MX_READY;
extern pthread_mutex_t MX_EXEC;
extern pthread_mutex_t MX_SUSPENDED_READY;
extern pthread_mutex_t MX_SUSPENDED_BLOCKED;
extern pthread_mutex_t MX_BLOCKED;
extern pthread_mutex_t MX_EXIT;

extern pthread_mutex_t MX_PCB;
extern sem_t limite_grado_multiprocesamiento;
extern sem_t limite_grado_multiprogramacion;
extern sem_t cantidad_procesos_nuevos_suspendidos;
extern sem_t cant_procesos_listos;
extern sem_t cantidad_procesos_suspendidos;

extern t_log *logger_kernel;
t_config_kernel *config_kernel;

#endif /* SERVER_H_ */
