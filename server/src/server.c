#include "server.h"

void *recibir_mensajes_servidor_nombre_modulo(int cliente_fd);

int main()
{
	logger = iniciar_logger("/server.log", "Servidor", 1, LOG_LEVEL_DEBUG);
	iniciar_servidor_proceso("127.0.0.1", "4444");

	borrar_log(logger);
	return EXIT_SUCCESS;
}

void iniciar_servidor_proceso(char *ip, char *puerto)
{
	int server_fd = iniciar_servidor(ip, puerto);
	log_info(logger, "Servidor listo para recibir al cliente");

	int cliente_fd;
	pthread_t hilo_client;

	while (1)
	{
		cliente_fd = esperar_cliente(server_fd);
		log_info(logger, "Se conectó un cliente: %d", cliente_fd);

		if (cliente_fd != -1)
		{
			pthread_create(
				&hilo_client,
				NULL,
				(void *)recibir_mensajes_servidor_nombre_modulo,
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
//void recibir_mensajes_servidor_kernel();
void *recibir_mensajes_servidor_nombre_modulo(int cliente_fd)
{
	t_package paquete;
	t_message item;
	t_list_message lista;

	int max_cantidad = 0;
	while (1)
	{
		paquete = recibir_mensaje(cliente_fd);
		int cod_op = paquete.cod_operacion;

		if (cod_op == 0)
			cod_op = -1;

		log_info(logger,"Codigo operación: %d", cod_op);
		switch (cod_op)
		{
		case MENSAJE:
			max_cantidad = 0;
			item = des_message(paquete);
			log_info(logger, "Me llego el mensaje: %s", item.message);
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

			log_info(logger, "Me llegaron los siguientes valores: %s", elementos);

			//log_info(logger, "Me llegaron los siguientes valores:\n");
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
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			free(paquete.buffer);
			return EXIT_FAILURE;
		default:
			max_cantidad++;
			if (max_cantidad > 1)
			{
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				return EXIT_FAILURE;
			}
			log_warning(logger,
						"Operacion desconocida. No quieras meter la pata");
			break;
		}
		free(paquete.buffer);
	}
	return NULL;
}