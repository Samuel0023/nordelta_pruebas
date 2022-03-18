#ifndef MATELIB_H
#define MATELIB_H

#include <stdint.h>
#include <semaphore.h>
#include <commons/config.h>
#include "../shared/include/server_utils.h"
#include "../shared/include/log_utils.h"
#include "../shared/include/config_utils.h"
#include "../shared/include/shared_utils.h"
#include "../shared/include/socket_client.h"
#include "../shared/include/sd_messages.h"
#include "../shared/include/sd_mate_init.h"
#include "../shared/include/TAD.h"
#include "../shared/include/sd_mate_close.h"
#include "../shared/include/server_utils.h"
#include "../shared/include/sd_mate_call_io.h"
#include "../shared/include/sd_semaphore.h"
#include "../shared/include/sd_memalloc.h"
#include "../shared/include/sd_memfree.h"
#include "../shared/include/sd_memwrite.h"
#include "../shared/include/sd_res_memoria.h"
#include "../shared/include/sd_res_semaforo_kernel.h"

//-------------------Type Definitions----------------------/
typedef struct mate_instance
{
    void *group_info;
} mate_instance;

typedef char *mate_io_resource;

typedef char *mate_sem_name;

typedef int32_t mate_pointer;

enum mate_errors {
    MATE_FREE_FAULT = -5,
    MATE_READ_FAULT = -6,
    MATE_WRITE_FAULT = -7
};

// TODO: Docstrings

//------------------General Functions---------------------/
int mate_init(mate_instance *lib_ref, char *config);

int mate_close(mate_instance *lib_ref);

//-----------------Semaphore Functions---------------------/
int mate_sem_init(mate_instance *lib_ref, mate_sem_name sem, unsigned int value);

int mate_sem_wait(mate_instance *lib_ref, mate_sem_name sem);

int mate_sem_post(mate_instance *lib_ref, mate_sem_name sem);

int mate_sem_destroy(mate_instance *lib_ref, mate_sem_name sem);

//--------------------IO Functions------------------------/

int mate_call_io(mate_instance *lib_ref, mate_io_resource io, void *msg);

//--------------Memory Module Functions-------------------/

mate_pointer mate_memalloc(mate_instance *lib_ref, int size);

int mate_memfree(mate_instance *lib_ref, mate_pointer addr);

int mate_memread(mate_instance *lib_ref, mate_pointer origin, void *dest, int size);

int mate_memwrite(mate_instance *lib_ref, void *origin, mate_pointer dest, int size);

#endif