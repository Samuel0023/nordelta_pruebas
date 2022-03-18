#ifndef SD_SEMAPHORE_H
#define SD_SEMAPHORE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <string.h>
#include "TAD.h"

t_package ser_semaphore(t_semaphore data_buffer, int operationCode);
t_package ser_semaphore_SEM_INIT(t_semaphore data_buffer);
t_package ser_semaphore_SEM_WAIT(t_semaphore data_buffer);
t_package ser_semaphore_SEM_POST(t_semaphore data_buffer);
t_package ser_semaphore_SEM_DESTROY(t_semaphore data_buffer);
t_semaphore des_semaphore(t_package paquete);
void show_semaphore(t_semaphore data_buffer);

#endif