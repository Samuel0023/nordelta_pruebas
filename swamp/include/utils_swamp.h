#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<time.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "tad_swamp.h"
#include "TAD.h"
#include "config_swamp.h"
#include "config_utils.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

extern t_log* logger;
char **paths_string;
char *string_parcial;
int flag_terminar_swamp;
extern t_log *logger;
extern t_config_swamp *config;

int esperar_cliente(int);
void recibir_mensaje_tp0(int);
void inicializacion_de_recursos();
void creacion_archivos_swap();
bool _file_bigger(t_file *file, t_file *file_less_bigger);
int find_better_file();
PageTable *getPageTable(int proceso_id);
PageTable *getPageTable_v2(int proceso_id);
void devolver_paths();
void liberar_puntero_doble(char **);
int obtener_cantidad_argumentos(char **);
char *llenar_archivo(int indice, char *dato);
int get_cant_marcos_swamp();

//----------------
void asignacionPageTable(PageTable *table,code_asig cod_op);
PageTable* get_page_table(int processID);
Page* get_page(PageTable *tabla_pagina_swap, int pageID);
void* get_stream_file(char* path);


#endif /* UTILS_H_ */