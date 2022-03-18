#ifndef CLIENT_H_
#define CLIENT_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include "utils.h"
#include "TAD.h"
#include "sd_messages.h"

void leer_consola(t_log*);
void enviar_clave(int, char*);
void terminar_programa(int, t_log*, t_config*);

#endif /* CLIENT_H_ */
