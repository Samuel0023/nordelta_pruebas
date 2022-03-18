#include "get_pagina.h"

t_page * buscarMarcoAReemplazar(int procesoid);

t_page *getPagina(int pageID, int procesoID)
{
    t_page_table *tabla = getTablaPaginas(procesoID);
    printf("Table de proceso id: %d\n", tabla->process_id);
    printf("Page id: %d Size: %d\n", pageID, list_size(tabla->pages));
    t_page *pagina = find_page_in_pageTable(pageID, tabla->pages);

    if (pagina == NULL)
        return NULL;

    if (pagina->presencia)
        pagina->uso = true;

    else
    {
        int frameid = getFrameLibre(procesoID);
        //Bitmap: marcarlo como libre--> ??
        //MATI: es correcto eso? o hay que marcarlo como ocupado? si es como ocupado, ya se ocupa la funcion getframelibre

        void *contenido = getPageSwamp(procesoID, pageID);
        //Guardar el contenido en la memoria principal
        int desplazamiento = frameid * get_tamanio_pagina();
        memcpy(stream_memoria + desplazamiento, &contenido, get_tamanio_pagina());

        pagina->presencia = true;
        pagina->uso = true;
        pagina->frame_id = frameid;
    }
    return pagina;
}

int getFrameLibre(int procesoid)
{
    //Dinámica-fija
    int frameid = get_first_available_bitmap_memoria(procesoid); //En este método, si no encuentra ninguno devolver un numero negativo-- SE ENCARGA LA FUNCION EN SI DE ESO? 
    if (frameid < 0)
    {
        //No hay memoria, ejecutar algoritmo de reemplazo
        t_page * page = buscarMarcoAReemplazar(procesoid);
        //Transferencia a swapping si es necesario
        if (page->modificado == true)
            transferir_a_swamp(page);
        frameid = page->frame_id;
    }
    return frameid;
}

t_list *getPaginasEnMemoria(int procesoid) //[t_page]
{
    t_list *paginasCandidatas = list_create();
    if (string_equals_ignore_case(get_tipo_asignacion(), "FIJA"))
    {
        paginasCandidatas = get_paginas_id_memoria(procesoid); //TODO:Hallar la tabla de páginas a partir del proceso id y filtrar las páginas que se encuentran en memoria
    }
    else
    {
        paginasCandidatas = get_all_paginas_memoria(); //Todas las páginas de todas las tablas que se encuentren en memoria
    }
    return paginasCandidatas;
}

void *getPageSwamp(int procesoId, int pageId)
{
    PageSwap pageswamp;
    pageswamp.contenido = NULL;
    pageswamp.tamanioContenido = 1;
    pageswamp.proceso_id = procesoId;
    pageswamp.page_id = pageId;

    t_package paquete = ser_lectura_pagina_swamp(pageswamp);
    sendMessage(paquete, conexionSWAMP);
    t_package respuesta_pagina = recibir_mensaje(conexionSWAMP);
    return des_respuesta_lectura_pagina(respuesta_pagina).contenido;
}

t_list *get_paginas_id_memoria(int procesoid)
{
    t_page_table *tabla = getTablaPaginas(procesoid);//malloc(sizeof(t_page_table));
    //tabla = getTablaPaginas(procesoid);

    bool _esta_en_memoria(t_page * pagina)
    {
        return pagina->presencia;
    }

    return list_filter(tabla->pages, (void *)_esta_en_memoria);
}

t_list *get_all_paginas_memoria()
{
    t_list *lista_tabla_paginas = list_create();

    void _agregar_a_lista_presencia(t_page_table * page_table)
    {
        bool _esta_en_memoria(t_page * pagina)
        {
            return pagina->presencia;
        }
        t_list * lista_aux = list_filter(page_table->pages,(void *) _esta_en_memoria);
        list_add_all(lista_tabla_paginas, lista_aux);
        list_destroy(lista_aux);
    }
    list_iterate(pageTables,(void*) _agregar_a_lista_presencia);

    return lista_tabla_paginas;
}

t_page * buscarMarcoAReemplazar(int procesoid){
    t_page * page_remplazar;
    if (strcmp(get_algoritmo_reemplazo_mmu(), "LRU") == 0)
    {
        page_remplazar = busqueda_por_LRU(procesoid);
    }
    else
    {
        page_remplazar =busquedaPorClockModificado(procesoid);
    }
    
    return page_remplazar;
}