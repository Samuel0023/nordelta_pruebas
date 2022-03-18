#ifndef UTILS
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <string.h>
#include "../include/tad_kernel.h"
#include "TAD.h"
#include <commons/temporal.h>
#include "sd_entero.h"
#include <commons/string.h>

extern t_log *logger_kernel;
extern t_list *list_NEW;
extern t_list *list_READY;
extern t_list *list_EXEC;
extern t_list *list_SUSPENDED_READY;
extern t_list *list_SUSPENDED_BLOCKED;
extern t_list *list_BLOCKED;
extern t_list *lista_EXIT;
extern sem_t cantidad_procesos_nuevos_suspendidos;

extern pthread_mutex_t MX_NEW;
extern pthread_mutex_t MX_READY;
extern pthread_mutex_t MX_EXEC;
extern pthread_mutex_t MX_SUSPENDED_READY;
extern pthread_mutex_t MX_SUSPENDED_BLOCKED;
extern pthread_mutex_t MX_BLOCKED;
extern pthread_mutex_t MX_EXIT;

int esperar_cliente(int);
t_list *recibir_paquete_tp0(int);
void recibir_mensaje_tp0(int);
void activar_mutex_estado(status_carpincho estado);
void desactivar_mutex_estado(status_carpincho estado);
long get_time_now_MS();
long convert_time_ms(char * time_now);

t_list *get_list_by_state(status_carpincho estado);
PCB_Carpincho *getByID(t_list *lista, int proceso_id);
PCB_Carpincho *find_pcb_in_kernel(int proceso_id);

PCB_Carpincho *list_remove_proceso(t_list *lista, int proceso_id, status_carpincho estado);
void cambiar_estado(PCB_Carpincho *proceso, status_carpincho estado_destino);
PCB_Carpincho* get_last_blocked();
void pasar_a_swap(PCB_Carpincho* pcb, int socket_memoria);

#endif /* UTILS_H_ */
