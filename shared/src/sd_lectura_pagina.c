#include "../include/sd_lectura_pagina.h"
//cambiar el nombre a sd_page_swamp
/* ******* MEMORIA ******* */
t_package ser_lectura_pagina(PageSwap data_buffer)
{
    t_package paquete;
    int tam_buffer = 3 * sizeof(int) + data_buffer.tamanioContenido; //aca se setea en 1 el tamanioContenido ??
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = LECTURA_PAGINA; // es correcto ese cod_operacion ??
    paquete.tam_buffer = tam_buffer;
    int offset = 0;

    memcpy(paquete.buffer + offset, &data_buffer.tamanioContenido, sizeof(int));
    offset += sizeof(int);

    memcpy(paquete.buffer + offset, data_buffer.contenido, data_buffer.tamanioContenido);
    offset += data_buffer.tamanioContenido;

    memcpy(paquete.buffer + offset, &data_buffer.proceso_id, sizeof(int));
    offset += sizeof(int);

    memcpy(paquete.buffer + offset, &data_buffer.page_id, sizeof(int));
    offset += sizeof(int);

    return paquete;
}
//reemplazar por:
t_package ser_page_swamp(PageSwap data_buffer, int opCode)
{
    t_package paquete;
    int tam_buffer = 3 * sizeof(int) + data_buffer.tamanioContenido; 
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = opCode;
    paquete.tam_buffer = tam_buffer;
    int offset = 0;

    memcpy(paquete.buffer + offset, &data_buffer.tamanioContenido, sizeof(int));
    offset += sizeof(int);

    memcpy(paquete.buffer + offset, data_buffer.contenido, data_buffer.tamanioContenido);
    offset += data_buffer.tamanioContenido;

    memcpy(paquete.buffer + offset, &data_buffer.proceso_id, sizeof(int));
    offset += sizeof(int);

    memcpy(paquete.buffer + offset, &data_buffer.page_id, sizeof(int));
    offset += sizeof(int);

    return paquete;
}
t_package ser_lectura_pagina_swamp(PageSwap data_buffer){
    return ser_page_swamp(data_buffer,LECTURA_PAGINA);    
}

t_package ser_escritura_pagina_swamp(PageSwap data_buffer){
    return ser_page_swamp(data_buffer,ESCRITURA_PAGINA);    
}

t_package ser_borrado_pagina_swamp(PageSwap data_buffer){
    return ser_page_swamp(data_buffer,BORRADO_PAGINA);    
}


respuesta_lectura_pagina des_respuesta_lectura_pagina(t_package paquete)
{
    respuesta_lectura_pagina data;
    void *buffer = paquete.buffer;

    memcpy(&data.tamanio, buffer, sizeof(int));
    buffer += sizeof(int);

    data.contenido = malloc(data.tamanio);
    memcpy(data.contenido, buffer, data.tamanio);
    buffer += data.tamanio;
    //data.contenido[data.tamanioContenido] = '\0';

    //
    return data;
}

/* *******SWAMP ******* */
PageSwap des_lectura_pagina(t_package paquete)
{
    PageSwap data;
    void *buffer = paquete.buffer;

    memcpy(&data.tamanioContenido, buffer, sizeof(int));
    buffer += sizeof(int);

    data.contenido = malloc(data.tamanioContenido);
    memcpy(data.contenido, buffer, data.tamanioContenido);
    buffer += data.tamanioContenido;
    //data.contenido[data.tamanioContenido] = '\0';

    //

    memcpy(&data.proceso_id, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&data.page_id, buffer, sizeof(int));
    buffer += sizeof(int);

    return data;
}
PageSwap des_page_swamp(t_package paquete)
{
    PageSwap data;
    void *buffer = paquete.buffer;

    memcpy(&data.tamanioContenido, buffer, sizeof(int));
    buffer += sizeof(int);

    data.contenido = malloc(data.tamanioContenido);
    memcpy(data.contenido, buffer, data.tamanioContenido);
    buffer += data.tamanioContenido;
    //data.contenido[data.tamanioContenido] = '\0';

    //

    memcpy(&data.proceso_id, buffer, sizeof(int));
    buffer += sizeof(int);

    memcpy(&data.page_id, buffer, sizeof(int));
    buffer += sizeof(int);

    return data;
}
//serializar void *

t_package ser_respuesta_lectura_pagina(respuesta_lectura_pagina data_buffer)
{
    t_package paquete;
    int tam_buffer = sizeof(int) + data_buffer.tamanio;
    paquete.buffer = malloc(tam_buffer);
    paquete.cod_operacion = VOID; // es correcto ese cod_operacion ??
    paquete.tam_buffer = tam_buffer;
    int offset = 0;

    memcpy(paquete.buffer + offset, &data_buffer.tamanio, sizeof(int));
    offset += sizeof(int);

    memcpy(paquete.buffer + offset, data_buffer.contenido, data_buffer.tamanio);
    offset += data_buffer.tamanio;

    return paquete;
}