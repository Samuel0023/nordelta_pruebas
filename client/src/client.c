#include "client.h"
#include "socket_client.h"
#include "unistd.h"
#include "stdlib.h"

t_list* messages;

/*
Protocolo para enviar un msj
int conexion = crear_conexion(ip, puerto);
t_package paquete = ser_message(item);
sendMessage(paquete, conexion);
*/

int main(void)
{
	messages = list_create();
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	t_log* logger = iniciar_logger("/client.log", "Cliente");
	// Usando el logger creado previamente
	log_info(logger, "Hola! Soy un log x2");
	
	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	t_config *g_config = leer_config_file("/cfg/client.config");
	char* ip = config_get_string_value(g_config,"IP_MODULO");
	char* puerto = config_get_string_value(g_config,"PUERTO_MODULO");
	char* valor = config_get_string_value(g_config,"CLAVE");
	char buf[100];
	int numbytes;
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion("127.0.0.1","5001");

	//Loggear valor de config
	//leer_consola(logger);

	//enviar CLAVE al servidor
	//enviar_clave(conexion, valor);
	
   	log_info(logger,"server connected %d", conexion);
	if(conexion != -1){

		enviar_clave(conexion,"1234");
	

			
   		if((numbytes=recv(conexion,buf,100,0)==-1)){
			log_info(logger,"Error en recv()");
			return 1;
		}
		buf[numbytes]='\0';
		log_info(logger ,"Mensaje del servidor %s",numbytes);

	}
	terminar_programa(conexion, logger, g_config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	return 1;
}

void leer_consola(t_log* logger)
{
	char* leido;

	//El primero te lo dejo de yapa
	leido = readline(">");
	while(strcmp(leido, "")){
		// Acá la idea es que imprimas por el log lo que recibis de la consola.
		list_add(messages, leido);
		log_info(logger, "%s", leido);
		leido = readline(">");
	}
	free(leido);
}

void enviar_clave(int conexion, char* valor)
{
	//Ahora toca lo divertido!
	t_message item;
	item.message = valor;
	item.long_message = strlen(valor);

	t_package paquete = ser_message(item);
    sendMessage(paquete, conexion);
}

void enviar_palabras(int conexion){
	t_list_message lista;
	lista.messages = list_create();
	for(int c=0; c<list_size(messages);c++){
		char* message = list_get(messages, c);
		t_message* item = malloc(sizeof(t_message));
		item->message = message;
		item->long_message = strlen(message);
		list_add(lista.messages, item);
	}
	lista.amount_messages = list_size(messages);

	t_package paquete = ser_messages(lista);
    sendMessage(paquete, conexion);

	void borrar_elementos(t_message* item){
		free(item->message);
		free(item);
	}
	list_destroy_and_destroy_elements(lista.messages, borrar_elementos);
}
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	liberar_conexion(conexion);
	borrar_log(logger);
	config_destroy(config);

	liberar_elementos_mensajes();
	list_destroy(messages);
}
