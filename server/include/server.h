#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utils.h"
#include "server_utils.h"
#include "TAD.h"
#include "sd_messages.h"

t_log* logger;

void iterator(t_log* logger, char* value);
void iniciar_servidor_proceso(char* ip, char* puerto);

#endif /* SERVER_H_ */
