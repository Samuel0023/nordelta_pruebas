#include "../include/sd_pageswamp.h"

t_package ser_pageswamp(t_page_swap data_buffer){
    t_package paquete;
    int tam_buffer = sizeof(int) * 3 + data_buffer.contentSize;
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = ESCRITURA_PAGINA;
    paquete.tam_buffer = tam_buffer;
    int offset = 0;

    memcpy(paquete.buffer + offset, &data_buffer.contentSize, sizeof(int));
	offset += sizeof(int);

    memcpy(paquete.buffer + offset, data_buffer.content, data_buffer.contentSize);
	offset += data_buffer.contentSize;

    memcpy(paquete.buffer + offset, &data_buffer.proceso_id, sizeof(int));
	offset += sizeof(int);
    
    memcpy(paquete.buffer + offset, &data_buffer.page_id, sizeof(int));
	offset += sizeof(int);

    return paquete;
}

t_page_swap des_pageswamp(t_package paquete){
    t_page_swap data;
    void *buffer = paquete.buffer;
    memcpy( &data.contentSize, buffer, sizeof(int));
    buffer += sizeof(int);

    data.content = malloc(data.contentSize);
    memcpy( data.content, buffer, data.contentSize);
    buffer += data.contentSize;

    memcpy( &data.proceso_id, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy( &data.page_id, buffer, sizeof(int));
    buffer += sizeof(int);

    return data;
}

void show_pageswamp(t_message data_buffer){
}