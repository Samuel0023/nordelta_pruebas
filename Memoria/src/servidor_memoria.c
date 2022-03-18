#include "servidor_memoria.h"

t_list* messages;


void* ejecutar_operacion_memoria(int cliente_fd){

	t_package paquete;
	t_package paquete_res;
	t_message item;
	t_list_message lista;
	t_msj_memalloc mate_memalloc;
 	t_msj_memwrite mate_memwrite;
 
	int proceso_id;
  	int max_cantidad = 0;
	int direccionLogica;
	t_msj_memwrite msj;

	while (1) {
		paquete = recibir_mensaje(cliente_fd);
		//log_info(logger,"Codigo de operacion: %d ",paquete.cod_operacion);
		int cod_op = paquete.cod_operacion;

		if (cod_op == 0)
			cod_op = -1;
        switch (cod_op) {
			case MATE_INIT:
				log_info(logger, "Inicio de MATE_INIT\n");
				proceso_id = des_entero(paquete);
				mem_mate_init(proceso_id,cliente_fd);
			break;
			case MEMALLOC:
				mate_memalloc = des_msj_code_memalloc(paquete);

				direccionLogica = -1;
				int alloc_size = mate_memalloc.tam_alloc; 	//Llega por socket
				int proceso_id = mate_memalloc.pid_carpincho; 	//Llega por socket
				printf("Pre mutex\n");
				pthread_mutex_lock(&mutex_mem_alloc);
				printf("Post mutex\n");
				if(hayAllocDisponible(alloc_size, proceso_id))
					direccionLogica = asignMemory(alloc_size, proceso_id);

				pthread_mutex_unlock(&mutex_mem_alloc);
				printf("DL: %d\n", direccionLogica);
				t_package paquete_res = ser_res_memalloc(direccionLogica);
				sendMessage(paquete_res, cliente_fd);
				break; 
			case MEMWRITE:
				log_info(logger, "Operación memwrite");
				printf("Pre deserializador\n");
				mate_memwrite = des_msj_code_memwrite(paquete);
				printf("Post deserializador\n");
				direccionLogica = memwrite(mate_memwrite);
				printf("Dirección logica escritura: %d\n", direccionLogica);
				paquete_res = ser_res_memwrite(direccionLogica);
				sendMessage(paquete_res, cliente_fd);
				printf("Fin\n");
				break;
			case MEMFREE:
				log_info(logger, "Operación memfree");
				//Devuelve: no devuelve (hito3)

				//send(cliente_fd,&respuesta,sizeof(respuesta),0);
				break;
            case MEMREAD:
				log_info(logger, "Operación memread");
				msj = des_msj_code_memwrite(paquete);
				//log_info(logger, "Me llego el mensaje: %s", item.message);
				msj.contenido = memread(msj);
				
				send(cliente_fd,msj.contenido,sizeof(msj.contenido),0);
				break;
				//  Mensajes cuando kernel no esta ejecutando  //
			case SEM_WAIT:
				log_error(logger, "Error en SEM_WAIT. Kernel no iniciado.");
				send(cliente_fd, ERROR_KERNEL_OFF, sizeof(error_code), 0);
				break;
			case SEM_POST:
				log_error(logger, "Error en SEM_POST. Kernel no iniciado.");
				send(cliente_fd, ERROR_KERNEL_OFF, sizeof(error_code), 0);
				break;
			case SEM_DESTROY:
				log_error(logger, "Error en SEM_DESTROY. Kernel no iniciado.");
				send(cliente_fd, ERROR_KERNEL_OFF, sizeof(error_code), 0);
				break;
			case CALL_IO:
				log_error(logger, "Error en CALL_IO. Kernel no iniciado.");
				send(cliente_fd, ERROR_KERNEL_OFF, sizeof(error_code), 0);
				break;
			case SWAP_OUT: 
				proceso_id = des_entero(paquete);
				swap_out_process(proceso_id);
				break;	
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				free(paquete.buffer);
				return EXIT_FAILURE;
			default:
				max_cantidad++;
				if(max_cantidad > 1){
					log_error(logger, "El cliente se desconecto. Terminando servidor");
					return EXIT_FAILURE;
				}
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
		free(paquete.buffer);
	}
	return NULL;
}


void iniciar_servidor_memoria(char * ip, char * puerto){
    int server_fd = iniciar_servidor(ip, puerto);
	log_info(logger, "Servidor listo para recibir al cliente");
	
	
	int cliente_fd;
	pthread_t hilo_client;
	
	if (server_fd < 0)
	{
		perror("Server MEMORIA no creado:");
	}
	while (1)
	{
		cliente_fd = esperar_cliente(server_fd);

		if (cliente_fd != -1)
		{
			log_info(logger,"Se conectó un cliente: %d", cliente_fd);
			pthread_create(&hilo_client,NULL,(void *)ejecutar_operacion_memoria,(void *)cliente_fd);
			pthread_detach(hilo_client);
		}else{
			break;
		}
	}
	liberar_conexion(cliente_fd);
}
