#ifndef SEMAFOROS_kERNEL_H_
#define SEMAFOROS_kERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <commons/log.h>
#include <fcntl.h>          
#include <sys/stat.h>        
#include <semaphore.h>
#include <pthread.h>
#include "utils.h"
#include "TAD.h"
#include "sd_messages.h"
#include "socket_client.h"
#include "deadlock.h"

extern t_list* semaphores_kernel; 
extern t_log* logger;

extern t_list *list_NEW;
extern t_list *list_READY;
extern t_list *list_EXEC;
extern t_list *list_SUSPENDED_READY;
extern t_list *list_SUSPENDED_BLOCKED;
extern t_list *list_BLOCKED;
extern t_list *lista_EXIT;

t_sem_global *get_semaphore(char* name);

void mate_sem_init_kernel(t_semaphore *semaphore);
void mate_sem_wait_kernel(t_semaphore *semaphore,int socket_cliente);
void mate_sem_post_kernel(t_semaphore *semaphore);
void mate_sem_destroy_kernel(t_semaphore *semaphore);

#endif /* SEMAFOROS_kERNEL_H_*/
