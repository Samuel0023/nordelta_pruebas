#include "config_kernel.h"

//TODO:falta para DISPOSITVOS_IO /DURACIONES_IO

t_config_kernel *leer_config_kernel(t_config *config)
{
    t_config_kernel *config_kernel = malloc(sizeof(t_config_kernel));

    config_kernel->IP = config_get_string_value(config, "IP");
    config_kernel->PUERTO = config_get_int_value(config, "PUERTO");
    config_kernel->IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    config_kernel->PUERTO_MEMORIA = config_get_int_value(config, "PUERTO_MEMORIA");

    //Mapea el string a un int para el atributo algoritmo
    if (strcmp("SFJ", config_get_string_value(config, "ALGORITMO_PLANIFICACION")) == 0)
    {
        config_kernel->ALGORITMO_PLANIFICACION = SFJ;
    }
    else
    {
        config_kernel->ALGORITMO_PLANIFICACION = HRRN;
    }
    
    config_kernel->devices = list_create();
    char** devices_name = config_get_array_value(config, "DISPOSITIVOS_IO");
    char** devices_duration = config_get_array_value(config, "DURACIONES_IO");
    int indice = 0;
    void devices_iteration(char* item){
        t_device_config* device = malloc(sizeof(t_device_config));
        device->dispositivo = devices_name[indice];
        device->duration = atoi(devices_duration[indice]);
     
        free(devices_duration[indice]);
        list_add(config_kernel->devices, device);
        indice++;
    };
    string_iterate_lines(devices_name, devices_iteration);
    free(devices_duration);
    free(devices_name);

    config_kernel->GRADO_MULTIPROGRAMACION = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    config_kernel->GRADO_MULTIPROCESAMIENTO = config_get_int_value(config, "GRADO_MULTIPROCESAMIENTO");
    config_kernel->ALPHA = config_get_double_value(config, "ALFA");
    config_kernel->TIEMPO_DEADLOCK = config_get_int_value(config, "TIEMPO_DEADLOCK");
    config_kernel->ARCHIVO_LOG = config_get_string_value(config, "ARCHIVO_LOG");
    config_kernel->ESTIMACION_INICIAL = config_get_int_value(config, "ESTIMACION_INICIAL");
    return config_kernel;
}
void free_config_kernel(t_config_kernel *config_kernel){
    for(int c=0; c<list_size(config_kernel->devices);c++){
        t_device_config* device = list_get(config_kernel->devices, c);
        free(device->dispositivo);
        free(device);
    }
	list_destroy(config_kernel->devices);
    free(config_kernel);
}

void show_config_kernel(t_config_kernel *config){
    char* alg = "SJF";
    if(config->ALGORITMO_PLANIFICACION == HRRN)
        alg = "HRRN";

    printf("IP: %s\n", config->IP);
    printf("IP_MEMORIA: %s PUERTO_MEMORIA: %d\n", config->IP_MEMORIA, config->PUERTO_MEMORIA);
    printf("PUERTO_ESCUCHA: %d\n", config->PUERTO);
    printf("ALGORITMO PLANIFICACION: %s\n", alg);
    printf("DISPOSITIVOS_IO: pendiente\n");
    printf("DURACIONES_IO: pendiente\n");
    printf("GRADO_MULTIPROGRAMACION: %d\n", config->GRADO_MULTIPROGRAMACION);
    printf("GRADO_MULTIPROCESAMIENTO: %d\n", config->GRADO_MULTIPROCESAMIENTO);
    printf("TIEMPO DEADLOCK: %d\n", config->TIEMPO_DEADLOCK);
    printf("Dispositivos\n");
    for(int c=0; c<list_size(config->devices);c++){
        t_device_config* item = list_get(config->devices, c);
        printf("Nombre: %s Duración: %d\n", item->dispositivo, item->duration);
    }
}