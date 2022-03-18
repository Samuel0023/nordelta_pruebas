#include "global.h"
#include "kernel.h"

int main(int argc, char **argv)
{
	t_config *g_config = leer_config_file("/cfg/kernel.config");
	config_kernel = leer_config_kernel(g_config);

	show_config_kernel(config_kernel);
	if (argc > 1 && strcmp(argv[1], "-test") == 0)
	{
		run_tests();
		//show_config_kernel(config_kernel);
		free_config_kernel(config_kernel);
		config_destroy(g_config);
		return 1;
	}
	inicializacion_estructuras();
	show_config_kernel(config_kernel);
	logger_kernel = iniciar_logger(config_kernel->ARCHIVO_LOG, "Kernel", 1, LOG_LEVEL_DEBUG);

	inicializacion_hilos();
	pthread_t thread_servidor;
	pthread_create(&thread_servidor, NULL, iniciar_servidor_kernel, NULL);
	pthread_join(thread_servidor, NULL);

	config_destroy(g_config);
	free_config_kernel(config_kernel);
	borrar_log(logger_kernel);
	liberar_memoria();

	return EXIT_SUCCESS;
}

void liberar_memoria()
{
	list_destroy(list_NEW);
	list_destroy(list_READY);
	list_destroy(list_EXEC);
	list_destroy(list_SUSPENDED_READY);
	list_destroy(list_SUSPENDED_BLOCKED);
	list_destroy(list_BLOCKED);
	list_destroy(lista_EXIT);
	list_destroy(semaphores_kernel);
}

void inicializacion_estructuras()
{
	// Inicialización de listas
	list_NEW = list_create();
	list_READY = list_create();
	list_EXEC = list_create();
	list_SUSPENDED_READY = list_create();
	list_SUSPENDED_BLOCKED = list_create();
	list_BLOCKED = list_create();
	lista_EXIT = list_create();

	// Inicialización de semaforos
	pthread_mutex_init(&MX_NEW, NULL);
	pthread_mutex_init(&MX_READY, NULL);
	pthread_mutex_init(&MX_EXEC, NULL);
	pthread_mutex_init(&MX_SUSPENDED_READY, NULL);
	pthread_mutex_init(&MX_SUSPENDED_BLOCKED, NULL);
	pthread_mutex_init(&MX_BLOCKED, NULL);
	pthread_mutex_init(&MX_EXIT, NULL);

	// Inicialización para semáforos destinados a multiprogramación y multiprocesamiento
	pthread_mutex_init(&MX_PCB, NULL);
	sem_init(&limite_grado_multiprocesamiento, 0, config_kernel->GRADO_MULTIPROCESAMIENTO);
	sem_init(&limite_grado_multiprogramacion, 0, config_kernel->GRADO_MULTIPROGRAMACION);
	sem_init(&cantidad_procesos_suspendidos, 0, 0);
	sem_init(&cant_procesos_listos, 0, 0);
	sem_init(&cantidad_procesos_nuevos_suspendidos, 0, 0);

	//Semáforos
	semaphores_kernel = list_create();
	devices = list_create();
	for (int c = 0; c < list_size(config_kernel->devices); c++)
	{
		t_device_config *item = list_get(config_kernel->devices, c);
		t_device *device = malloc(sizeof(t_device));

		device->name = item->dispositivo;
		device->duration = item->duration;
		device->pcbs = list_create();
		sem_init(&device->pcb_pendientes, 0, 0);
		pthread_mutex_init(&device->pcbs_mutex, NULL);

		list_add(devices, device);
	}

	//ID autoincrementable
	id_proceso = 1;
	pthread_mutex_init(&MX_ID_PROCESO, NULL);
	asignaciones = list_create();
	peticiones = list_create();
}