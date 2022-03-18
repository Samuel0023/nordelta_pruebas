#include "deadlock.h"
bool recursos_disponibles_distintos_de_0();
bool pcb_bloqueado_por_algun_proceso(PCB_Carpincho *pcb_a_evaluar);
bool esta_bloqueado_pcb_por_motivo(t_list *lista,int motivo);

bool recursos_disponibles_distintos_de_0(){
    int cant =0;
    for(int i=0 ;i<list_size(semaphores_kernel);i++){
        if(recursos_disponibles[i]==0){
            cant++;
        }
    }
    return cant!=list_size(semaphores_kernel);
}
void aplicar_deadlock(){
    t_list* procesos_en_deadlock =  getProcesosEnDeadlock();
    int id;
    PCB_Carpincho* pcb_a_liberar;
    if(list_size(procesos_en_deadlock)>=2){
        id = list_get(procesos_en_deadlock, 0);
        pcb_a_liberar = find_pcb_in_kernel(id);  
        printf("\n cantidad de procesos en deadlock %d",list_size(procesos_en_deadlock));
        for(int c=1; c<list_size(procesos_en_deadlock); c++){
            int id = list_get(procesos_en_deadlock, c);
            printf("id: %id\n", id);
            PCB_Carpincho* item = find_pcb_in_kernel(id);
            if(item == NULL) continue;
            printf("Item: %d\n", item->id);
            if(item->id > pcb_a_liberar->id){
                pcb_a_liberar = item;
            }
        }

        //Las solicitudes al proceso lo descarto
        /*for(int c=0; c<list_size(peticiones); c++){
            t_celda *item = list_get(peticiones, c);
            
            if(item->proceso == pcb_a_liberar->id){
                list_remove(peticiones, c);
                free(item);
            }
        }*/

        printf("Wii\n");
        // Sem post por cada semaforo instanciado
        /*for(int c=0; c<list_size(asignaciones); c++){
            t_celda *item = list_get(asignaciones, c);
            
            if(item->proceso == pcb_a_liberar->id){
                t_sem_global* sem_selected;
                for(int c=0; c<list_size(semaphores_kernel);c++){
                    t_sem_global *semaphore = list_get(semaphores_kernel, c);
                    if(semaphore->id == item->semaforo){
                        sem_selected = semaphore;
                        break;
                    }
                } 
                char* name = sem_selected->name_sem;
                
                t_semaphore *sem_ref = malloc(sizeof(t_semaphore));
                sem_ref->name = name;
                sem_ref->value = item->value;
                sem_ref->processID = item->proceso;
                mate_sem_post_kernel(sem_ref);
            }
        }*/

        if(pcb_a_liberar != NULL){
            printf("Pre Liberacion\n");
            printf("Liberacion: %d\n", pcb_a_liberar->id);
            //Pasar a finalizado -> implica liberar memoria
            cambiar_estado(pcb_a_liberar, EXIT);   
        }
    }
}

bool existe_celda(int x, int y){
    bool _es_la_misma_celda(t_celda *celda){
        return (celda->proceso == x) && (celda->semaforo==y);
    }
    return list_any_satisfy(asignaciones,(void*)_es_la_misma_celda) && list_any_satisfy(peticiones,(void*)_es_la_misma_celda) ;
}
bool es_un_proceso_bloqueado(int id_proceso){
    bool _existe_pcb(PCB_Carpincho *pcb){
        return pcb->id == id_proceso;
    }
    return list_any_satisfy(list_BLOCKED,(void*)_existe_pcb);
}
bool vector_de_asignaciones_vacio(int id_proceso){
    int cant_sem_globales = list_size(semaphores_kernel);
    int *vector_asignaciones =  get_vector_de_asignaciones_segun(id_proceso);
    int valor_en_cero=0;
    for(int i=0 ;i<cant_sem_globales;i++){
        if(vector_asignaciones[i]== 0){
            valor_en_cero++;
        }
    }
    free(vector_asignaciones);
    return valor_en_cero != cant_sem_globales;
}
void makeCelda(int x,int y,int value){
  
    if(!existe_celda(x,y)){
        t_celda *celda_nueva = malloc(sizeof(t_celda));
        celda_nueva->proceso = x;
        celda_nueva->semaforo = y;
        celda_nueva->value = value;

        t_celda *celda_nueva2 = malloc(sizeof(t_celda));
        celda_nueva2->proceso = x;
        celda_nueva2->semaforo = y;
        celda_nueva2->value = value;

        list_add(asignaciones,celda_nueva);
        list_add(peticiones,celda_nueva2);
    }
   
}
t_celda *getCelda(int x,int y,t_matriz_op cod_op){
    bool _es_la_misma_coord(t_celda *celda){
        return (celda->proceso == x) && (celda->semaforo==y);        
    }
    switch(cod_op){
        case ASIGNACIONES:
            return list_find(asignaciones,(void*)_es_la_misma_coord);
            break;
        case PETICIONES:
            return list_find(peticiones,(void*)_es_la_misma_coord);            
        break;
        default: break;
    }
}

