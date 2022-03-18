#ifndef TESTS_H
#define TESTS_H
#include <CUnit/Basic.h>
#include <TAD.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/sd_lectura_pagina.h"

#include "../include/tad_swamp.h"
#include <commons/string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "../include/config_swamp.h"

extern t_config_swamp *config;
extern char **paths_string;
extern char *string_parcial;
int run_tests();
void verificarLecturaPagina();
void verificar_respuesta_lectura_pagina();
void verificar_llenar_archivo();
void verificar_devolver_paths();
void verificar_inicializacion();
void check_write_page();
void verificar_cant_marcos();

#endif