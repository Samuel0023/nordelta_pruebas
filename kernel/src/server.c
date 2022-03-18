#include "server.h"

extern t_config_kernel *config_kernel;
void *recibir_mensajes_servidor_kernel(int cliente_fd);

void* iniciar_servidor_kernel()
{
	char* ip = config_kernel->IP;
	char* puerto = string_itoa(config_kernel->PUERTO);
	int server_fd = iniciar_servidor(ip, puerto);
	free(puerto);
	log_info(logger_kernel, "Servidor listo para recibir al cliente");

	int cliente_fd;
	pthread_t hilo_client;

	while (1)
	{
		cliente_fd = esperar_cliente(server_fd);
		log_info(logger_kernel, "Se conectó un cliente: %d", cliente_fd);

		if (cliente_fd != -1)
		{
			pthread_create(
				&hilo_client,
				NULL,
				(void *)recibir_mensajes_servidor_kernel,
				(void *)cliente_fd);
			pthread_detach(hilo_client);
		}
		else
		{
			break;
		}
	}
	return NULL;
}

/*************** PRIVATE FUNCTIONS ****************/
void *recibir_mensajes_servidor_kernel(int cliente_fd)
{
	t_semaphore semaphore;
	t_package package_to_respond;
	t_package package_to_memory;

	t_package paquete;
	t_message item;
	t_list_message lista;

	t_mate_close data_close;
	t_mate_call_io data_io;
	t_msj_memalloc data;
	t_msj_memfree data_free;
	t_msj_memwrite data_write;
	t_msj_memwrite data_read;
	char* message;

	t_package paquete_res;
	t_package paquete_res_memoria;
	//t_package paquete_res_kernel;
	int code_res;

	int max_cantidad = 0;
	while (1)
	{
		paquete = recibir_mensaje(cliente_fd);
		int cod_op = paquete.cod_operacion;
		//printf("Cliente t: %d %d\n", cliente_fd, cod_op);
		char* puerto = string_itoa(config_kernel->PUERTO_MEMORIA);
		int conexion_memoria = crear_conexion(config_kernel->IP_MEMORIA, puerto);
		free(puerto);

		if (cod_op == 0)
			cod_op = -1;

		//printf("Codigo operación: %d", cod_op);

		switch (cod_op)
		{
			case SEM_INIT: 
				semaphore = des_semaphore(paquete);
				// return mate_sem_init(semaphore.name,semaphore.value);
				mate_sem_init_kernel(&semaphore);
				log_info(logger_kernel, "Se inicia semaforo: %s", semaphore.name);

				paquete_res_memoria = ser_res_mate_sem_init(semaphore.value);
				sendMessage(paquete_res_memoria, cliente_fd);
				break;
			case SEM_WAIT:
				semaphore = des_semaphore(paquete);
				log_info(logger_kernel, "SEM WAIT semaforo: %s", semaphore.name);
				mate_sem_wait_kernel(&semaphore,cliente_fd);
				break;
			case SEM_POST:
				// libera de un CARPINCHO a la vez, BLOCKED -> READY
				semaphore = des_semaphore(paquete);
				mate_sem_post_kernel(&semaphore);
				break;
			case SEM_DESTROY:
				mate_sem_destroy_kernel(&semaphore);
				break;
			case CALL_IO:
				//bloqueo al CARPINCHO segun rafaga
				// del dispositivo
				//definido por .config
				data_io = des_msj_code_mete_call_io(paquete);
				mate_call_io(data_io.nombre_io,data_io.id_carpincho);
				log_info(logger_kernel, "Carpincho solicita utilizar %s", data_io.nombre_io);
				free(data_io.mensaje);
				free(data_io.nombre_io);
				break;
        
			case MATE_INIT:
				log_info(logger_kernel, "Inicio de MATE_INIT\n");

				t_carpincho_init carpincho = des_carpincho_init(paquete);
				//int pid_nuevo = inicializar_carpincho_nuevo(carpincho.estimacion_anterior, cliente_fd);
				int pid_nuevo = inicializar_carpincho_nuevo(config_kernel->ESTIMACION_INICIAL, cliente_fd);
				
				log_info(logger_kernel, "Id del carpincho nuevo %d", pid_nuevo);
				enviar_id(cliente_fd, pid_nuevo);
				enviar_id(conexion_memoria, pid_nuevo);
				break;
			case MEMWRITE:
				data_write = des_msj_code_memwrite(paquete);
				log_info(logger_kernel, "Se quiere escribir en la dirección %d", data_write.dir_logica);
				//pasarselo a memoria
				
				printf("Con memoria: %d\n", conexion_memoria);
				if (conexion_memoria == -1) //ver
					exit(1);

				paquete = ser_msj_code_memwrite(data_write);
				sendMessage(paquete, conexion_memoria);

				//respuesta de memoria a kernel 
				paquete_res_memoria = recibir_mensaje(conexion_memoria);
				code_res = des_respuesta_memoria(paquete_res_memoria);

				printf("Code res de memoria: %d\n", code_res);
				enviar_respuesta_memwrite(cliente_fd,code_res);
				printf("End kernel\n");
				//free(paquete_res_memoria.buffer);
				break;
			case MEMREAD:
				data_read = des_msj_code_memwrite(paquete);
				log_info(logger_kernel, "Se quiere leer el contenido de la dirección %d", data_read.dir_logica);
				//pasarselo a memoria
				// if (conexion_memoria == -1) //ver
				// 	exit(1);

				// paquete_res = ser_msj_code_memread(data_read);
				// sendMessage(paquete_res, conexion_memoria);

				//respuesta de memoria a kernel
				paquete_res_memoria = recibir_mensaje(conexion_memoria);
				code_res = des_respuesta_memoria(paquete);
				enviar_respuesta_memread(cliente_fd,code_res);
				free(paquete_res_memoria.buffer);
				break;
			case MEMFREE:
				//dato de prueba
				data_free = des_msj_code_memfree(paquete);
				log_info(logger_kernel, "Se libera alloc del carpincho %d ", data_free.pid_carpincho);
				//pasarselo a memoria
				// if (conexion_memoria == -1) //ver
				// 	exit(1);

				// paquete = ser_msj_code_memfree(data_free);
				// sendMessage(paquete, conexion_memoria);

				//respuesta de memoria a kernel
				paquete_res_memoria = recibir_mensaje(conexion_memoria);
				code_res = des_respuesta_memoria(paquete);
				enviar_respuesta_memfree(cliente_fd,code_res);
				free(paquete_res_memoria.buffer);
				break;
			case MEMALLOC:
				//dato
				data = des_msj_code_memalloc(paquete);
				log_info(logger_kernel, "Se solicita un alloc de %d ", data.tam_alloc);
				//pasarselo a memoria
				if (conexion_memoria == -1)
				 	exit(1);

				paquete = ser_msj_code_memalloc(data);
				sendMessage(paquete, conexion_memoria);

				paquete_res_memoria = recibir_mensaje(conexion_memoria);
				code_res = des_respuesta_memoria(paquete_res_memoria);
				//code_res=-5; /*para probar enviar dir*/
				enviar_respuesta_memalloc(cliente_fd,code_res);
				
				break;
			case MATE_CLOSE:
				data_close = des_msj_code_mete_close(paquete);
				log_info(logger_kernel, "Finaliza el carpincho %d de MATE_CLOSE \n", data_close.id_carpincho);
				// debe pasar el carpincho a exit
				break;
			case MENSAJE:
				max_cantidad = 0;
				item = des_message(paquete);
				log_info(logger_kernel, "Me llego el mensaje: %s", item.message);
				free(item.message);
				break;			
			case PAQUETE:
				max_cantidad = 0;
				lista = des_messages(paquete);
				char *elementos = string_new();
				if (lista.amount_messages > 0)
				{
					for (int c = 0; c < lista.amount_messages - 1; c++)
						{
							t_message *item = list_get(lista.messages, c);
							string_append_with_format(&elementos, "%s, ", item->message);
						}

						t_message *item = list_get(lista.messages, lista.amount_messages - 1);
						string_append_with_format(&elementos, "%s", item->message);
					}

					log_info(logger_kernel, "Me llegaron los siguientes valores: %s", elementos);

					//log_info(logger_kernel, "Me llegaron los siguientes valores:\n");
					//list_iterate(lista, (void*) iterator);

					free(elementos);
					void borrar_elementos(t_message * item)
					{
						free(item->message);
						free(item);
					}
					list_destroy_and_destroy_elements(lista.messages, borrar_elementos);
					break;
			case -1:
				log_error(logger_kernel, "el cliente se desconecto. Terminando servidor");
				free(paquete.buffer);
				return EXIT_FAILURE;
			default:
				max_cantidad++;
				if (max_cantidad > 1)
				{
					log_error(logger_kernel, "el cliente se desconecto. Terminando servidor");
					return EXIT_FAILURE;
				}
				log_warning(logger_kernel,
							"Operacion desconocida. No quieras meter la pata");
			break;
		}
		free(paquete.buffer);
	}
	return NULL;
}