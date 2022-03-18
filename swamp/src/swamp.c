#include "swamp.h"

void inicializacion_de_recursos()
{
	MARCOS_POR_CARPINCHO = config->MARCOS_MAXIMOS;
	marcos_en_swamp = config->TAMANIO_SWAP / config->TAMANIO_PAGINA;
	t_paths = list_create();
	t_files = list_create();
	tabla_paginas = list_create();

	devolver_paths();
}

void devolver_paths()
{
	string_parcial = string_substring(config->ARCHIVOS_SWAP, 1, string_length(config->ARCHIVOS_SWAP) - 2);

	paths_string = string_split(string_parcial, ",");
	int cantidad_de_archivos = obtener_cantidad_argumentos(paths_string);
	for (int i = 0; i < cantidad_de_archivos; i++)
	{

		list_add(t_paths, paths_string[i]);
	}
	free(string_parcial);
}

int main(int argc, char **argv)
{

	logger = iniciar_logger("/swap.log", "Swamp");
	t_config *g_config = leer_config_file("/cfg/swap.config");
	config = leer_config_swamp(g_config);

	if (argc > 1 && strcmp(argv[1], "-test") == 0)
	{
		run_tests();
		config_destroy(g_config);
		return 1;
	}
	inicializacion_de_recursos();
	inicializacion();

	char *puerto = string_itoa(config->PUERTO);
	iniciar_servidor_swamp(config->IP, puerto);

	free(puerto);

	for (int i = 0; i < list_size(t_files); i++)
	{
		t_file *file = list_get(t_files, i);
		list_destroy(file->bitmap_reservado);
	}

	config_destroy(g_config);
	borrar_log(logger);
	return EXIT_SUCCESS;
}