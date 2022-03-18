#include "matelib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <stdint.h>

//------------------General Functions---------------------/
typedef struct mate_inner_structure
{
  //void *memory;
  t_log* logger;
  t_config *config;
  int socket;
  int id;
} mate_inner_structure;

int getSocket(mate_inner_structure* mate){
  char* ip = config_get_string_value(mate->config,"IP_KERNEL");
  char* puerto = config_get_string_value(mate->config,"PUERTO_KERNEL");
  
  int socket_kernel = crear_conexion(ip, puerto);
  int socket = socket_kernel;
  if(socket<0){
    ip = config_get_string_value(mate->config,"IP_MEMORIA");
    puerto = config_get_string_value(mate->config,"PUERTO_MEMORIA");
    socket = crear_conexion(ip, puerto);
  }
  return socket;
}

int mate_init(mate_instance *lib_ref, char *config_path)
{
  lib_ref->group_info = malloc(sizeof(mate_inner_structure));
  mate_inner_structure* mate = ((mate_inner_structure *)lib_ref->group_info);

  mate->config = leer_config_file(config_path);
  mate->socket = getSocket(mate);
  char* path_logger = config_get_string_value(mate->config,"LOG");

  mate->logger = iniciar_logger(path_logger, "Carpincho");
  log_info(mate->logger, "Logger iniciado");

  log_info(mate->logger, "Socket Kernel %d", mate->socket);
  t_package package;
  package.tam_buffer = 1;
  package.buffer = malloc(package.tam_buffer);
  package.cod_operacion = MATE_INIT;
  sendMessage(package, mate->socket);
  package = recibir_mensaje(mate->socket);

  int proceso_id = des_respuesta_id(package);
  mate->id = proceso_id;
  log_info(mate->logger,"Proceso id: %d\n", mate->id);

  return 0;
}

int mate_close(mate_instance *lib_ref)
{
  mate_inner_structure* mate = ((mate_inner_structure *)lib_ref->group_info);/// ?
  //enviar mensaje a kernel para que nos pase a exit
  t_mate_close data;
  data.id_carpincho = mate->id;

  t_package package = ser_msj_code_mete_close(data);
  sendMessage(package, mate->socket);
  log_info(mate->logger, "Se envia mensaje de MATE_CLOSE a kernel");
  log_info(mate->logger, "Finaliza carpincho: %d", mate->id);
    
  liberar_conexion(mate->socket);
  borrar_log(mate->logger);
  config_destroy(mate->config);    

  free(lib_ref->group_info);
  
  return 0;
}

//-----------------Semaphore Functions---------------------/

int mate_sem_init(mate_instance *lib_ref, mate_sem_name sem, unsigned int value) {
  
  mate_inner_structure* mate = ((mate_inner_structure *)lib_ref->group_info);
  // enviar mensaje a kernel para que cree el semaforo -- tiene que ser global
  t_package paquete;
  t_semaphore data;

  data.value = value;
  data.name = sem;
  data.long_name = strlen(sem);
  data.processID = mate->id;

  paquete = ser_semaphore_SEM_INIT(data);

  sendMessage(paquete, mate->socket);
  //esperar respuesta de kernel
  t_package package = recibir_mensaje(mate->socket);
  int res = des_respuesta_kernel_sem(package);

  return res;
}

int mate_sem_wait(mate_instance *lib_ref, mate_sem_name sem) {
  mate_inner_structure* mate = ((mate_inner_structure *)lib_ref->group_info);
  // enviar mensaje a kernel para que cree el semaforo -- tiene que ser global
  t_package paquete;
  t_semaphore data;

  data.name = sem;
  data.long_name = strlen(sem);
  data.processID = mate->id;
  data.value = 0;

  paquete = ser_semaphore_SEM_WAIT(data);

  sendMessage(paquete, mate->socket);
  //esperar respuesta de kernel 
  t_package package = recibir_mensaje(mate->socket);
  int res = des_respuesta_kernel_sem(package);

  return res;
}

int mate_sem_post(mate_instance *lib_ref, mate_sem_name sem) {
  mate_inner_structure* mate = ((mate_inner_structure *)lib_ref->group_info);
  // enviar mensaje a kernel para que cree el semaforo -- tiene que ser global
  t_package paquete;
  t_semaphore data;

  data.name = sem;
  data.long_name = strlen(sem);
  data.processID = mate->id;

  paquete = ser_semaphore_SEM_POST(data);

  sendMessage(paquete, mate->socket);
  //esperar respuesta de kernel
  t_package package = recibir_mensaje(mate->socket);
  int res = des_respuesta_kernel_sem(package);

  return res;
}

int mate_sem_destroy(mate_instance *lib_ref, mate_sem_name sem) {
  mate_inner_structure* mate = ((mate_inner_structure *)lib_ref->group_info);
  // enviar mensaje a kernel para que cree el semaforo -- tiene que ser global
  t_package paquete;
  t_semaphore data;

  data.name = sem;
  data.long_name = strlen(sem);
  data.processID = mate->id;

  paquete = ser_semaphore_SEM_DESTROY(data);

  sendMessage(paquete, mate->socket);
  //esperar respuesta de kernel 
  t_package package = recibir_mensaje(mate->socket);
  int res = des_respuesta_kernel_sem(package);

  return res;
}

