#include "asignacion_memoria.h"

int asignMemory(int alloc_size, int process_id){
    t_page_table* pageTable = getTablaPaginas(process_id);
    int FRAMES_PER_PROCESS = get_marcos_por_carpincho();
    char* ASSIGNATION_TYPE = get_tipo_asignacion();
    int PAGE_SIZE = get_tamanio_pagina();

    int amountPages = list_size(pageTable->pages);
    if(amountPages == 0){	
        case_with_empty_pages(alloc_size, process_id, FRAMES_PER_PROCESS, ASSIGNATION_TYPE,
            pageTable, PAGE_SIZE);
    }else{
        case_with_not_empty_pages(alloc_size, process_id, FRAMES_PER_PROCESS, ASSIGNATION_TYPE,
            pageTable, PAGE_SIZE);
    }
    return 0;        
}

void case_with_empty_pages(
    int alloc_size, int process_id, int FRAMES_PER_PROCESS, char* ASSIGNATION_TYPE,
    t_page_table* pageTable, int PAGE_SIZE 
){
    int amountPages = list_size(pageTable->pages);
    int frames_to_use =  getFramesToUse(alloc_size + 9 + 9);
    t_list* frames = list_create();                     //[int]

    for(int c=0; c<frames_to_use; c++){
        int frame_id = get_first_available_bitmap(
            bitmap, process_id, reserved_bitmap, 
            FRAMES_PER_PROCESS, ASSIGNATION_TYPE
        );

        list_add(frames, frame_id);
        
        t_page* page = malloc(sizeof(t_page));
        page->page_id = c;
        page->frame_id = frame_id;
        page->presencia = true;
        page->modificado = false;
        page->timestamp = memory_timestamp;
        page->process_id = process_id;
        list_add(pageTable->pages, page);

        memory_timestamp++;
    }

    int firstFrame = list_get(frames, 0);
    int start_next_hmd = firstFrame*PAGE_SIZE + 9+alloc_size;

    t_heap_meta_data hmd = crearHeapMetaData(-1, 0+9+alloc_size, false);
    guardarHMD(hmd, firstFrame*PAGE_SIZE);
    t_heap_meta_data next_hmd = crearHeapMetaData(0, -1, true);
    guardarHMD(next_hmd, start_next_hmd);


    //void transferir_a_swamp(t_page page);
    for(int c=0;c<amountPages;c++){
        t_page* page = list_get(pageTable->pages, c);
        t_page* page_without_pointer = pointer_page_convert_to_page(page);
        transferir_a_swamp(page_without_pointer);
    }
}

void case_with_not_empty_pages(
    int alloc_size, int process_id, int FRAMES_PER_PROCESS, char* ASSIGNATION_TYPE,
    t_page_table* pageTable, int PAGE_SIZE
){
    int DL = -1; 	
    int offset = 0;
    t_heap_meta_data heap_meta_data;
    do{
        int DF = DLconvertToDF(DL, process_id);
        heap_meta_data = deserializarHeapMetaData(DF);
        int malloc_size = getMallocSize(DL, heap_meta_data);
        
        if(heap_meta_data.isFree){
            if(malloc_size == alloc_size){
                heap_meta_data.isFree = false;
                t_list* pages = searchPages(DL, process_id);         //[t_page] 	
                for(int c=0;c<list_size(pages);c++){
                    t_page* page = list_get(pages, c);
                    page->modificado = true;
                }
                DL = DL +9;
                break;
            }
            if(malloc_size >= (alloc_size + 9)){
                heap_meta_data.isFree = false;
                t_list* pages = searchPages(DL, process_id);         //[t_page] 	
                for(int c=0;c<list_size(pages);c++){
                    t_page* page = list_get(pages, c);
                    page->modificado = true;
                }

                //Se crea el HMD
                int DF = DLconvertToDF(heap_meta_data.nextAlloc, process_id);
                t_heap_meta_data nextHeapMetaData = deserializarHeapMetaData(DF);
                nextHeapMetaData.prevAlloc = nextHeapMetaData.prevAlloc + alloc_size + 9;
                int nextAlloc = NULL;
                /*if(nextHeapMetaData != NULL){
                    nextAlloc = nextHeapMetaData.nextAlloc;
                }*/
                heap_meta_data.nextAlloc = DL + 9 + alloc_size; 

                t_heap_meta_data newHeapMetaData = crearHeapMetaData(DL, nextAlloc, true);

                DF = DLconvertToDF(DL + 9 + alloc_size, process_id);
                guardarHMD(newHeapMetaData, DF); 
                DL = DL +9;
                break;
            }
        }
        DL = heap_meta_data.nextAlloc;
    }while(heap_meta_data.nextAlloc != NULL);
    return DL;
}