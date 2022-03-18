#include "algoritmos.h"

double estimacion_rafaga(PCB_Carpincho * pcb);
double estimacion_hrrn(PCB_Carpincho * pcb);

PCB_Carpincho * algoritmo_sjf(){
    PCB_Carpincho * pcb;
    PCB_Carpincho * pcb_seleccionado = list_get(list_READY, 0);

    double estimacion = estimacion_rafaga(pcb_seleccionado);
    double estimacion_aux;

    for (size_t i = 0; i < list_size(list_READY); i++)
    {
        pcb = list_get(list_READY, i);
        estimacion_aux = estimacion_rafaga(pcb);

        if (estimacion_aux < estimacion)
        {
            pcb_seleccionado = pcb;
            estimacion = estimacion_aux;
        }
        
    }

    pcb_seleccionado->estimacion_anterior = estimacion;
    pcb_seleccionado->inicio_ejecucion_time = get_time_now_MS();
    
    return pcb_seleccionado;
}

PCB_Carpincho * algoritmo_hrrn(){
    PCB_Carpincho * pcb;
    PCB_Carpincho * pcb_seleccionado = list_get(list_READY, 0);

    double estimacion = estimacion_hrrn(pcb_seleccionado);
    double estimacion_aux;

    for (size_t i = 0; i < list_size(list_READY); i++)
    {
        pcb = list_get(list_READY, i);
        estimacion_aux = estimacion_hrrn(pcb);

        if (estimacion_aux > estimacion)
        {
            pcb_seleccionado = pcb;
            estimacion = estimacion_aux;
        }
        
    }

    pcb_seleccionado->estimacion_anterior = estimacion;
    return pcb_seleccionado;
}

/* funciones privadas */
double estimacion_rafaga(PCB_Carpincho * pcb){
    double estimacion =  config_kernel->ALPHA * pcb->rafaga_anterior + (1 - config_kernel->ALPHA) * pcb->estimacion_anterior;

    return estimacion;
}

double estimacion_hrrn(PCB_Carpincho * pcb){
    double prox_rafaga = estimacion_rafaga(pcb);
    long time_now = get_time_now_MS();
    long time_espera = time_now - pcb->inicio_ejecucion_time;

    double estimacion = (prox_rafaga + time_espera) / time_espera;

    return estimacion;
}

PCB_Carpincho* get_last_blocked(){
	pthread_mutex_lock(&MX_BLOCKED);
	int table_length = list_size(list_BLOCKED);
	pthread_mutex_unlock(&MX_BLOCKED);

	PCB_Carpincho* pcb = list_get(list_BLOCKED, table_length-1); 
	return pcb;
}

PCB_Carpincho* get_first_pcb(t_list* pcbs){
    PCB_Carpincho* pcb = list_get(pcbs, 0); 
    list_remove(pcbs, 0);
	return pcb;
}

PCB_Carpincho* proximo_carpincho_new(){
    pthread_mutex_lock(&MX_NEW);
    PCB_Carpincho* item = list_get(list_NEW, 0);
    pthread_mutex_unlock(&MX_NEW);
    return item;
}

PCB_Carpincho* get_first_suspended_ready(){
    int index = 0;
    pthread_mutex_lock(&MX_SUSPENDED_READY);
    PCB_Carpincho* item = list_get(list_SUSPENDED_READY, index);
    list_remove(list_SUSPENDED_READY, index);
    pthread_mutex_unlock(&MX_SUSPENDED_READY);
    return item;
}

PCB_Carpincho* proximo_carpincho_to_ready(){
    if(list_size(list_SUSPENDED_READY) > 0){
        return get_first_suspended_ready(); 
    }else
        return proximo_carpincho_new();
} 