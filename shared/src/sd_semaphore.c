#include "../include/sd_semaphore.h"

t_package ser_semaphore(t_semaphore data_buffer, int operationCode){
    t_package paquete;
    int tam_buffer = sizeof(int) * 3 + data_buffer.long_name;
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = operationCode;
    paquete.tam_buffer = tam_buffer;
    int offset = 0;

    memcpy(paquete.buffer + offset, &data_buffer.long_name, sizeof(int));
	offset += sizeof(int);

    memcpy(paquete.buffer + offset, data_buffer.name, data_buffer.long_name);
	offset += data_buffer.long_name;
    
    memcpy(paquete.buffer + offset, &data_buffer.processID, sizeof(int));
	offset += sizeof(int);

    memcpy(paquete.buffer + offset, &data_buffer.value, sizeof(int));
	offset += sizeof(int);

    return paquete;
}

t_package ser_semaphore_SEM_INIT(t_semaphore data_buffer){
    return ser_semaphore(data_buffer, SEM_INIT);    
}

t_package ser_semaphore_SEM_WAIT(t_semaphore data_buffer){
    return ser_semaphore(data_buffer, SEM_WAIT);    
}

t_package ser_semaphore_SEM_POST(t_semaphore data_buffer){
    return ser_semaphore(data_buffer, SEM_POST);    
}

t_package ser_semaphore_SEM_DESTROY(t_semaphore data_buffer){
    return ser_semaphore(data_buffer, SEM_DESTROY);    
}


t_semaphore des_semaphore(t_package paquete){
    t_semaphore data;
    void *buffer = paquete.buffer;
    memcpy( &data.long_name, buffer, sizeof(int));
    buffer += sizeof(int);

    data.name = malloc(data.long_name + 1);
    memcpy( data.name, buffer, data.long_name);
    buffer += data.long_name;

    data.name[data.long_name] = '\0';
    
    memcpy( &data.processID, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy( &data.value, buffer, sizeof(int));
    buffer += sizeof(int);
    return data;
}

void show_semaphore(t_semaphore data_buffer){
    printf("Nombre: %s\n", data_buffer.name);
    printf("Valor: %d\n", data_buffer.value);
    printf("Proceso: %d\n", data_buffer.processID);
}