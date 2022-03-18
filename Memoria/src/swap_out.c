#include "swap_out.h"

void swap_out_process(int process_id){
    t_page_table* page_table = getTablaPaginas(process_id);
    for(int c=0; c<list_size(page_table->pages);c++){
        t_page* page = list_get(page_table->pages, c);
        if(page->modificado && page->presencia){
            PageSwap pagina_swap;
            int tamanio_pagina = get_tamanio_pagina();
            void* contenido = malloc(tamanio_pagina);
            memcpy(contenido, stream_memoria + tamanio_pagina * page->frame_id, tamanio_pagina);

            pagina_swap.contenido = contenido;
	        pagina_swap.tamanioContenido = tamanio_pagina;
	        pagina_swap.proceso_id = page->process_id;
	        pagina_swap.page_id = page->page_id;
            
            t_package paquete_a_enviar = ser_lectura_pagina(pagina_swap);
            sendMessage(paquete_a_enviar, conexionSWAMP);
        }
    }
    if(strcmp(get_tipo_asignacion(), "FIJA") == 0){
        //Si se trata de asignación fija, marcar como disponible el arreglo de bitmapReservados
        for(int c=0; c<list_size(reserved_bitmap); c++){
            int value = list_get(reserved_bitmap, c);
            if(value == process_id){
                value = 0;
            }
        }
    }

    /*
        Entrada: página
        1. TLB, página que se emigra a swap, entrada que quitamos de la tlb
    */
    for (int c = 0; c < list_size(entradas_tlb); c++)
    {
        t_entrada_tlb *item = list_get(entradas_tlb, c);
        if(item->process_id == process_id){
            item->process_id = 0;
            item->libre = true;
            item->page_id = 0;
            item->frame_id = 0;
            item->timestamp = 0;
            item->timestamp_entrada = 0;
        }
    }
}