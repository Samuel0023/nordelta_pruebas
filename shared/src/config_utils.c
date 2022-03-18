#include "../include/config_utils.h"

//"/cfg/client.config" formato de la variable
t_config *leer_config_file(char *configName)
{
	char *current_working_directory = getcwd(NULL, 0);

	char *path_config = string_new();
	string_append_with_format(&path_config, "%s", current_working_directory);
	string_append_with_format(&path_config, "%s", configName);

	t_config *g_config = config_create(path_config);

	free(path_config);
	free(current_working_directory);

	return g_config;
}



//TODO:falta para DISPOSITVOS_IO /DURACIONES_IO

// t_config_kernel *leer_config_kernel(t_config *config)
// {
// 	t_config_kernel *config_kernel = malloc(sizeof(t_config_kernel));

// 	config_kernel->IP = config_get_string_value(config, "IP");
// 	config_kernel->PUERTO = config_get_int_value(config, "PUERTO");
// 	config_kernel->IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
// 	config_kernel->PUERTO_MEMORIA = config_get_int_value(config, "PUERTO_MEMORIA");

// 	//Mapea el string a un int para el atributo algoritmo
// 	if (strcmp("SFJ", config_get_string_value(config, "ALGORITMO_PLANIFICACION")) == 0)
// 	{
// 		config_kernel->ALGORITMO_PLANIFICACION = SFJ;
// 	}
// 	else
// 	{
// 		config_kernel->ALGORITMO_PLANIFICACION = HRRN;
// 	}
// 	//Todo:falta para DISPOSITVOS_IO /DURACIONES_IO

// 	config_kernel->GRADO_MULTIPROGRAMACION = config_get_string_value(config, "GRADO_MULTIPROGRAMACION");
// 	config_kernel->GRADO_MULTIPROCESAMIENTO = config_get_string_value(config, "GRADO_MULTIPROCESAMIENTO");
// 	config_kernel->TIEMPO_DEADLOCK = config_get_int_value(config, "TIEMPO_DEADLOCK");
// 	//config_kernel->ARCHIVO_LOG = config_get_string_value(config, "ARCHIVO_LOG");

// 	return config_kernel;
// }

//
t_config_mongo_store *leer_config_mongo_store(t_config *config)
{
	t_config_mongo_store *config_mongo_store = malloc(sizeof(t_config_mongo_store));

	config_mongo_store->IP_DISCORDIADOR = config_get_string_value(config, "IP_DISCORDIADOR");
	config_mongo_store->PUERTO_DISCORDIADOR = config_get_int_value(config, "PUERTO_DISCORDIADOR");
	config_mongo_store->IP_MODULO = config_get_string_value(config, "IP_MODULO");
	config_mongo_store->PUERTO = config_get_int_value(config, "PUERTO");
	config_mongo_store->PUNTO_MONTAJE = config_get_string_value(config, "PUNTO_MONTAJE");
	config_mongo_store->TIEMPO_SINCRONIZACION = config_get_int_value(config, "TIEMPO_SINCRONIZACION");
	config_mongo_store->ARCHIVO_LOG = config_get_string_value(config, "ARCHIVO_LOG");
	config_mongo_store->BLOCK_SIZE = config_get_int_value(config, "BLOCK_SIZE");
	config_mongo_store->BLOCKS = config_get_int_value(config, "BLOCKS");

	char *posiciones_sabotaje_string = config_get_string_value(config, "POSICIONES_SABOTAJE");
	posiciones_sabotaje_string = string_substring(posiciones_sabotaje_string, 1, string_length(posiciones_sabotaje_string) - 2);

	config_mongo_store->POSICIONES_SABOTAJE = list_create();
	return config_mongo_store;
}