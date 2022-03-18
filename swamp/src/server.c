#include "server.h"

void *recibir_mensajes_servidor_swamp(int cliente_fd);

void iniciar_servidor_swamp(char *ip, char *puerto)
{

	flag_terminar_swamp = 1;
	int server_fd = iniciar_servidor(ip, puerto);
	log_info(logger, "SWAMP listo para recibir al cliente");

	int cliente_fd;

	pthread_t hilo_client;

	while (1)
	{
		//break;
		cliente_fd = esperar_cliente(server_fd);
		log_info(logger, "Se conectó un cliente: %d", cliente_fd);

		if (cliente_fd != -1)
		{
			pthread_create(
				&hilo_client,
				NULL,
				(void *)recibir_mensajes_servidor_swamp,
				(void *)cliente_fd);
			pthread_detach(hilo_client);
		}
		else
		{
			break;
		}
	}
}

void iterator(t_log *logger, char *value)
{
	log_info(logger, "%s\n", value);
}

/*************** PRIVATE FUNCTIONS ****************/
void *recibir_mensajes_servidor_swamp(int cliente_fd)
{
	t_package paquete; //debe ser recibido por parametro ??
	t_package paquete_a_enviar;
	t_message item;
	t_list_message lista;
	PageSwap pagina_swap;
	PageTable *tabla_pagina_swap;
	Page *pagina;
	int max_cantidad = 0;
	int cant_marcos = 0;
	while (1)
	{
		paquete = recibir_mensaje(cliente_fd);
		int cod_op = paquete.cod_operacion;

		if (cod_op == 0)
			cod_op = -1;

		switch (cod_op)
		{
			case CANT_MARCOS_SWAMP:
				log_info(logger, "CANT MARCOS SWAMP");
				cant_marcos = get_cant_marcos_swamp();
				
				paquete_a_enviar = ser_entero(cant_marcos, 0);
				sendMessage(paquete_a_enviar, cliente_fd);
				break;
			case LECTURA_PAGINA:
				log_info(logger, "LECTURA PAGINA");

				//PageSwap pagina_swap
				pagina_swap = des_lectura_pagina(paquete);
				void* content = lectura_pagina(pagina_swap);

				respuesta_lectura_pagina respuesta_a_enviar;
				respuesta_a_enviar.tamanio = atoi(config->TAMANIO_PAGINA);
				respuesta_a_enviar.contenido = content;

				paquete_a_enviar = ser_respuesta_lectura_pagina(respuesta_a_enviar);
				sendMessage(paquete_a_enviar, cliente_fd);

				break;
			case ESCRITURA_PAGINA: 
				log_info(logger, "ESCRITURA PAGINA");
				pagina_swap = des_lectura_pagina(paquete);
				guardarContenido(pagina_swap);
				break;
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				free(paquete.buffer);
				return EXIT_FAILURE;
			default:
				max_cantidad++;
				if (max_cantidad > 1)
				{
					log_error(logger, "el cliente se desconecto. Terminando servidor");
					return EXIT_FAILURE;
					//aca podria liberarse las estructuras
				}
				log_warning(logger,
							"Operacion desconocida. No quieras meter la pata");
				break;
		}
		free(paquete.buffer);
	}
	return NULL;
}