void increase(t_matriz_op cod_op,int pos_x,int pos_y){
    switch(cod_op){
        case ASIGNACIONES:
            increase_asignaciones(pos_x,pos_y);
            break;
        case PETICIONES:
            increase_peticiones(pos_x,pos_y);
            break;
        default: break;
    }
}

void decrease(t_matriz_op cod_op, int pos_x, int pos_y){
    switch(cod_op){
        case ASIGNACIONES:
            decrease_asignaciones(pos_x,pos_y);
            break;
        case PETICIONES:
            decrease_peticiones(pos_x,pos_y);
        break;
        default: break;
    }
}
void increase_asignaciones(int pos_x, int pos_y){
    t_celda *celda = getCelda(pos_x,pos_y,ASIGNACIONES);
    celda->value++;
}

void decrease_asignaciones(int pos_x, int pos_y){
    t_celda *celda = getCelda(pos_x,pos_y,ASIGNACIONES);
    celda->value--;
}

void increase_peticiones(int pos_x, int pos_y){
    t_celda *celda = getCelda(pos_x,pos_y,PETICIONES);
    celda->value++;
}

void decrease_peticiones(int pos_x, int pos_y){
    t_celda *celda = getCelda(pos_x,pos_y,PETICIONES);
    celda->value--;
}
int sum_recursos_semaphore(int id_sem){
    bool _sem_be_equal(t_celda *celda){
        return celda->semaforo == id_sem;
    }
    int sum=0;
    t_list* filtered_by_sem = list_filter(asignaciones,(void*)_sem_be_equal);
    int aux;
    for(int i=0 ; i<list_size(filtered_by_sem);i++){
        t_celda *celda= list_get(filtered_by_sem,i);
        sum+=celda->value;
        //printf("\t recursos_por semaforo [%d] :%d \n",id_sem,sum);
        
    }
    list_destroy(filtered_by_sem);
    return sum;
}
void set_recursos_totales(){
    t_sem_global *sem;
    recursos_totales = (int*) malloc(sizeof(list_size(semaphores_kernel)*sizeof(int)));
    for(int i =0; i<list_size(semaphores_kernel);i++){
        sem=list_get(semaphores_kernel,i);
        recursos_totales[i]= sem->valor_inicial;
    }
    printf("\n recursos_totales [%d][%d][%d]\n",recursos_totales[0],recursos_totales[1],recursos_totales[2]);
}
void set_recursos_disp(){
    recursos_disponibles = (int*) malloc(sizeof(list_size(semaphores_kernel)*sizeof(int)));

   for(int i=0 ; i<list_size(semaphores_kernel);i++){
       recursos_disponibles[i] = recursos_totales[i] - sum_recursos_semaphore(i);
       printf("\nsuma_recursos_asignados [%d]=%d\n",i,sum_recursos_semaphore(i));
   }
   printf("\n recursos_disponibles [%d][%d][%d]\n",recursos_disponibles[0],recursos_disponibles[1],recursos_disponibles[2]);
}
bool existe_id(int id_buscado,t_list* lista){
    bool _existe_id(int id){
        return id == id_buscado;
    }
    return list_any_satisfy(lista,(void*)_existe_id);
}

t_list *get_procesos_id(){
    int _get_process_id(t_celda *celda){
        return celda->proceso;
    }
    //mapear por procesoID
    t_list* processIDS = list_map(peticiones,(void*) _get_process_id);
    //printf("Size: %d\n", list_size(processIDS));
    if(list_size(processIDS) == 0){
        list_destroy(processIDS);
        return list_create();
    }

    //eliminar repetidos 
    void* _maximo_proceso_id(int id1, int id2){
        return id1>= id2? id1 : id2;
    }
    int max = list_get_maximum(processIDS,(void*)_maximo_proceso_id);
    t_list* aux = list_create();
    for(int i=0 ;i<=max;i++){
        if(existe_id(i,processIDS)){
            list_add(aux,i);
        }
    }
    //filtrar por la lista de pcb bloqueados
    bool _esta_bloqueado(int id_proceso){
        return es_un_proceso_bloqueado(id_proceso);
    }
    t_list* processIDS_BLOCKED = list_filter(aux,(void*)_esta_bloqueado);
    list_destroy(aux);
    list_destroy(processIDS);
    return processIDS_BLOCKED;
}

