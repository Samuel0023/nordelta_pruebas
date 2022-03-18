#ifndef TAD_SWAMP_H
#define TAD_SWAMP_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netdb.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>

typedef enum
{
    ASIGNACION_FIJA = 1,
    ASIGNACION_DINAMICA = 2,
} code_asig;

typedef struct
{
    int pageID;
    int marco;
} Page;

typedef struct
{
    int procesoID;
    int indiceArchivo;
    t_list *paginas;
} PageTable;

typedef struct
{
    char *path;
    t_bitarray *bitmap;
    t_list *bitmap_reservado;
} t_file;

t_list *t_paths;
t_list *t_files;
t_list *tabla_paginas;
int MARCOS_POR_CARPINCHO;
int marcos_en_swamp;
#endif