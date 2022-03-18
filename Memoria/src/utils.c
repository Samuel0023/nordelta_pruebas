#include "utils.h"

bool esDireccionLogicaValida(int inicioDL, int contenido){
    int limite_maximo = getcantMarcosSWAMP() * get_tamanio_pagina();
    if(inicioDL+contenido > limite_maximo){
        return false;
    }
    return true;
}

void liberar_table_page(int process_id);

char *get_label_presencia(bool presencia)
{
    char *label = string_new();
    if (presencia)
        string_append_with_format(&label, "%s", "       Libre");
    else
        string_append_with_format(&label, "%s", "       Ocupado");
    //string_trim_left(&label);
    return label;
}

int getFramesToUse(int tamanioBytes)
{
    int TAMANIO_PAGINA = get_tamanio_pagina();
    int framesToUse = (tamanioBytes) / TAMANIO_PAGINA;
    if ((tamanioBytes) % TAMANIO_PAGINA != 0)
    {
        framesToUse++;
    }
    return framesToUse;
}

int getcantMarcosSWAMP()
{
    printf("Conexion: %d\n", conexionSWAMP);
    if (conexionSWAMP == -1)
        return 10;
    int cantMarcosSWAMP = 0;

    peticion_cant_marcos_swamp(conexionSWAMP);
    cantMarcosSWAMP = respuesta_cant_marcos_swamp(conexionSWAMP);

    printf("Cantidad de marcos: %d\n", cantMarcosSWAMP);
    return cantMarcosSWAMP;
}

void transferir_a_swamp(t_page * page)
{
    int pageID = page->page_id;
    int PAGE_SIZE = get_tamanio_pagina();
    t_page_swap pageswamp;

    void *content = malloc(PAGE_SIZE);
    memcpy(content, stream_memoria + (page->frame_id * PAGE_SIZE), PAGE_SIZE);

    pageswamp.content = content;
    pageswamp.contentSize = PAGE_SIZE;
    pageswamp.proceso_id = page->process_id;
    pageswamp.page_id = pageID;

    t_package package = ser_pageswamp(pageswamp);
    sendMessage(package, conexionSWAMP);

    free(content);
}

void print_bit_map(t_bitarray *bitarray)
{
    uint32_t bits = bitarray_get_max_bit(bitarray);
    printf("Bits: %d\n", bits);
    for (int c = 0; c < bits; c++)
    {
        bool bit = bitarray_test_bit(bitarray, c);
        printf("%d ", bit);
    }
    printf("\n");
}
void create_table_page(int process_id){
    printf("Creacion table: %d\n", process_id);
    t_page_table *new_table_page=  malloc(sizeof(t_page_table));
    new_table_page->process_id= process_id;
    new_table_page->pages = list_create();
    new_table_page->amount_tlb_miss = 0;
    new_table_page->amount_tlb_hit = 0;
    

    list_add(pageTables,new_table_page);
}
void liberar_table_page(int process_id){
    bool _is_the_same(t_page_table *table){
        return table->process_id == process_id;
    }
    t_page_table *table = list_find(pageTables,(void*)_is_the_same);
    list_destroy(table->pages);
    free(table);
}
bool check_disp_reserved_bitmap(){
    int cant_disp=0;
    int pos=0;
    while(pos<list_size(reserved_bitmap)){
        if(list_get(reserved_bitmap,pos)==0){
            cant_disp++;
        }
        pos++;
    }
    return cant_disp >= MARCOS_POR_CARPINCHO;
}
void book_reserved_bitmap(int process_id){
    int size = list_size(reserved_bitmap);
    int cant_to_book = MARCOS_POR_CARPINCHO;
    int item,pos;
    item=0;
    pos=0;
    //asignacion fija
   // printf("\n id process %d \n",cant_to_book);
    while(pos<size && cant_to_book>0){
        if((item =list_get(reserved_bitmap,pos)==0)){
          list_replace(reserved_bitmap,pos,process_id);
          //printf("\ncant faltante %d\n",cant_to_book);
          cant_to_book--;
        }
        pos++;
    }
}
t_page_table *getTablaPaginas(int procesoID)
{
    t_page_table *pageTable;
    bool _buscarPorID(void *elemento)
    {
        return (((t_page_table *)elemento)->process_id == procesoID);
    }
    pageTable = (t_page_table *)list_find(pageTables, _buscarPorID);
    return pageTable;
}

