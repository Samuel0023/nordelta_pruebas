#ifndef TAD_H_
#define TAD_H_
#include <netdb.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <commons/bitarray.h>

typedef enum
{
	MENSAJE = 1,
	PAQUETE = 2,

	MEMALLOC = 3,
	MEMFREE = 4,
	MEMWRITE = 5,
	MEMREAD = 6,
	SEM_INIT = 7,
	SEM_WAIT=8,
	SEM_POST=9,
	SEM_DESTROY=10,
	CALL_IO,

	//SWAMP
	LECTURA_PAGINA,
	ESCRITURA_PAGINA,
	BORRADO_PAGINA,
	CANT_MARCOS_SWAMP,

	VOID,
	MATE_INIT,
	MATE_CLOSE,
	SWAP_OUT
} op_code;

typedef enum{
	ERROR_KERNEL_OFF = -1
} error_code;

typedef enum
{
	NEW,
	READY,
	EXEC,
	SUSPENDED_READY,
	SUSPENDED_BLOCKED,
	BLOCKED,
	EXIT
} status_carpincho;

// enum mate_errors {
//     MATE_FREE_FAULT = -5,
//     MATE_READ_FAULT = -6,
//     MATE_WRITE_FAULT = -7
// };

typedef struct
{
	uint32_t size;
	void *stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer *buffer;
} t_paquete;

typedef struct
{
	op_code cod_operacion;
	int tam_buffer;
	void *buffer;
} t_package;

typedef struct
{
	uint32_t long_message;
	char *message;
} t_message;

typedef struct
{
	uint32_t amount_messages;
	t_list *messages;
} t_list_message;

typedef struct{
	void* content;
	int contentSize;
	int proceso_id;
	int page_id;
} t_page_swap;

typedef struct{
	int respuesta_cantidad_marcos;
} t_cant_marcos_swamp;

typedef struct
{
	int pid_carpincho;
	int tam_alloc;
} t_msj_memalloc;

typedef struct
{
	int pid_carpincho;
	int dir_logica;
} t_msj_memfree;

typedef struct
{
	int pid_carpincho;
	int dir_logica;
	int tam_contenido;
	void * contenido;
} t_msj_memwrite;

typedef struct
{
	uint32_t long_name;
	char* name;
	int value;
	int processID;
} t_semaphore;  //materef

typedef struct{
	void *contenido;
	int tamanioContenido;
	int proceso_id;
	int page_id;
}PageSwap;

typedef struct
{
	uint32_t id_carpincho;
} t_mate_close;

typedef struct
{
	uint32_t id_carpincho;
	int tam_nombre_io;
	char * nombre_io;
	int tam_mensaje;
	char * mensaje;
} t_mate_call_io;

typedef struct
{
	void *contenido;
	int tamanio;
}respuesta_lectura_pagina;

typedef struct
{
	double estimacion_anterior;
	//analizar que otros datos vienen de config y sirven para mate init
}t_carpincho_init;


#endif /* TAD_H_ */
