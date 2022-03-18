#ifndef SD_MESSAGES_H
#define SD_MESSAGES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <string.h>
#include "TAD.h"

t_package ser_message(t_message data_buffer);
t_message des_message(t_package paquete);
t_package ser_messages(t_list_message data_buffer);
t_list_message des_messages(t_package paquete);

#endif