t_list *searchPages(int DL, int processID)
{ //[t_page]
    int TAMANIO_PAGINA = get_tamanio_pagina();
    t_page_table *pageTable = getTablaPaginas(processID);
    int start = DL / TAMANIO_PAGINA;
    int end = (DL + 9) / TAMANIO_PAGINA;
    if ((DL + 9) % TAMANIO_PAGINA == 0)
        end--;

    t_list *pages = list_slice(pageTable->pages, start, end + 1);
    return pages;
}

int DLconvertToDF(int DL, int process_id)
{
    int TAMANIO_PAGINA = get_tamanio_pagina();

    int page_id = DL / TAMANIO_PAGINA;
    int offset = DL % TAMANIO_PAGINA;
    t_page* page = getPaginaMMU(page_id, process_id);
    printf("Frame de conversion: %d\n ", page->frame_id);
    int DF = page->frame_id * TAMANIO_PAGINA;
    return DF + offset;
}

t_page getPage(int page_id, int process_id)//ver para eliminar
{
    t_page page;
    page.frame_id = 2; //agregado para probar test
    return page;
}
t_page pointer_page_convert_to_page(t_page *pointer_page)
{
    t_page page;
    page.page_id = pointer_page->page_id;
    page.frame_id = pointer_page->frame_id;
    page.presencia = pointer_page->presencia;
    page.modificado = pointer_page->modificado;
    page.timestamp = pointer_page->timestamp;
    page.process_id = pointer_page->process_id;
    return page;
}
//Devuelve la direccion Logica
bool hayAllocDisponible(int param_alloc_size, int proceso_id)
{
    printf("Line id: %d %d\n", param_alloc_size, proceso_id);
    t_page_table *tablaPaginas = getTablaPaginas(proceso_id);
    printf("Line 2\n");
    int getMarcosPermitidosLibre()
    {

        if (get_tipo_asignacion() == "FIJA")
        {
            return MARCOS_POR_CARPINCHO;
        }

        int marcosLibresMP = 0;
        int marcosTotales = bitarray_get_max_bit(bitmap);
        for (int i = 0; i < marcosTotales; i++)
        {
            if (!bitarray_test_bit(bitmap, i))
            {
                marcosLibresMP++;
            }
        }
        return marcosLibresMP;
    }

    if (list_size(tablaPaginas->pages) == 0)
    {
        printf("Line 3\n");
        int marcosAUsar = getFramesToUse(param_alloc_size + 9 + 9);
        int marcosPermitidosLibre = getMarcosPermitidosLibre();
        if (marcosPermitidosLibre >= marcosAUsar)
        {
            return true;
        }
        return false;
    }
    else
    {
        /*Lista de heapMetaData*/
        int DF = DLconvertToDF(0, proceso_id);

        t_heap_meta_data *heapMetaData = deserializarHeapMetaData(DF);

        int currentDL = heapMetaData->nextAlloc;
        while (heapMetaData->nextAlloc != NULL)
        {
            DF = DLconvertToDF(currentDL, proceso_id);
            heapMetaData = deserializarHeapMetaData(DF);

            int malloc_size = getMallocSize(currentDL, heapMetaData);
            printf(malloc_size); //borrar
            currentDL = 9 + malloc_size;

            if (heapMetaData->isFree)
            {
                if (malloc_size == param_alloc_size)
                {
                    return true;
                }
                if (malloc_size >= (param_alloc_size + 9))
                {
                    return true;
                }
            }
        }
        return false;
    }
}

t_page *find_page_in_pageTable(int page_id, t_list *pages)
{
    bool _es_el_mismo_page_id(t_page * pagina)
    {
        printf("Item buscador: %d\n", pagina->page_id);
        return pagina->page_id == page_id;
    }
    return list_find(pages, (void *)_es_el_mismo_page_id);
}

int get_first_available_bitmap_memoria(int procesoID){
    return get_first_available_bitmap(bitmap, procesoID, reserved_bitmap, get_marcos_por_carpincho(), get_tipo_asignacion());

}