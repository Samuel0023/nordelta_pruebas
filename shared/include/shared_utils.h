#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "TAD.h"

//Funcion de ejemplo del git de VSCODE
char *mi_funcion_compartida();

t_log *iniciar_logger(char *, char *);

void escribir_log_info(t_log *, char *, char *);
void escribir_log_warning(t_log *, char *, char *);
void escribir_log_error(t_log *, char *, char *);

void borrar_log(t_log *);
char *get_string_operacion(op_code code);

#endif