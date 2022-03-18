#ifndef ALGORITMO_H
#define ALGORITMO_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../include/tad_kernel.h"
#include <commons/collections/list.h>
#include "config_kernel.h"
#include "../include/utils.h"
#include <commons/temporal.h>

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

extern t_config_kernel *config_kernel;

PCB_Carpincho * algoritmo_sjf();
PCB_Carpincho * algoritmo_hrrn();
PCB_Carpincho* get_last_blocked();
PCB_Carpincho* get_first_pcb(t_list* pcbs);
PCB_Carpincho* proximo_carpincho_to_ready();

#endif