#include "planificadores.h"

void inicializacion_hilos(){
    pthread_t thread_largo_plazo;
    pthread_create(&thread_largo_plazo, NULL, planificador_largo_plazo, NULL);
	pthread_detach(thread_largo_plazo);

    pthread_t thread_corto_plazo;
    pthread_create(&thread_corto_plazo, NULL, planificador_corto_plazo, NULL);
	pthread_detach(thread_corto_plazo);

    pthread_t thread_mediano_plazo;
    pthread_create(&thread_mediano_plazo, NULL, planificador_mediano_plazo, NULL);
	pthread_detach(thread_mediano_plazo);

    for(int c=0; c<list_size(devices);c++){
        t_device* device = list_get(devices, c);
        pthread_t thread_device;
        pthread_create(&thread_device, NULL, planificador_dispositivo, device);
        pthread_detach(thread_device);
    }

    pthread_t thread_deadlock;
    pthread_create(&thread_deadlock, NULL, algoritmo_deadlock, NULL);
	pthread_detach(thread_deadlock);
}

void algoritmo_deadlock(){
    log_info(logger_kernel, "Inicio de deadlock");
    while(1){
        log_info(logger_kernel, "Evaluando deadlock");
        sleep(config_kernel->TIEMPO_DEADLOCK);
        aplicar_deadlock();
    }
}

void planificador_largo_plazo(){
    log_info(logger_kernel, "Inicio de planificador de largo plazo");
    while(1){
        sem_wait(&cantidad_procesos_nuevos_suspendidos);
        sem_wait(&limite_grado_multiprogramacion);
        log_info(logger_kernel, "Ingreso de carpincho nuevo");

        pthread_mutex_lock(&MX_PCB);
        PCB_Carpincho* pcb = proximo_carpincho_to_ready();
        cambiar_estado(pcb, READY);
        sem_post(&cant_procesos_listos);
        pthread_mutex_unlock(&MX_PCB);
    }
}

void planificador_corto_plazo(){
    log_info(logger_kernel, "Inicio de planificador de corto plazo");
    while(1){
        sem_wait(&cant_procesos_listos);
        sem_wait(&limite_grado_multiprocesamiento);
        log_info(logger_kernel, "Ingreso de carpincho listo");

        PCB_Carpincho* pcb = proximo_carpincho_listo();

        log_info(logger_kernel, "Ingreso de carpincho en ejecución");
        cambiar_estado(pcb, EXEC);

        //t_package ser_entero(int value, op_code cod_operacion)//TODO

        //PCB_Carpincho *item = list_get(list_EXEC, 0);
        //Se restringe en el proceso de READY a EXEC

    }
}

void planificador_mediano_plazo(){
    log_info(logger_kernel, "Inicio de planificador de mediano plazo");

    int conexion_memoria = crear_conexion(config_kernel->IP_MEMORIA, string_itoa(config_kernel->PUERTO));
    while(1){
        sem_wait(&cantidad_procesos_suspendidos);
        PCB_Carpincho* pcb = get_last_blocked();
        cambiar_estado(pcb, SUSPENDED_BLOCKED);
        pasar_a_swap(pcb, conexion_memoria);
        sem_post(&limite_grado_multiprogramacion);
    }
}

void planificador_dispositivo(t_device* device){
    //creo el sem del device
    sem_init(&device->pcb_pendientes,0,0);
    log_info(logger_kernel, "Inicio de dispositivo: %s - %d", device->name, device->duration);
    while(1){	
        sem_wait(&device->pcb_pendientes);
        pthread_mutex_lock(&device->pcbs_mutex);
        PCB_Carpincho* pcb = get_first_pcb(device->pcbs);
        pthread_mutex_unlock(&device->pcbs_mutex);
        sleep(device->duration);
        if(pcb->estado == BLOCKED) cambiar_estado(pcb, READY); 	
        else {	
            cambiar_estado(pcb, SUSPENDED_READY); 	
            signal(cantidad_procesos_nuevos_suspendidos);	
        }
    }	
}

PCB_Carpincho* proximo_carpincho_listo(){
    if(config_kernel->ALGORITMO_PLANIFICACION == HRRN)
        return algoritmo_hrrn();
    return algoritmo_sjf();
}