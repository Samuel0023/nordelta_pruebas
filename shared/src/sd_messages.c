#include "../include/sd_messages.h"

t_package ser_message(t_message data_buffer){
    t_package paquete;
    int tam_buffer = sizeof(int) + data_buffer.long_message;
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = MEMALLOC;
    paquete.tam_buffer = tam_buffer;
    int offset = 0;

    memcpy(paquete.buffer + offset, &data_buffer.long_message, sizeof(int));
	offset += sizeof(int);

    memcpy(paquete.buffer + offset, data_buffer.message, data_buffer.long_message);
	offset += data_buffer.long_message;
    
    return paquete;
}

t_message des_message(t_package paquete){
    t_message data;
    void *buffer = paquete.buffer;
    memcpy( &data.long_message, buffer, sizeof(int));
    buffer += sizeof(int);

    data.message = malloc(data.long_message + 1);
    memcpy( data.message, buffer, data.long_message);
    buffer += data.long_message;

    data.message[data.long_message] = '\0';
    return data;
}

t_package ser_messages(t_list_message data_buffer){
    t_package paquete;
    int tamanio_caracteres = 0;
    for(int c=0; c < data_buffer.amount_messages; c++){
        t_message* item = list_get(data_buffer.messages, c);
        tamanio_caracteres += strlen(item->message);
    }


    int tam_buffer = sizeof(int) + data_buffer.amount_messages * (sizeof(int) + tamanio_caracteres );
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = PAQUETE;
    paquete.tam_buffer = tam_buffer;
    int offset = 0;

    memcpy(paquete.buffer + offset, &data_buffer.amount_messages, sizeof(int));
	offset += sizeof(int);

    for(int c=0; c < data_buffer.amount_messages; c++){
        t_message* item = list_get(data_buffer.messages, c);
        
        memcpy(paquete.buffer + offset, &item->long_message, sizeof(int));
	    offset += sizeof(int);
        memcpy(paquete.buffer + offset, item->message, item->long_message);
	    offset += item->long_message;
    }
    
    return paquete;
}

t_list_message des_messages(t_package paquete){
    t_list_message data;
    data.messages = list_create();
    void *buffer = paquete.buffer;
    memcpy( &data.amount_messages, buffer, sizeof(int));
    buffer += sizeof(int);
    
    for(int c=0; c<data.amount_messages; c++){
        t_message* item = malloc(sizeof(t_message));

        memcpy( &item->long_message, buffer, sizeof(int));
        buffer += sizeof(int);
        
        item->message = malloc(item->long_message + 1);
        memcpy( item->message, buffer, item->long_message);
        buffer += item->long_message;
        item->message[item->long_message] = '\0';

        list_add(data.messages, item);
    }

    return data;
}

void show_message(t_message data_buffer){
}