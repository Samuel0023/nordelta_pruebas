#ifndef MATE_CALL_IO_H_
#define MATE_CALL_IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>          
#include <sys/stat.h>        
#include "TAD.h"
#include "utils.h"
#include <semaphore.h>


extern t_list *devices;
extern t_list *list_READY;
extern t_list *list_EXEC;
extern t_list *list_SUSPENDED_READY;
extern t_list *list_SUSPENDED_BLOCKED;
extern t_list *list_BLOCKED;

bool existe_pcb(int process_id,t_device *device);
t_device *get_dispositivo(char* name_device);
void mate_call_io(char *name_device,int proceso_id);

#endif