#include "memoria.h"

void MySignalHandler(int sig)
{
	switch (sig)
	{
		case SIGINT:
			mostrar_tlb_hit_miss_globales();
			mostrar_tlb_hit_miss_por_proceso();
			exit(1);
		case SIGUSR1:
			generar_dump_tlb();
			break;
		case SIGUSR2:
			printf("Aca se borraran las entradas de la tlb\n");
			limpiar_entradas_tlb();
			break;
		default:
			break;
	}
}

void inicializacion()
{

	int cant_frames = get_tamanio() / get_tamanio_pagina();
	int BYTES_CONFIG = get_tamanio();

	t_heap_meta_data heapMetaData = crearHeapMetaData(NULL, NULL, false);
	char *IP_SWAMP = get_ip_swamp();
	char *PUERTO_SWAMP = get_puerto_swamp();

	//pageTables = list_create();

	if (stream_memoria == NULL)
	{
		stream_memoria = malloc(BYTES_CONFIG);
	}

	bitmap = initialize_bitmap(cant_frames);
	reserved_bitmap = list_create();
	conexionSWAMP = crear_conexion(IP_SWAMP, PUERTO_SWAMP);
	pthread_mutex_init(&mutex_mem_alloc, NULL);
	memory_timestamp = 0;

	for (int c = 0; c < cant_frames; c++)
	{

		list_add(reserved_bitmap, 0);
	}

	inicializar_entradas_tlb();
	//void* heapMetaDataSerializado = serializarHeapMetaData(heapMetaData);
	//guardarHMDMP(0, 9, heapMetaDataSerializado)
}

void inicializacion_estructuras_memoria()
{
	pageTables = list_create();
	entradas_tlb = list_create();
	tlb_hit_totales = 0;
	tlb_miss_totales = 0;
	timestamp = 0;
	puntero_clock = 0;
	pthread_mutex_init(&MX_TIMESTAMP, NULL);
}

int main(int argc, char **argv)
{
	config = leer_config_file("/cfg/memoria.config");

	if (argc > 1 && strcmp(argv[1], "-test") == 0)
	{
		logger = iniciar_logger("/memoria.log", "MEMORIA", 1, LOG_LEVEL_DEBUG);
		run_tests();
		config_destroy(config);
		return 1;
	}

	signal(SIGINT, &MySignalHandler);
	signal(SIGUSR1, &MySignalHandler);
	signal(SIGUSR2, &MySignalHandler);

	// crear config_memoria
	char *ip = get_ip();
	char *puerto = get_puerto();
	inicializacion_estructuras_memoria();
	//show_values_config();
	inicializacion();

	logger = iniciar_logger("/memoria.log", "MEMORIA", 1, LOG_LEVEL_DEBUG);
	iniciar_servidor_memoria(ip, puerto);

	log_destroy(logger);
	config_destroy(config);
	return EXIT_SUCCESS;
}