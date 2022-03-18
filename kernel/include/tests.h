#ifndef TESTS_H
#define TESTS_H
#include <CUnit/Basic.h>
#include "TAD.h"
#include <stdlib.h>
#include <stdio.h>
#include "sd_memalloc.h"
#include "sd_memfree.h"
#include "sd_memwrite.h"
#include "../include/utils.h"
#include "sd_semaphore.h"
#include "planificadores.h"
#include "../include/tad_kernel.h"
#include "../include/algoritmos.h"
#include "../include/semaforos_kernel.h"
#include "config_kernel.h"
#include <semaphore.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include "sd_mate_close.h"
#include "../include/sd_mate_call_io.h"
#include "sd_mate_init.h"
#include "sd_res_memoria.h"
#include "deadlock.h"
#include "mate_call_io.h"

int run_tests();
void cambio_de_estado_carpincho();
void testeo_proximo_carpincho_new();
void validar_mediano_plazo();
void validar_sd_memalloc();
void validar_sd_memfree();
void validar_sd_memwrite();
void validar_convert_time_a_ms();
void validar_algoritmo();
void semaforo_struct();

//semaforos
void validar_semaphores_kernel();
void validar_deadlock();
void validar_sd_respuesta_id_mate_init();
void validar_inicializar_carpincho_mate_init();

extern t_config_kernel *config_kernel;

extern t_list *list_NEW;
extern t_list *list_READY;
extern t_list *list_EXEC;
extern t_list *list_SUSPENDED_READY;
extern t_list *list_SUSPENDED_BLOCKED;
extern t_list *list_BLOCKED;
extern t_list *lista_EXIT;
extern t_list *devices;

extern pthread_mutex_t MX_NEW;
extern pthread_mutex_t MX_ID_PROCESO;
extern sem_t cantidad_procesos_nuevos_suspendidos;
extern int id_proceso; 

#endif