#include "../include/tests.h"

void validar_sd_memalloc();
void validar_sd_memfree();
void validar_sd_memwrite();
void validar_convert_time_a_ms();
void validar_algoritmo();
void semaforo_struct();
void validar_mate_call_io();
void validar_kernel_mate_call_io();
void validar_respuesta_de_memoria();

int run_tests()
{
    CU_initialize_registry();
    CU_pSuite tests = CU_add_suite("Kernel Suite", NULL, NULL);

    CU_add_test(tests, "Verificar ser memalloc\n", validar_sd_memalloc);
    CU_add_test(tests, "Verificar ser memfree\n", validar_sd_memfree);
    CU_add_test(tests, "Verificar ser memwrite\n", validar_sd_memwrite);
    CU_add_test(tests, "Verificar método estado genérico: ", cambio_de_estado_carpincho);
    CU_add_test(tests,"verificar semaphores en kernel \n", validar_semaphores_kernel);
    CU_add_test(tests,"verificar deadlock \n", validar_deadlock);    
    CU_add_test(tests, "Verif. prox. carpincho en new: ", testeo_proximo_carpincho_new);
    CU_add_test(tests, "Verificar conversion a ms: ", validar_convert_time_a_ms);
    CU_add_test(tests, "Validar algoritmo: ", validar_algoritmo);
    CU_add_test(tests, "Validar struct: ", semaforo_struct);
    CU_add_test(tests, "Verificar serializacion de mate_call_io", validar_mate_call_io);
    CU_add_test(tests, "Verificar funcionalidad de mate_call_io en kernel", validar_kernel_mate_call_io);
    CU_add_test(tests, "Validar planificacion mediano plazo: ", validar_mediano_plazo);
    CU_add_test(tests, "Validar ser y des respuesta id mate_init: ", validar_sd_respuesta_id_mate_init);
    CU_add_test(tests, "Validar inicializacion pcb carpincho nuevo, mate_init: ", validar_inicializar_carpincho_mate_init);
    CU_add_test(tests, "Validar ser y des derespuesta de memoria: ", validar_respuesta_de_memoria);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void cambio_de_estado_carpincho()
{
    list_READY = list_create();
    list_NEW = list_create();
    list_BLOCKED = list_create();
    pthread_mutex_init(&MX_NEW, NULL);
	pthread_mutex_init(&MX_READY, NULL);
    PCB_Carpincho *item1 = malloc(sizeof(PCB_Carpincho));
    item1->id = 10;
    item1->estado = NEW;
    PCB_Carpincho *item2 = malloc(sizeof(PCB_Carpincho));
    item2->id = 15;
    item2->estado = NEW;

    list_add(list_NEW, item1);
    list_add(list_NEW, item2);
    cambiar_estado(item1, READY);

    CU_ASSERT_EQUAL(list_size(list_NEW), 1);
    CU_ASSERT_EQUAL(list_size(list_READY), 1);

    free(item1);
    free(item2);
    list_destroy(list_READY);
    list_destroy(list_NEW);
    list_destroy(list_BLOCKED);
}

void semaforo_struct()
{
    t_semaphore item;
    item.name = "semaphore";
    item.long_name = strlen(item.name);
    item.value = 3;
    item.processID = 7;

    t_package package = ser_semaphore(item, SEM_INIT);
    t_semaphore item2 = des_semaphore(package);

    CU_ASSERT_STRING_EQUAL(item.name, item2.name);
    CU_ASSERT_EQUAL(item.value, item2.value);
    CU_ASSERT_EQUAL(item.processID, item2.processID);

    free(package.buffer);
    free(item2.name);
}

void validar_sd_memalloc(){
    t_msj_memalloc data;
    t_msj_memalloc res;
    t_package paquete;

    data.tam_alloc = 4;
	data.pid_carpincho = 1;

    paquete = ser_msj_code_memalloc(data);
    res = des_msj_code_memalloc(paquete);

    CU_ASSERT_EQUAL(res.pid_carpincho, data.pid_carpincho);
    CU_ASSERT_EQUAL(res.tam_alloc, data.tam_alloc);

    free(paquete.buffer);
}

void validar_sd_memfree(){
    t_msj_memfree data_free;
    t_msj_memfree res;
    t_package paquete;

    data_free.pid_carpincho = 1;
	data_free.dir_logica = 9;

    paquete = ser_msj_code_memfree(data_free);
    res = des_msj_code_memfree(paquete);

    CU_ASSERT_EQUAL(res.pid_carpincho, data_free.pid_carpincho);
    CU_ASSERT_EQUAL(res.dir_logica, data_free.dir_logica);

    free(paquete.buffer);
}

void validar_sd_memwrite(){
    t_msj_memwrite data_write;
    t_msj_memwrite res;
    t_package paquete;

    data_write.pid_carpincho = 1;
    data_write.dir_logica = 9;
    data_write.tam_contenido = 4;
    data_write.contenido = "hola";

    paquete = ser_msj_code_memwrite(data_write);
    res = des_msj_code_memwrite(paquete);
    printf("mesj %s", res.contenido);

    char * res_cont = res.contenido;
    res_cont[data_write.tam_contenido]= '\0';

    CU_ASSERT_EQUAL(res.pid_carpincho, data_write.pid_carpincho);
    CU_ASSERT_EQUAL(res.dir_logica, data_write.dir_logica);
    CU_ASSERT_EQUAL(res.tam_contenido, data_write.tam_contenido);
    CU_ASSERT_STRING_EQUAL(res_cont, "hola");

    free(res.contenido);
    free(paquete.buffer);
}

void testeo_proximo_carpincho_new()
{
    pthread_mutex_init(&MX_NEW, NULL);
    list_NEW = list_create();
    PCB_Carpincho *item1 = malloc(sizeof(PCB_Carpincho));
    item1->id = 10;
    item1->estado = NEW;
    PCB_Carpincho *item2 = malloc(sizeof(PCB_Carpincho));
    item2->id = 15;
    item2->estado = NEW;
    list_add(list_NEW, item1);
    list_add(list_NEW, item2);

    PCB_Carpincho *selectedItem = proximo_carpincho_new(); 
    CU_ASSERT_EQUAL(selectedItem->id, item1->id);
    list_destroy(list_NEW);
    free(item1);
    free(item2);
}

void mate_sem_init__kernel(){
    semaphores_kernel = list_create();
    t_semaphore item;
    item.name = "semaphore";
    item.long_name = strlen(item.name);
    item.value = 3;
    item.processID = 7;

    mate_sem_init_kernel(&item);
    printf("se agregan %d t_semaphore \n", list_size(semaphores_kernel));
    mate_sem_init_kernel(&item);
     printf("se agregan %d t_semaphore \n", list_size(semaphores_kernel));
    mate_sem_destroy_kernel(&item);
   
    CU_ASSERT_EQUAL(list_size(semaphores_kernel),0);
    list_destroy(semaphores_kernel);
}

void validar_semaphores_kernel(){
    list_READY = list_create();
    list_BLOCKED = list_create();
 	list_EXEC = list_create();
    PCB_Carpincho * c1 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c2 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c3 = malloc(sizeof(PCB_Carpincho));
    c1->id = 1;
    c2->id = 2;
    c3->id = 3;
    
    c1->estado = EXEC;
    c2->estado = EXEC;
    c3->estado = EXEC;
    
    list_add(list_EXEC, c1);
    list_add(list_EXEC, c2);
    list_add(list_EXEC, c3);

    t_semaphore *item= malloc(sizeof(t_semaphore));
    item->name = "semaphore";
    item->long_name = strlen(item->name);
    item->value = 3;
    item->processID = 1;

    t_semaphore *item2= malloc(sizeof(t_semaphore));
    item2->name = "semaphore2";
    item2->long_name = strlen(item2->name);
    item2->value = 3;
    item2->processID = 2;
    

    semaphores_kernel = list_create();
    asignaciones=list_create();         //lista de dupla [procesos][semaforos]
    peticiones = list_create();
    
    mate_sem_init_kernel(item);
    printf("\nse agregan %d semaphore \n", list_size(semaphores_kernel));
    printf("\n celda1 : %d \n", getCelda(1,0,PETICIONES)->value);
    mate_sem_init_kernel(item);
    printf("se agregan %d semaphore \n", list_size(semaphores_kernel));

    mate_sem_init_kernel(item2);
    printf("se agregan %d semaphore \n", list_size(semaphores_kernel));
    printf("\n celda2 : %d \n", getCelda(2,1,PETICIONES)->value);
    // mate_sem_wait_kernel(&item);
    // printf("se realiza un sem_wait %d t_semaphore->value \n",item.value);    

    printf("semaphoro agregado %s \n",get_semaphore(item->name)->name_sem);

    printf("semaphoro agregado %s \n",get_semaphore(item2->name)->name_sem);
    
    mate_sem_wait_kernel(item,1);
    printf("se realiza un sem_wait y el value de sem1 es : %d \n",get_semaphore(item->name)->value);  
    mate_sem_wait_kernel(item,1);
    printf("se realiza un sem_wait y el value de sem1 es : %d \n",get_semaphore(item->name)->value);  
    mate_sem_wait_kernel(item,1);
    printf("se realiza un sem_wait y el value de sem1 es : %d \n",get_semaphore(item->name)->value);  

    printf("cant de carpinchos en BLOCKED: %d \n",list_size(list_BLOCKED));
    PCB_Carpincho* blocked_pcb = list_get(list_BLOCKED,0);
    printf("carpincho bloqueado id= %d  \n", blocked_pcb->id);
    
    mate_sem_post_kernel(item);
    printf("se realiza un sem_post y el value de sem1 es : %d \n",get_semaphore(item->name)->value);  
    
    printf("cant de carpinchos en READY: %d ",list_size(list_READY));
     
    mate_sem_destroy_kernel(item);
    mate_sem_destroy_kernel(item2);
    
    
    CU_ASSERT_EQUAL(list_size(semaphores_kernel),0);

    list_destroy(list_READY);
    list_destroy(list_EXEC);
    list_destroy(list_BLOCKED);
    list_destroy(semaphores_kernel);
    for(int c=0; c < list_size(asignaciones); c++){
        void* value = list_get(asignaciones, c);
        free(value);
    }
    list_destroy(asignaciones); 
    for(int c=0; c < list_size(peticiones); c++){
        void* value = list_get(peticiones, c);
        free(value);
    }
    list_destroy(peticiones);
    free(item);
    free(item2);
    free(c1);
    free(c2);
    free(c3);
}
void validar_deadlock(){

    semaphores_kernel = list_create();
    asignaciones=list_create();
    peticiones = list_create();

    list_BLOCKED = list_create();
    PCB_Carpincho * c1 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c2 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c3 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c4 = malloc(sizeof(PCB_Carpincho));
    c1->id = 1;
    c2->id = 2;
    c3->id = 3;
    c4->id = 4;
    
    c1->estado = BLOCKED;
    c2->estado = BLOCKED;
    c3->estado = BLOCKED;
    c4->estado = BLOCKED;
    
    list_add(list_BLOCKED, c1);
    list_add(list_BLOCKED, c2);
    list_add(list_BLOCKED, c3);
    list_add(list_BLOCKED, c4);
    
    t_sem_global *sem0= malloc(sizeof(t_semaphore));
    sem0->id =0;
    sem0->name_sem = "semaphore0";
    sem0->value = 9;
    
    t_sem_global *sem1= malloc(sizeof(t_semaphore));
    sem1->id =1;
    sem1->name_sem = "semaphore1";
    sem1->value = 5;
    
    t_sem_global *sem2= malloc(sizeof(t_semaphore));
    sem2->id =2;
    sem2->name_sem = "semaphore2";
    sem2->value = 6;

    list_add(semaphores_kernel,sem0);
    list_add(semaphores_kernel,sem1);
    list_add(semaphores_kernel,sem2);
    
    //asignaciones
    t_celda *celda1 = malloc(sizeof(t_celda));
    celda1->proceso = 1;
    celda1->semaforo =0;
    celda1->value = 1;
    t_celda *celda2 = malloc(sizeof(t_celda));
    celda2->proceso = 1;
    celda2->semaforo =2;
    celda2->value = 1;
    t_celda *celda3 = malloc(sizeof(t_celda));
    celda3->proceso = 2;
    celda3->semaforo =1;
    celda3->value = 3;
    t_celda *celda4 = malloc(sizeof(t_celda)); 
    celda4->proceso = 3;
    celda4->semaforo =0;
    celda4->value = 4;       
    t_celda *celda5 = malloc(sizeof(t_celda));
    celda5->proceso = 3;
    celda5->semaforo =1;
    celda5->value = 1;
    t_celda *celda14 = malloc(sizeof(t_celda));
    celda14->proceso = 3;
    celda14->semaforo =2;
    celda14->value = 3;

    t_celda *celda15 = malloc(sizeof(t_celda));
    celda15->proceso = 4;
    celda15->semaforo =0;
    celda15->value = 0;
    t_celda *celda16 = malloc(sizeof(t_celda));
    celda16->proceso = 4;
    celda16->semaforo =1;
    celda16->value = 0;
    t_celda *celda17 = malloc(sizeof(t_celda));
    celda17->proceso = 4;
    celda17->semaforo =2;
    celda17->value = 0;

    list_add(asignaciones,celda1);
    list_add(asignaciones,celda2);
    list_add(asignaciones,celda3);
    list_add(asignaciones,celda4);
    list_add(asignaciones,celda5);
    list_add(asignaciones,celda14);
    list_add(asignaciones,celda15);
    list_add(asignaciones,celda16);
    list_add(asignaciones,celda17);
    
    
    //peticiones
    t_celda *celda6 = malloc(sizeof(t_celda));
    celda6->proceso = 1;
    celda6->semaforo =0;
    celda6->value = 2;
    t_celda *celda7 = malloc(sizeof(t_celda));
    celda7->proceso = 1;
    celda7->semaforo =2;
    celda7->value = 1;
    t_celda *celda8 = malloc(sizeof(t_celda));
    celda8->proceso = 2;
    celda8->semaforo =1;
    celda8->value = 3;
    t_celda *celda9 = malloc(sizeof(t_celda));
    celda9->proceso = 2;
    celda9->semaforo =2;
    celda9->value = 1;    
    t_celda *celda10 = malloc(sizeof(t_celda));
    celda10->proceso = 2;
    celda10->semaforo =0;
    celda10->value = 1;
    t_celda *celda11 = malloc(sizeof(t_celda));
    celda11->proceso = 3;
    celda11->semaforo =0;
    celda11->value = 6;
    t_celda *celda12 = malloc(sizeof(t_celda));
    celda12->proceso = 3;
    celda12->semaforo =1;
    celda12->value = 1;
    t_celda *celda13 = malloc(sizeof(t_celda));
    celda13->proceso = 3;
    celda13->semaforo =2;
    celda13->value = 1;
    
    t_celda *celda18 = malloc(sizeof(t_celda));
    celda18->proceso = 4;
    celda18->semaforo =0;
    celda18->value = 1;
    t_celda *celda19 = malloc(sizeof(t_celda));
    celda19->proceso = 4;
    celda19->semaforo =1;
    celda19->value = 0;
    t_celda *celda20 = malloc(sizeof(t_celda));
    celda20->proceso = 4;
    celda20->semaforo =2;
    celda20->value = 1;

    list_add(peticiones,celda6);
    list_add(peticiones,celda7);
    list_add(peticiones,celda8);
    list_add(peticiones,celda9);
    list_add(peticiones,celda10);
    list_add(peticiones,celda11);
    list_add(peticiones,celda12);
    list_add(peticiones,celda13);
    list_add(peticiones,celda18);
    list_add(peticiones,celda19);
    list_add(peticiones,celda20);
 
    t_list *procesos_en_deadlock = getProcesosEnDeadlock();
    printf("\n procesos_en_deadlock %d\n",list_size(procesos_en_deadlock));
    printf("\n id :%d %d\n",list_get(procesos_en_deadlock,0),list_get(procesos_en_deadlock,1));
    CU_ASSERT_EQUAL(2,list_size(procesos_en_deadlock));
    for(int c=0; c < list_size(semaphores_kernel); c++){
        void* value = list_get(semaphores_kernel, c);
        free(value);
    }
    list_destroy(semaphores_kernel);
    for(int c=0; c < list_size(asignaciones); c++){
        void* value = list_get(asignaciones, c);
        free(value);
    }
    list_destroy(asignaciones); 
    for(int c=0; c < list_size(peticiones); c++){
        void* value = list_get(peticiones, c);
        free(value);
    }
    list_destroy(peticiones);
    for(int c=0; c < list_size(list_BLOCKED); c++){
        void* value = list_get(list_BLOCKED, c);
        free(value);
    }
    list_destroy(list_BLOCKED);
    list_destroy(procesos_en_deadlock);
}

void validar_convert_time_a_ms(){
    long dato = 45.99;

    //printf("dato: %ld \n", dato);
    char * hora = "10:22:15:221";
    long ms = convert_time_ms(hora);

    CU_ASSERT_EQUAL(ms, 361335221);
}

void validar_algoritmo(){
    list_READY = list_create();

    PCB_Carpincho * c1 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c2 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c3 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * ps;
    PCB_Carpincho * ps_hrrn;

    c1->estado = READY;
    c2->estado = READY;
    c3->estado = READY;

    c1->estimacion_anterior = 10.3;
    c2->estimacion_anterior = 9.56;
    c3->estimacion_anterior = 11.6;

    c1->id = 1;
    c2->id = 2;
    c3->id = 3;

    c1->inicio_ejecucion_time = 3;
    c2->inicio_ejecucion_time = 1;
    c3->inicio_ejecucion_time = 2;

    c1->inicio_listo_time = 361335222;
    c2->inicio_listo_time = 361335221;
    c3->inicio_listo_time = 361335223;

    // c1->rafaga_anterior = 1.56;
    // c2->rafaga_anterior = 1.41;
    // c3->rafaga_anterior = 1.86;
    c1->rafaga_anterior = 0;
    c2->rafaga_anterior = 0;
    c3->rafaga_anterior = 0;

    list_add(list_READY, c1);
    list_add(list_READY, c2);
    list_add(list_READY, c3);

    ps = algoritmo_sjf();
    ps_hrrn = algoritmo_hrrn();

    CU_ASSERT_EQUAL(ps->id, 2);
    CU_ASSERT_EQUAL(ps_hrrn->id, 2);

    /* liberar */
    list_destroy(list_READY);
    free(c1);
    free(c2);
    free(c3);
}

void validar_mensaje_mate_close(){
    t_package paquete;
    t_mate_close data;
    t_mate_close res;

    data.id_carpincho = 1;
    paquete = ser_msj_code_mete_close(data);
    res = des_msj_code_mete_close(paquete);

    CU_ASSERT_EQUAL(data.id_carpincho, res.id_carpincho);

    free(paquete.buffer);
}

void validar_mate_call_io(){
    t_mate_call_io data;
    t_mate_call_io res;
    t_package paquete;

    data.id_carpincho = 1;
    data.tam_nombre_io = strlen("teclado");
    data.nombre_io = "teclado";
    data.tam_mensaje = strlen("Usar teclado");
    data.mensaje = "Usar teclado";

    paquete = ser_msj_code_mete_call_io(data);
    res = des_msj_code_mete_call_io(paquete);

    CU_ASSERT_EQUAL(data.id_carpincho, res.id_carpincho);
    CU_ASSERT_STRING_EQUAL(data.nombre_io, res.nombre_io);
    CU_ASSERT_STRING_EQUAL(data.mensaje, res.mensaje);

    free(paquete.buffer);
    free(res.mensaje);
    free(res.nombre_io);
}
void validar_kernel_mate_call_io(){

    list_READY = list_create();
    list_EXEC = list_create();
    list_SUSPENDED_READY = list_create();
    list_SUSPENDED_BLOCKED = list_create();
    list_BLOCKED = list_create();

    
    devices =list_create();

    PCB_Carpincho * c1 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c2 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c3 = malloc(sizeof(PCB_Carpincho));
    PCB_Carpincho * c4 = malloc(sizeof(PCB_Carpincho));
    c1->id = 1;
    c2->id = 2;
    c3->id = 3;
    c4->id = 4;

    list_add(list_READY,c1);

    list_add(list_EXEC,c2);

    list_add(list_SUSPENDED_READY,c1);

    list_add(list_SUSPENDED_BLOCKED,c3);

    list_add(list_BLOCKED,c4);

    t_device* device = malloc(sizeof(t_device));
    device->name = "impresora";
    device->duration = 2;
    device->pcbs = list_create();
    sem_init(&device->pcb_pendientes,0,1);
    list_add(device->pcbs,c1);
    list_add(device->pcbs,c3);
    list_add(device->pcbs,c2);

    list_add(devices,device);
                                            
    //printf("\n cant %d\n",list_size(devices));
    mate_call_io("impresora",c4->id);

    CU_ASSERT_EQUAL(true,existe_pcb(c4->id,device));
    list_destroy(list_READY);
    list_destroy(list_EXEC);
    list_destroy(list_SUSPENDED_READY);
    list_destroy(list_SUSPENDED_BLOCKED);
    list_destroy(list_BLOCKED);
    list_destroy(device->pcbs);
    list_destroy(devices);
    free(c1);
    free(c2);
    free(c3);
    free(c4);
    free(device);
}
void validar_mediano_plazo(){
    list_NEW = list_create();
    PCB_Carpincho *item1 = malloc(sizeof(PCB_Carpincho));
    item1->id = 10;
    item1->estado = NEW;
    PCB_Carpincho *item2 = malloc(sizeof(PCB_Carpincho));
    item2->id = 15;
    item2->estado = NEW;
    list_add(list_NEW, item1);
    list_add(list_NEW, item2);

    list_SUSPENDED_READY = list_create();
    PCB_Carpincho *item3 = malloc(sizeof(PCB_Carpincho));
    item3->id = 9;
    item3->estado = SUSPENDED_READY;
    list_add(list_SUSPENDED_READY, item3);

    PCB_Carpincho *selectedItem = proximo_carpincho_to_ready(); 
    CU_ASSERT_EQUAL(selectedItem->id, item3->id);
    CU_ASSERT_EQUAL(list_size(list_NEW), 2);
    CU_ASSERT_EQUAL(list_size(list_SUSPENDED_READY), 0);

    free(item1);
    free(item2);
    free(item3);
    list_destroy(list_NEW);
    list_destroy(list_SUSPENDED_READY);

}

void validar_sd_respuesta_id_mate_init(){
    int id_carpincho;
    int id_deserealizado;
    t_package paquete;

    id_carpincho = 5;

    paquete = ser_respuesta_id(id_carpincho);
    id_deserealizado = des_respuesta_id(paquete);

    CU_ASSERT_EQUAL(id_carpincho, id_deserealizado);
    free(paquete.buffer);
}

void validar_inicializar_carpincho_mate_init(){
    list_NEW = list_create();
    pthread_mutex_init(&MX_ID_PROCESO, NULL);
    pthread_mutex_init(&MX_NEW, NULL);
    sem_init(&cantidad_procesos_nuevos_suspendidos, 0, 0);
    id_proceso = 1; 

    double estimacion_anterior_config = 50;
    int socket_carpincho = 10;
    int pid1 = inicializar_carpincho_nuevo(estimacion_anterior_config, socket_carpincho);
    int pid2 = inicializar_carpincho_nuevo(estimacion_anterior_config, socket_carpincho);

    CU_ASSERT_EQUAL(pid1, 1);
    CU_ASSERT_EQUAL(pid2, 2);
    CU_ASSERT_EQUAL(list_size(list_NEW), 2);
    PCB_Carpincho * carpincho_nuevo1 = list_get(list_NEW, 0);
    PCB_Carpincho * carpincho_nuevo2 = list_get(list_NEW, 1);
    CU_ASSERT_EQUAL(carpincho_nuevo1->id, 1);
    CU_ASSERT_EQUAL(carpincho_nuevo2->id, 2);

    list_destroy(list_NEW);
    free(carpincho_nuevo1);
    free(carpincho_nuevo2);
}

void validar_respuesta_de_memoria(){
    t_package paquete;
    int data = -5;
    int res;

    paquete = ser_res_memfree(data);
    res = des_respuesta_memoria(paquete);

    CU_ASSERT_EQUAL(data, res);

    free(paquete.buffer);
}