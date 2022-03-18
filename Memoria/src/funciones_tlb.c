#include "funciones_tlb.h"

void generar_dump_tlb()
{

    char *format = "%d/%m/%y %H:%M:%S";

    char *timestampT = temporal_get_string_time(format);

    char *contenido = string_new();
    string_append_with_format(&contenido, "\n-----------------------------------------------------------------------------------------------\n");
    string_append_with_format(&contenido, "Dump: %s\n", timestampT);
    int cant_tablas = list_size(entradas_tlb);

    for (int c = 0; c < cant_tablas; c++)
    {

        t_entrada_tlb *entrada = list_get(entradas_tlb, c);

        if (entrada->libre)
        {
            string_append_with_format(&contenido, "Entrada: %d  Estado: %s  Carpincho: -  Página: -  Marco: -\n", c, get_label_presencia(entrada->libre));
        }
        else
        {
            string_append_with_format(&contenido, "Entrada: %d  Estado: %s  Carpincho: %d  Página: %d  Marco: %d\n", c, get_label_presencia(entrada->libre), entrada->process_id, entrada->page_id, entrada->frame_id);
        }
    }

    string_append_with_format(&contenido, "------------------------------------------------------------------------------------------------\n");
    printf("%s\n", contenido);
    //Mostrar dump de las entradas de la tlb
    free(timestampT);
    timestampT = temporal_get_string_time("");
    char *path = string_new();
    string_append(&path, get_path_dump_tlb());
    string_append_with_format(&path, "/Dump_%s.tlb", timestampT);
    int file_size = strlen(contenido);
    int fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(fd, file_size);
    void *stream = mmap(NULL, file_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    strcpy(stream, contenido);
    free(contenido);
    free(path);
    free(timestampT);
}

void limpiar_entradas_tlb()
{
    for (int c = 0; c < get_cantidad_entradas_tlb(); c++)
    {

        t_entrada_tlb *una_entrada_tlb = list_get(entradas_tlb, c);

        una_entrada_tlb->libre = 1;
        una_entrada_tlb->page_id = -1;
        una_entrada_tlb->timestamp = -1;
        una_entrada_tlb->timestamp_entrada = -1;
        una_entrada_tlb->frame_id = -1;

        //printf("%d-%d-%d-%d-%d\n", una_entrada_tlb->libre, una_entrada_tlb->page_id, una_entrada_tlb->timestamp, una_entrada_tlb->timestamp_entrada, una_entrada_tlb->frame_id);
    }
}

void inicializar_entradas_tlb()
{
    for (int c = 0; c < get_cantidad_entradas_tlb(); c++)
    {

        //printf("Entradas tlb : %d \n", get_cantidad_entradas_tlb());

        t_entrada_tlb *una_entrada_tlb = malloc(sizeof(t_entrada_tlb)); /*  */
        una_entrada_tlb->libre = 1;
        una_entrada_tlb->page_id = -1;
        una_entrada_tlb->frame_id = -1;
        una_entrada_tlb->process_id = -1;
        //printf("%dªfor  del tlb\n", c);
        list_add(entradas_tlb, una_entrada_tlb);
        //free(una_entrada_tlb);
    }
}

t_page* getPaginaMMU(int pageID, int procesoID)
{
    printf("Page: %d ṔID: %d\n", pageID, procesoID);
    t_page* page;
    if (getPaginaTLB(pageID, procesoID) == -1)
    {   
        printf("TLB miss\n");
        //tlb_miss_totales++;
        sleep(get_retardo_fallo_tlb());
        page = getPagina(pageID, procesoID);
        if (page != NULL)
        {
            agregar_entrada_tlb(page);
        }
    }
    else
    {
        printf("TLB hit\n");
        sleep(get_retardo_acierto_tlb());
        t_page_table *tabla = getTablaPaginas(procesoID);
        page = find_page_in_pageTable(pageID, tabla->pages);
    }
    return page;
}

t_entrada_tlb *entrada_fifo_tlb(int *pos_reemplazo)
{

    t_entrada_tlb *entrada_a_reemplazar;
    *pos_reemplazo = -1;
    int pos_entrada = -1;
    bool se_reemplazo = false;
    //encontrar el time_stamp de entrada menor
    t_entrada_tlb *menor_timestamp_entrada(t_entrada_tlb * entrada1, t_entrada_tlb * entrada2)
    {
        pos_entrada++;
        if (entrada1->timestamp_entrada < entrada2->timestamp_entrada)
        {
            if (!se_reemplazo)
            {
                *pos_reemplazo = pos_entrada;
                se_reemplazo = true;
            }

            return entrada1;
        }
        else
        {
            *pos_reemplazo = pos_entrada + 1;
            return entrada2;
        }
    }

    entrada_a_reemplazar = list_get_minimum(entradas_tlb, menor_timestamp_entrada);

    return entrada_a_reemplazar;
}

t_entrada_tlb *entrada_lru_tlb(int *pos_reemplazo)
{

    t_entrada_tlb *entrada_a_reemplazar;
    *pos_reemplazo = -1;
    int pos_entrada = -1;
    bool se_reemplazo = false;
    //encontrar el time_stamp de entrada menor
    t_entrada_tlb *menor_timestamp(t_entrada_tlb * entrada1, t_entrada_tlb * entrada2)
    {
        pos_entrada++;
        if (entrada1->timestamp < entrada2->timestamp)
        {
            if (!se_reemplazo)
            {
                *pos_reemplazo = pos_entrada;
                se_reemplazo = true;
            }
            return entrada1;
        }

        else
        {
            *pos_reemplazo = pos_entrada + 1;
            return entrada2;
        }
    }

    entrada_a_reemplazar = list_get_minimum(entradas_tlb, menor_timestamp);

    return entrada_a_reemplazar;
}

void agregar_entrada_tlb(t_page* una_pagina)
{
    int posicion_entrada = -1;
    bool entrada_libre(t_entrada_tlb * entry)
    {
        posicion_entrada++;
        return entry->libre;
    }

    t_entrada_tlb *una_entrada = list_find(entradas_tlb, entrada_libre);

    void destroyer(t_entrada_tlb * item)
    {
        free(item);
    }

    if (una_entrada == NULL)
    {
        t_entrada_tlb *entrada_a_reemplazar;
        printf("Aplicar algoritmo de reemplazo\n");

        int pos_reemplazo = -1;
        if (get_algoritmo_reemplazo_tlb() == "FIFO")
        {
            entrada_a_reemplazar = entrada_fifo_tlb(&pos_reemplazo);
        }
        else
        {
            printf("Algortimos de reemplazo LRU\n"); //muy similar
            entrada_a_reemplazar = entrada_lru_tlb(&pos_reemplazo);
        }


        entrada_a_reemplazar->page_id = una_pagina->page_id;
        entrada_a_reemplazar->process_id = una_pagina->process_id;
        log_info("Process victima:%d-Pagina:%d-Marco:%d\n", entrada_a_reemplazar->process_id, entrada_a_reemplazar->page_id, entrada_a_reemplazar->frame_id);
        printf("Process victima:%d-Pagina:%d-Marco:%d\n", entrada_a_reemplazar->process_id, entrada_a_reemplazar->page_id, entrada_a_reemplazar->frame_id);
        entrada_a_reemplazar->page_id = una_pagina->page_id;
        entrada_a_reemplazar->process_id = una_pagina->process_id;

        entrada_a_reemplazar->libre = 0;
        entrada_a_reemplazar->frame_id = una_pagina->frame_id;
        entrada_a_reemplazar->timestamp = timestamp;
        entrada_a_reemplazar->timestamp_entrada = timestamp;
        log_info("Process entrante:%d-Pagina:%d-Marco:%d\n", entrada_a_reemplazar->process_id, entrada_a_reemplazar->page_id, entrada_a_reemplazar->frame_id);
        printf("Process entrante:%d-Pagina:%d-Marco:%d\n", entrada_a_reemplazar->process_id, entrada_a_reemplazar->page_id, entrada_a_reemplazar->frame_id);
        //reemplazar esta entrada
        list_replace_and_destroy_element(entradas_tlb, pos_reemplazo, entrada_a_reemplazar, destroyer);
        //list_replace(entradas_tlb, pos_reemplazo, entrada_a_reemplazar);
    }

    else
    {
        una_entrada->page_id = una_pagina->page_id;
        una_entrada->process_id = una_pagina->process_id;
        una_entrada->libre = 0;
        una_entrada->frame_id = una_pagina->frame_id;
        una_entrada->timestamp = timestamp;
        una_entrada->timestamp_entrada = timestamp;

        //reemplazar esta entrada

        list_replace_and_destroy_element(entradas_tlb, posicion_entrada, una_entrada, destroyer);
        //list_replace(entradas_tlb, posicion_entrada, una_entrada);
    }
}
void mostrar_tlb_hit_miss_por_proceso()
{
    //tendra leak , necesito un malloc ??

    for (int i = 0; i < list_size(pageTables); i++)
    {
        t_page_table *una_tabla_de_paginas = list_get(pageTables, i);
        log_info("Carpincho %d-Hits:%d-Miss:%d\n", una_tabla_de_paginas->process_id, una_tabla_de_paginas->amount_tlb_hit, una_tabla_de_paginas->amount_tlb_miss);
        printf("Carpincho %d-Hits:%d-Miss:%d\n", una_tabla_de_paginas->process_id, una_tabla_de_paginas->amount_tlb_hit, una_tabla_de_paginas->amount_tlb_miss);
    }
}

void mostrar_tlb_hit_miss_globales()
{

    log_info("Hit totales:%d\n", tlb_hit_totales);

    log_info("Miss totales:%d\n", tlb_miss_totales);
    printf("\nHit totales %d\n", tlb_hit_totales);
    printf("Miss totales %d\n", tlb_miss_totales);
}

void sumar_tlb_hit(int procesoID)
{
    if (list_size(pageTables) == 0)
    {
        printf("\nCuando se agregue valores a pageTAbles borrar\n");
        return;
    }
    t_page_table *una_tabla_de_paginas = getTablaPaginas(procesoID);
    tlb_hit_totales++;

    una_tabla_de_paginas->amount_tlb_hit++;

    //list_find me devuelve el contenido
    return;
}
void sumar_tlb_miss(int procesoID)
{
    if (list_size(pageTables) == 0)
    {
        printf("\nCuando se agregue valores a pageTAbles borrar\n");
        return;
    }
    t_page_table *una_tabla_de_paginas = getTablaPaginas(procesoID);
    tlb_miss_totales++;
    una_tabla_de_paginas->amount_tlb_miss++;
    return;
}

int getPaginaTLB(int pageID, int procesoID)
{
    int marcoID = -1;
    int posicion_entrada = -1;
    bool misma_pagina_mismo_proceso(t_entrada_tlb * entry)
    {
        posicion_entrada++;
        return (entry->process_id == procesoID) && (entry->page_id == pageID);
    }

    t_entrada_tlb *una_entrada = list_find(entradas_tlb, misma_pagina_mismo_proceso);
    if (una_entrada == NULL)
    {
        sumar_tlb_miss(procesoID);

        log_info("TLB Miss-Process:%d-Pagina:%d\n", procesoID, pageID);
        printf("TLB-Miss-Process:%d-Pagina:%d-Marco: -1\n", procesoID, pageID);
    }
    else
    {
        log_info("TLB Hit-Process:%d-Pagina:%d-Marco:%d\n", procesoID, pageID, una_entrada->frame_id);
        printf("TLB-Hit-Process:%d-Pagina:%d-Marco:%d\n", procesoID, pageID, una_entrada->frame_id);
        sumar_tlb_hit(procesoID);
        pthread_mutex_lock(&MX_TIMESTAMP);
        timestamp++; //agregarlo en la entrada

        una_entrada->timestamp = timestamp;
        pthread_mutex_unlock(&MX_TIMESTAMP);

        void destroyer(t_entrada_tlb * item)
        {
            free(item);
        }
        list_replace_and_destroy_element(entradas_tlb, posicion_entrada, una_entrada, destroyer);
        //list_replace(entradas_tlb, posicion_entrada, una_entrada);
        marcoID = una_entrada->frame_id;
    }
    return marcoID;
}
