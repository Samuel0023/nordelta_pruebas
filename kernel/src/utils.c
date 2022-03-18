#include "utils.h"

PCB_Carpincho *getByID(t_list *lista, int proceso_id)
{
	PCB_Carpincho *item = list_get(lista, 0);
	status_carpincho estado_planificacion = item->estado;
	bool mismo_id(PCB_Carpincho * item_auxiliar)
	{
		return item_auxiliar->id == proceso_id;
	}
	activar_mutex_estado(estado_planificacion);
	PCB_Carpincho *pcb = list_find(lista, (void*) mismo_id);
	desactivar_mutex_estado(estado_planificacion);
	return pcb;
}
PCB_Carpincho *find_pcb_in_kernel(int proceso_id){
    //ready,exec,suspended_ready,suspended_blocked,blocked
    bool _existe_process_id(PCB_Carpincho *pcb){
        return pcb->id == proceso_id;
    }
    PCB_Carpincho *pcb = NULL;
    int cant_listas_a_analizar = 5;
    int lista=0;
    t_list *lista_a_analizar;
    while(lista<cant_listas_a_analizar){
        switch(lista){
            case 0:
               lista_a_analizar=  get_list_by_state(READY);
                break;
            case 1:
                lista_a_analizar =  get_list_by_state(EXEC);
                break;
            case 2:
                lista_a_analizar =  get_list_by_state(SUSPENDED_READY);
                break;
            case 3:
                lista_a_analizar =  get_list_by_state(SUSPENDED_BLOCKED);
                break;
            case 4:
                lista_a_analizar =  get_list_by_state(BLOCKED);
                break;
            default:
                break;
        }
        pcb = list_find(lista_a_analizar,(void*)_existe_process_id);
        printf("Iteracion");
        if(pcb == NULL){
            lista++;
        }else{
            printf("Encontro: %d\n", lista);
            lista=5;
        }
    }

	if(pcb != NULL)
    	printf("ID: %d\n", pcb->id);
    return pcb;
}
PCB_Carpincho *list_remove_proceso(t_list *lista, int proceso_id, status_carpincho estado)
{
	bool mismo_id(PCB_Carpincho * item_auxiliar)
	{
		return item_auxiliar->id == proceso_id;
	}
	activar_mutex_estado(estado);
	int index = 0;
	PCB_Carpincho *proceso;
	for(int c=0; c<list_size(lista); c++){
		PCB_Carpincho *auxiliar = list_get(lista, c);
		if(auxiliar->id == proceso_id){
			proceso = auxiliar;
			index = c;
			break;
		}
	}	
	list_remove(lista, index);
	//PCB_Carpincho *proceso = list_remove_by_condition(lista, mismo_id);
	desactivar_mutex_estado(estado);
	return proceso;
}

t_list *get_list_by_state(status_carpincho estado)
{
	t_list *list_state;
	switch (estado)
	{
	case NEW:
		list_state = list_NEW;
		break;
	case READY:
		list_state = list_READY;
		break;
	case EXEC:
		list_state = list_EXEC;
		break;
	case SUSPENDED_READY:
		list_state = list_SUSPENDED_READY;
		break;
	case SUSPENDED_BLOCKED:
		list_state = list_SUSPENDED_BLOCKED;
		break;
	case BLOCKED:
		list_state = list_BLOCKED;
		break;
	case EXIT:
		list_state = lista_EXIT;
		break;
	default:
		break;
	}
	return list_state;
}

bool se_puede_agregar_pcb_suspendido(){
	return list_size(list_BLOCKED) > 0 && list_size(list_READY) > 0 && list_size(list_NEW) > 0;
}

void cambiar_estado(PCB_Carpincho *proceso, status_carpincho estado_destino)
{
	status_carpincho estado_origen = proceso->estado;
	t_list *list_state_origin = get_list_by_state(estado_origen);

	//printf("PID inicio: %d %d %d\n", proceso->id, list_size(list_state_origin), proceso->estado);
	PCB_Carpincho *item = list_remove_proceso(list_state_origin, proceso->id, proceso->estado);
	item->estado = estado_destino;

	activar_mutex_estado(estado_destino);

	t_list *list_state_destine = get_list_by_state(estado_destino);
	list_add(list_state_destine, item);
	
	desactivar_mutex_estado(estado_destino);

	if(estado_destino == READY) proceso->inicio_ejecucion_time = get_time_now_MS();
	if(se_puede_agregar_pcb_suspendido()) sem_post(&cantidad_procesos_nuevos_suspendidos);
}

void activar_mutex_estado(status_carpincho estado)
{
	switch (estado)
	{
	case NEW:
		pthread_mutex_lock(&MX_NEW);
		break;
	case READY:
		pthread_mutex_lock(&MX_READY);
		break;
	case EXEC:
		pthread_mutex_lock(&MX_EXEC);
		break;
	case SUSPENDED_READY:
		pthread_mutex_lock(&MX_SUSPENDED_READY);
		break;
	case SUSPENDED_BLOCKED:
		pthread_mutex_lock(&MX_SUSPENDED_BLOCKED);
		break;
	case BLOCKED:
		pthread_mutex_lock(&MX_BLOCKED);
		break;
	case EXIT:
		pthread_mutex_lock(&MX_EXIT);
		break;
	default:
		break;
	}
	return;
}
void desactivar_mutex_estado(status_carpincho estado)
{
	switch (estado)
	{
	case NEW:
		pthread_mutex_unlock(&MX_NEW);
		break;
	case READY:
		pthread_mutex_unlock(&MX_READY);
		break;
	case EXEC:
		pthread_mutex_unlock(&MX_EXEC);
		break;
	case SUSPENDED_READY:
		pthread_mutex_unlock(&MX_SUSPENDED_READY);
		break;
	case SUSPENDED_BLOCKED:
		pthread_mutex_unlock(&MX_SUSPENDED_BLOCKED);
		break;
	case BLOCKED:
		pthread_mutex_unlock(&MX_BLOCKED);
		break;
	case EXIT:
		pthread_mutex_unlock(&MX_EXIT);
		break;
	default:
		break;
	}
	return;
}

void recibir_mensaje_tp0(int socket_cliente)
{
	int size;
	char *buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger_kernel, "Me llego el mensaje %s", buffer);
	free(buffer);
}

//podemos usar la lista de valores para poder hablar del for y de como recorrer la lista
t_list *recibir_paquete_tp0(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void *buffer;
	t_list *valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while (desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);
		char *valor = malloc(tamanio);
		memcpy(valor, buffer + desplazamiento, tamanio);
		desplazamiento += tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
	return NULL;
}

long convert_time_ms(char * time_now){
	char ** list_time = string_split(time_now, ":");
	long hour_ms = 36000000 * atoi(list_time[0]);
	long min_ms = 60000 * atoi(list_time[1]);
	long seg_ms = 1000 * atoi(list_time[2]);

	long result = (hour_ms + min_ms + seg_ms + atoi(list_time[3])); 
	free(list_time[0]);
	free(list_time[1]);
	free(list_time[2]);
	free(list_time[3]);
	free(list_time);
	return result;
}

long get_time_now_MS(){
	char * now = temporal_get_string_time("%H:%M:%S:%MS");
	long miliseg = convert_time_ms(now);
	free(now);
	return miliseg;
}

void pasar_a_swap(PCB_Carpincho* pcb, int socket_memoria){
	t_package package = ser_entero(pcb->id, SWAP_OUT);
	sendMessage(package, socket_memoria);
}