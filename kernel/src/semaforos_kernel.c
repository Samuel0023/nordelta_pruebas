#include "semaforos_kernel.h"
void _liberar_memoria_item(char*);

t_sem_global *get_semaphore(char* name){
    bool _es_el_mismo_nombre(t_sem_global *semaphore){
        return string_equals_ignore_case(semaphore->name_sem,name);
    }
    return list_find(semaphores_kernel,(void*)_es_el_mismo_nombre);
}
void mate_sem_init_kernel(t_semaphore *sem_ref){
    bool _existe_semaforo(char *name){
        bool _es_el_mismo_nombre(t_sem_global *semaphore){
            return string_equals_ignore_case(semaphore->name_sem,name);
        }
        return list_any_satisfy(semaphores_kernel,(void*)_es_el_mismo_nombre);
    }
    if(!_existe_semaforo(sem_ref->name)){
        t_sem_global *semaphore = malloc(sizeof(t_sem_global));
        semaphore->id=list_size(semaphores_kernel);
        semaphore->name_sem =sem_ref->name;
        semaphore->value = sem_ref->value;
        semaphore->valor_inicial = sem_ref->value;
        //sem_init(&semaphore->sem,0, 0);
        //sem_init(&semaphore->sem,0,semaphore->value);
        list_add(semaphores_kernel,semaphore);
       //log_info("Semaforo agregado : %s",semaphore->name);

        //celda para las matrices de asignacion y peticion
        printf("\n semaforo %d \n",semaphore->id);
        printf("\n proceso %d \n",sem_ref->processID);
        //makeCelda(sem_ref->processID,semaphore->id,semaphore->value);
    }

}
void mate_sem_wait_kernel(t_semaphore *sem_ref,int socket_cliente){
    printf("Test\n");
    t_sem_global *semaphore = get_semaphore(sem_ref->name);
   // makeCelda(sem_ref->processID,semaphore->id,0);
    //increase(PETICIONES,sem_ref->processID,semaphore->id);

    semaphore->value--;
    if(semaphore->value <0){
        printf("Negative\n");
        printf("ID: %d lista en ejecucion %d\n", sem_ref->processID, list_size(list_EXEC));
        printf("ID: %d lista DE BLOQUEADOS %d\n", sem_ref->processID, list_size(list_BLOCKED));
	    
        PCB_Carpincho* pcb = getByID(list_EXEC, sem_ref->processID);
        if(pcb == NULL){
            printf("No encontré\n");
        }
		cambiar_estado(pcb, BLOCKED);
        agregar_motivo_retencion(pcb,semaphore->id);
        //sem_wait(&semaphore->sem);//SE COMENTA
        pcb->socket_res = socket_cliente;
	}else{
        printf("Positivo\n");
        //para test comentar la linea de abajo
        sendMessage(make_res("mate_sem_wait successful"), socket_cliente);
        printf("Agregar retenidos\n");
        agregar_retenidos(sem_ref->processID,semaphore->id);
        //increase(ASIGNACIONES,sem_ref->processID,semaphore->id);
        //decrease(PETICIONES,sem_ref->processID,semaphore->id);

    }
}
void mate_sem_post_kernel(t_semaphore *sem_ref){

    t_sem_global *semaphore = get_semaphore(sem_ref->name);
    makeCelda(sem_ref->processID,semaphore->id,0);
    printf("ID: %d lista DE BLOQUEADOS%d\n", sem_ref->processID, list_size(list_BLOCKED));
    
    if(semaphore->value<1){
        PCB_Carpincho* pcb  = getByID(list_BLOCKED,sem_ref->processID);
        cambiar_estado(pcb,READY);
        //sem_post(&semaphore->sem);

        //para test comentar la linea de abajo
        sendMessage(make_res("carpincho unlocked"), pcb->socket_res);
        liberar_motivo_retencion(pcb,semaphore->id);
        //decrease(PETICIONES,pcb->id,semaphore->id);
        //increase(ASIGNACIONES,pcb->id,semaphore->id);
    }else{
        //decrease(ASIGNACIONES,sem_ref->processID,semaphore->id);

        semaphore->value++;
    }
}
void mate_sem_destroy_kernel(t_semaphore *sem_ref){
    char *name_aux = sem_ref->name;
    bool _es_el_mismo_semaphore(t_sem_global *semaphore){
      return string_equals_ignore_case(semaphore->name_sem,name_aux);
    }
    void semaphore_destroy(t_sem_global *semaphore){
        //sem_destroy(&semaphore->sem);
        free(semaphore);
    }
    list_remove_and_destroy_by_condition(semaphores_kernel,(void*)_es_el_mismo_semaphore, (void*)semaphore_destroy);
}

void _liberar_memoria_item(char* item){
    return free(item);
}