//--------------------IO Functions------------------------/

int mate_call_io(mate_instance *lib_ref, mate_io_resource io, void *msg)
{
  mate_inner_structure* mate = ((mate_inner_structure *)lib_ref->group_info);
  t_mate_call_io data;
  data.mensaje = (char *) msg;
  data.nombre_io = (char *) io;
  data.tam_mensaje = strlen(data.mensaje);
  data.tam_nombre_io = strlen(data.nombre_io);
  data.id_carpincho = mate->id;
  log_info(mate->logger, "Se quiere utilizar %s", data.nombre_io);
  log_info(mate->logger, "%s", data.mensaje);

  t_package paquete = ser_msj_code_mete_call_io(data);
  sendMessage(paquete, mate->socket);
  //repuesta de  kernel
  //completar
  return 0;
}

//--------------Memory Module Functions-------------------/

mate_pointer mate_memalloc(mate_instance *lib_ref, int size)
{
  //((mate_inner_structure *)lib_ref->group_info)->memory = malloc(size);
  t_msj_memalloc data;
  data.pid_carpincho= ((mate_inner_structure *)lib_ref->group_info)->id;
  data.tam_alloc = size;

  t_package paquete = ser_msj_code_memalloc(data);
  sendMessage(paquete,((mate_inner_structure *)lib_ref->group_info)->socket);
  log_info(((mate_inner_structure *)lib_ref->group_info)->logger, "Solicitud de un alloc de %d bytes", size);
  //respuesta
  t_package package = recibir_mensaje(((mate_inner_structure *)lib_ref->group_info)->socket);

  int res = des_respuesta_memoria(package);
  mate_pointer dir= (mate_pointer) res;

  if (dir == MATE_FREE_FAULT)
  {
    log_info(((mate_inner_structure *)lib_ref->group_info)->logger, "No hay espacio en memoria, code error: %d", dir);
  }
  
  return dir;
}

int mate_memfree(mate_instance *lib_ref, mate_pointer addr)
{
  
  //free(((mate_inner_structure *)lib_ref->group_info)->memory);
  t_msj_memfree data;
  data.dir_logica = (int) addr;
  data.pid_carpincho = ((mate_inner_structure *)lib_ref->group_info)->id;

  t_package paquete = ser_msj_code_memfree(data);
  sendMessage(paquete, ((mate_inner_structure *)lib_ref->group_info)->socket);
  //respuesta
  t_package package = recibir_mensaje(((mate_inner_structure *)lib_ref->group_info)->socket);

  int res = des_respuesta_memoria(package);
  mate_pointer dir= (mate_pointer) res;

  if (dir == MATE_FREE_FAULT)
  {
    log_info(((mate_inner_structure *)lib_ref->group_info)->logger, "Direccion no valida para MATE_MEMFREE, code error: %d", dir);
  }

  return dir;
}

int mate_memread(mate_instance *lib_ref, mate_pointer origin, void *dest, int size)
{
  //memcpy(dest, ((mate_inner_structure *)lib_ref->group_info)->memory, size);
  t_msj_memwrite data;
  data.contenido = dest;
  data.dir_logica = (int) origin;
  data.pid_carpincho = ((mate_inner_structure *)lib_ref->group_info)->id;
  data.tam_contenido = size;

  t_package paquete = ser_msj_code_memread(data);
  sendMessage(paquete, ((mate_inner_structure *)lib_ref->group_info)->socket);

  //respuesta
  t_package package = recibir_mensaje(((mate_inner_structure *)lib_ref->group_info)->socket);

  int res = des_respuesta_memoria(package);
  mate_pointer dir= (mate_pointer) res;

  if (dir == MATE_READ_FAULT)
  {
    log_info(((mate_inner_structure *)lib_ref->group_info)->logger, "Direccion no valida para MATE_MEMREAD, code error: %d", dir);
  }

  return dir;
}

int mate_memwrite(mate_instance *lib_ref, void *origin, mate_pointer dest, int size)
{
  t_msj_memwrite data;
  data.contenido = origin;
  data.dir_logica = (int) dest;
  data.pid_carpincho = ((mate_inner_structure *)lib_ref->group_info)->id;
  data.tam_contenido = size;

  t_package paquete = ser_msj_code_memwrite(data);
  sendMessage(paquete, ((mate_inner_structure *)lib_ref->group_info)->socket);

  //respuesta
  t_package package = recibir_mensaje(((mate_inner_structure *)lib_ref->group_info)->socket);

  int res = des_respuesta_memoria(package);
  printf("Recepcion mem write\n");
  mate_pointer dir= (mate_pointer) res;

  if (dir == MATE_WRITE_FAULT) 
  {
    log_info(((mate_inner_structure *)lib_ref->group_info)->logger, "Direccion no valida para MATE_MEMWRITE, code error: %d", dir);
  }

  return dir;
}