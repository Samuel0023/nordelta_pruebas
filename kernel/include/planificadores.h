#ifndef PLANIFICADORES_H_
#define PLANIFICADORES_H_

#include "algoritmos.h"
#include "tad_kernel.h"
#include "algoritmos.h"

extern t_list *list_NEW;
extern t_list *list_READY;
extern t_list *list_EXEC;
extern t_list *list_SUSPENDED_READY;
extern t_list *list_SUSPENDED_BLOCKED;
extern t_list *list_BLOCKED;
extern t_list *lista_EXIT;

extern pthread_mutex_t MX_PCB;
extern pthread_mutex_t MX_NEW;
extern pthread_mutex_t MX_READY;
extern pthread_mutex_t MX_EXEC;
extern pthread_mutex_t MX_SUSPENDED_READY;
extern pthread_mutex_t MX_SUSPENDED_BLOCKED;
extern pthread_mutex_t MX_BLOCKED;
extern pthread_mutex_t MX_EXIT;

extern sem_t cantidad_procesos_suspendidos;
extern sem_t cantidad_procesos_nuevos_suspendidos;
extern sem_t cant_procesos_listos;
extern sem_t limite_grado_multiprogramacion;
extern sem_t limite_grado_multiprocesamiento;

void inicializacion_hilos();
void planificador_largo_plazo();
void planificador_mediano_plazo();
void planificador_corto_plazo();
void planificador_dispositivo(t_device* device);
void algoritmo_deadlock();
PCB_Carpincho* proximo_carpincho_listo();

extern t_log *logger_kernel;
extern t_config_kernel *config_kernel;
extern t_list* devices;

#endif /* PLANIFICADORES_H_ */
