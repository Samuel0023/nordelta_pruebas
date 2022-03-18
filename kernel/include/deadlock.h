#ifndef DEADLOCK_H
#define DEADLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <commons/log.h>
#include <fcntl.h>           
#include "utils.h"
#include "TAD.h"
#include "semaforos_kernel.h"
#include "tad_kernel.h"

typedef enum{
    ASIGNACIONES=0,
    PETICIONES=1,
}t_matriz_op;

typedef struct{
    int proceso;
    int semaforo;
    int value;
}t_celda;
extern t_list *list_BLOCKED;
extern t_list *asignaciones; //lista de dupla [procesos][semaforos]
extern t_list *peticiones; //lista de dupla [procesos][semaforos]
extern t_list *semaphores_kernel;          //[t_semaphore]   
int *recursos_totales;
int *recursos_disponibles;

void aplicar_deadlock();
bool _existe_celda(int x,int y);
bool es_un_proceso_bloqueado(int id_proceso);
bool vector_de_asignaciones_vacio(int id_proceso);
void increase_requests(int pos_x, int pos_y);
void increase_allocations(int pos_x, int pos_y);
void decrease_allocations(int pos_x, int pos_y);
void decrease_allocations(int pos_x, int pos_y);

void increase(t_matriz_op,int pos_x, int pos_y);
void decrease(t_matriz_op,int pos_x, int pos_y);
void set_recursos_totales();
int sum_recursos_semaphore(int sem);
void set_recursos_disp();
void makeCelda(int x,int y,int value);
t_celda *getCelda(int x,int y,t_matriz_op);

void remove_id(int id,t_list* list);
int *get_vector_de_asignaciones_segun(int procesoID);
t_list* get_vector_de_peticiones_segun(int procesoID);
bool existe_id(int id_buscado,t_list* lista);
t_list *get_procesos_id();
bool satisface(t_list* vector_proceso);
void simulacionAsignacion(int proceso_id);
t_list *obtener_candidatos_a_ser_deadlock(t_list* lista_de_ids);
t_list *getProcesosEnDeadlock();
void agregar_motivo_de_retencion(PCB_Carpincho *carpincho,int semaphore_id);
void agregar_retenidos(int process_id,int semaphores_id);
void liberar_motivo_retencion(PCB_Carpincho *carpincho, int id_semaphore);
#endif