bool satisface(t_list* vector_proceso){
    int cant = list_size(semaphores_kernel);
    int aux=0;
    for(int i=0 ; i<cant;i++){

        if(recursos_disponibles[i]>= list_get(vector_proceso, i)){
            aux++;
        }
    }
  return aux==cant;
}

t_list* get_vector_de_peticiones_segun(int procesoID){
    t_list* vector_proceso = list_create(); 
    t_celda* celda;
    for(int i = 0 ; i<list_size(semaphores_kernel);i++){
        int value;
        if((celda = getCelda(procesoID,i,PETICIONES))==NULL){
            value = 0;
        }else{
            value = celda->value;
        }
        list_add(vector_proceso, value);
    }
    return vector_proceso;
}
int *get_vector_de_asignaciones_segun(int procesoID){

    int *vector_proceso = (int*) malloc(list_size(semaphores_kernel) * sizeof(int));
    t_celda* celda;
    for(int i = 0 ; i<list_size(semaphores_kernel);i++){
        if((celda = getCelda(procesoID,i,ASIGNACIONES))==NULL){
            vector_proceso[i]= 0;
        }else{
            vector_proceso[i]=celda->value;
        }
    }
    return vector_proceso;
}
void remove_id(int id,t_list* list){
    bool _is_the_same(int process_id){
        return id==process_id;
    }
    list_remove_by_condition(list,(void*)_is_the_same);
}
void simulacionAsignacion(int proceso_id){
    //simula la asignacion y la devolucion de recursos al vector de recursos disponibles
    int *vector_asignaciones = get_vector_de_asignaciones_segun(proceso_id);
    for(int i=0;i<sizeof(vector_asignaciones)-1;i++){
        recursos_disponibles[i]= recursos_disponibles[i] + vector_asignaciones[i];
    }
    free(vector_asignaciones);
}
t_list *obtener_candidatos_a_ser_deadlock(t_list* lista_de_ids){
    bool _vector_asignaciones_en_cero(int id_proceso){
        return vector_de_asignaciones_vacio(id_proceso);
    }
    t_list* candidatos = list_filter(lista_de_ids,(void*)_vector_asignaciones_en_cero);
    return candidatos;
}
bool esta_bloqueado_pcb_por_motivo(t_list *lista,int motivo){
    bool _is_the_same(int retenidos){
        return retenidos == motivo;
    }
    list_any_satisfy(lista,(void*)_is_the_same);
}
bool pcb_bloqueado_por_algun_proceso(PCB_Carpincho *pcb_a_evaluar){
    bool _es_retenido(PCB_Carpincho *pcb){
        return esta_bloqueado_pcb_por_motivo(pcb->sem_retenidos,pcb_a_evaluar->motivo_retencion);
    }
    return list_any_satisfy(list_BLOCKED,(void*)_es_retenido);
}
t_list *getProcesosEnDeadlock(){
    t_list* procesos_por_analizar = list_duplicate(list_BLOCKED);

    int cant_bloqueados = 0;
    PCB_Carpincho *pcb;
    for(int i=0;i<list_size(list_BLOCKED);i++){
        
        if(pcb_bloqueado_por_algun_proceso(pcb=list_get((list_BLOCKED),i))){
            printf("Deadlock encontrado\n");
        }else{
            list_remove(procesos_por_analizar,i);
        }
    }
    return procesos_por_analizar;
}   

// void liberar_procesos(){
//     t_list* procesos_en_deadlock = getProcesosEnDeadlock();

//}
void agregar_motivo_retencion(PCB_Carpincho *pcb,int semaphore_id){
    pcb->motivo_retencion=semaphore_id;
}

void agregar_retenidos(int process_id,int semaphore_id){
    PCB_Carpincho *pcb;
    pcb= find_pcb_in_kernel(process_id);
    if(pcb==NULL){
        printf("\n Que estas haciendo flaco, el PCB no existe\n");
    }else{
        printf("Paso por aqui: %d\n", pcb->id);
        list_add(pcb->sem_retenidos,semaphore_id);
    }
}
void liberar_motivo_retencion(PCB_Carpincho *carpincho ,int id_semaphore){
    list_remove(carpincho->sem_retenidos,id_semaphore);
}