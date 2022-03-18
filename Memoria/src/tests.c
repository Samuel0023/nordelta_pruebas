#include "../include/tests.h"

int run_tests()
{
    CU_initialize_registry();
    CU_pSuite tests = CU_add_suite("Memoria Suite", NULL, NULL);

    CU_add_test(tests, "Verificar marcos a usar: ", verificar_marcos_a_usar);
    CU_add_test(tests, "Verificar bitmap disponibilidad: ", verificar_bitmap_disponible);
    CU_add_test(tests, "Ser. y des. de pageswamp: ", ser_des_pageswamp);
    CU_add_test(tests, "Ser. y des. de heapmetada: ", ser_des_heapmetadata);
    CU_add_test(tests, "Verificar getMallocSize: ", verificar_malloc_size);
    CU_add_test(tests, "Verificar searchpages: ", verificar_search_pages);
    CU_add_test(tests, "Verificar transferencia swamp", verificar_transferencia_swap);
    CU_add_test(tests, "Verificar cantidad marcos swamp", verificar_get_cant_marcos_swap);
    CU_add_test(tests, "Verificar getTablaPaginas", verificar_getTablaPaginas);
    //CU_add_test(tests, "Verificar direccion logica convertida a fisica", verificar_DLconvertToDF);
    CU_add_test(tests, "Verificar siguiente heap metadata", verificar_getNextHeapMetaData);
    CU_add_test(tests, "Verificar hayAllocDisponible con tabla de pagina de proceso vacia devuelve 9", verificar_hayAllocDisponible_con_tablaPaginaProceso_vacia);
    CU_add_test(tests, "Verificar find_page_in_pageTable", validar_find_page_in_pageTable);
    CU_add_test(tests, "Verificar get_paginas de un pid con presencia en memoria", validar_get_paginas_id_memoria);
    CU_add_test(tests, "Verificar get_all_paginas_memoria trae las 4 paginas que estan en memoria", validar_get_all_paginas_memoria);
    CU_add_test(tests, "Verificamos el funcionamiento de mate_init y si agrega la tabla de paginas", verificar_mem_mate_init);
    CU_add_test(tests, "Verificar inicializar entradas_tlb", verificar_inicializar_entradas_tlb);
    CU_add_test(tests, "Verificar ALGORITMO de MMU", validar_algoritmo_MMU);

    CU_add_test(tests, "Verificar algoritmos_tlb", verificar_algoritmos_tlb_Y_getPaginaTLB);
    CU_add_test(tests, "Verificar suma_tlb_hit_miss", verificar_suma_tlb_hit_miss);
    //CU_add_test(tests, "Verificar limpieza entradas tlb", verificar_limpiar_entradas_tlb);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void verificar_marcos_a_usar()
{
    CU_ASSERT_EQUAL(getFramesToUse(30), 1);
    CU_ASSERT_EQUAL(getFramesToUse(40), 2);
    CU_ASSERT_EQUAL(getFramesToUse(get_tamanio_pagina()), 1);
}

void verificar_bitmap_disponible()
{
    bitmap = initialize_bitmap(get_tamanio() / get_tamanio_pagina());
    bitarray_set_bit(bitmap, 0);
    bitarray_set_bit(bitmap, 1);

    reserved_bitmap = list_create();
    CU_ASSERT_EQUAL(get_first_available_bitmap(bitmap, 1, reserved_bitmap, 4, "FIJA"), 2);

    //Testeo el metodo creado en utils para reducir parametros
    bitarray_clean_bit(bitmap, 2);
    CU_ASSERT_EQUAL(get_first_available_bitmap_memoria(1), 2);

    list_destroy(reserved_bitmap);
    free(bitmap->bitarray);
    bitarray_destroy(bitmap);
}

void ser_des_pageswamp()
{
    int PAGE_SIZE = get_tamanio_pagina();
    t_page_swap pageswamp;

    void *content = malloc(10);
    pageswamp.content = content;
    pageswamp.contentSize = PAGE_SIZE;
    pageswamp.proceso_id = 3;
    pageswamp.page_id = 2;
    t_package package = ser_pageswamp(pageswamp);

    t_page_swap pageswamp_otro = des_pageswamp(package);
    CU_ASSERT_EQUAL(pageswamp.contentSize, pageswamp_otro.contentSize);
    CU_ASSERT_EQUAL(pageswamp.proceso_id, pageswamp_otro.proceso_id);
    CU_ASSERT_EQUAL(pageswamp.page_id, pageswamp_otro.page_id);

    free(package.buffer);
    free(pageswamp.content);
    free(pageswamp_otro.content);
}

void ser_des_heapmetadata()
{
    stream_memoria = malloc(100);
    t_heap_meta_data hmd = crearHeapMetaData(1, 10, false);
    guardarHMD(hmd, 0);
    t_heap_meta_data other_hmd = deserializarHeapMetaData(0);

    CU_ASSERT_EQUAL(hmd.prevAlloc, other_hmd.prevAlloc);
    CU_ASSERT_EQUAL(hmd.nextAlloc, other_hmd.nextAlloc);
    CU_ASSERT_EQUAL(hmd.isFree, other_hmd.isFree);

    free(stream_memoria);
}

void verificar_malloc_size()
{
    conexionSWAMP = -1;
    stream_memoria = malloc(100);
    t_heap_meta_data hmd = crearHeapMetaData(NULL, 15, false);
    guardarHMD(hmd, 0);
    t_heap_meta_data hmd2 = crearHeapMetaData(15, NULL, false);
    guardarHMD(hmd2, 15);

    int size = getMallocSize(0, hmd);
    int other_size = getMallocSize(15, hmd2);

    CU_ASSERT_EQUAL(size, 6);
    CU_ASSERT_EQUAL(other_size, 296);

    free(stream_memoria);
}

void verificar_search_pages()
{
    int BYTES_CONFIG = get_tamanio();

    pageTables = list_create();
    stream_memoria = malloc(BYTES_CONFIG);

    t_page_table tableA;
    tableA.process_id = 1;
    tableA.pages = list_create();

    t_page_table tableB;
    tableB.process_id = 2;
    tableB.pages = list_create();

    t_page page;
    page.page_id = 1;
    page.frame_id = 1;
    page.presencia = true;
    page.modificado = false;
    page.timestamp = 1;
    list_add(tableB.pages, &page);

    list_add(pageTables, &tableA);
    list_add(pageTables, &tableB);

    t_list *pagesB = searchPages(3, 2);
    t_page *pageOfList = list_get(pagesB, 0);
    CU_ASSERT_EQUAL(list_size(pagesB), 1);
    CU_ASSERT_EQUAL(pageOfList->page_id, 1);

    free(stream_memoria);
    list_destroy(tableA.pages);
    list_destroy(tableB.pages);
    list_destroy(pagesB);
    list_destroy(pageTables);
}

void verificar_transferencia_swap()
{
    //Funciona con swamp encendido
    int BYTES_CONFIG = get_tamanio();
    char *IP_SWAMP = get_ip_swamp();
    char *PUERTO_SWAMP = get_puerto_swamp();

    stream_memoria = malloc(BYTES_CONFIG);
    conexionSWAMP = crear_conexion(IP_SWAMP, PUERTO_SWAMP);

    t_page page;
    page.page_id = 1;
    page.frame_id = 1;
    page.presencia = true;
    page.modificado = false;
    page.timestamp = 1;

    //transferir_a_swamp(page);

    free(stream_memoria);
}

//Esperar implementacion en swamp
void verificar_get_cant_marcos_swap()
{
    int cantMarcosSwamp = getcantMarcosSWAMP();

    CU_ASSERT_EQUAL(cantMarcosSwamp, 10);
}

void verificar_getTablaPaginas()
{
    t_page_table *pageTable;
    int procesoIdABuscar = 2;
    pageTables = list_create();
    int BYTES_CONFIG = get_tamanio();
    stream_memoria = malloc(BYTES_CONFIG);

    t_page_table tableA;
    tableA.process_id = 1;
    tableA.pages = list_create();

    t_page_table tableB;
    tableB.process_id = 2;
    tableB.pages = list_create();

    t_page_table tableC;
    tableC.process_id = 3;
    tableC.pages = list_create();

    list_add(pageTables, &tableA);
    list_add(pageTables, &tableB);
    list_add(pageTables, &tableC);

    pageTable = getTablaPaginas(procesoIdABuscar);

    CU_ASSERT_EQUAL(pageTable->process_id, 2);

    free(stream_memoria);
    list_destroy(tableA.pages);
    list_destroy(tableB.pages);
    list_destroy(tableC.pages);

    // free(pageTable);
    list_destroy(pageTables);
}

void verificar_DLconvertToDF()
{
    int resultadoDF;
    int idProceso = 5;
    int dl = 32;

    resultadoDF = DLconvertToDF(dl, idProceso); //harcodeado valor de marco = 2 => BAse DF = 64
    printf("dl %d", resultadoDF);

    CU_ASSERT_EQUAL(resultadoDF, 64);
}

void verificar_getNextHeapMetaData()
{
    t_heap_meta_data *resultadoHMD;

    stream_memoria = malloc(100);
    t_heap_meta_data hmd1 = crearHeapMetaData(NULL, 10, false);
    guardarHMD(hmd1, 0);
    t_heap_meta_data hmd2 = crearHeapMetaData(0, 25, true);
    guardarHMD(hmd2, 10);
    t_heap_meta_data hmd3 = crearHeapMetaData(10, NULL, false);
    guardarHMD(hmd3, 25);
    resultadoHMD = getNextHeapMetaData(0);

    CU_ASSERT_TRUE(resultadoHMD->isFree);
    CU_ASSERT_EQUAL(resultadoHMD->nextAlloc, 25);

    free(stream_memoria);
    free(resultadoHMD);
}

void verificar_hayAllocDisponible_con_tablaPaginaProceso_vacia()
{

    pageTables = list_create();
    conexionSWAMP = -1;
    MARCOS_POR_CARPINCHO = 5;
    bitmap = initialize_bitmap(10);

    int DLAllocDisponibleTablaVacia, DLAllocDisponible;
    t_page_table tableA;
    tableA.process_id = 1;
    tableA.pages = list_create();

    t_page_table tableB;
    tableB.process_id = 2;
    tableB.pages = list_create();

    t_page_table tableC;
    tableC.process_id = 3;
    tableC.pages = list_create();

    t_page page;
    page.page_id = 0;
    page.frame_id = 0;
    page.presencia = true;
    page.modificado = false;
    page.timestamp = 1;
    list_add(tableB.pages, &page);

    list_add(pageTables, &tableA);
    list_add(pageTables, &tableB);
    list_add(pageTables, &tableC);

    stream_memoria = malloc(100);
    t_heap_meta_data hmd1 = crearHeapMetaData(NULL, 10, false);
    guardarHMD(hmd1, 0);
    t_heap_meta_data hmd2 = crearHeapMetaData(0, 25, true);
    guardarHMD(hmd2, 10);
    t_heap_meta_data hmd3 = crearHeapMetaData(10, NULL, true);
    guardarHMD(hmd3, 25);

    DLAllocDisponibleTablaVacia = hayAllocDisponible(35, 3);
    // DLAllocDisponible = hayAllocDisponible(15,2);
    // printf("\nvalor dl alloc disponible: %d\n", DLAllocDisponible);
    printf("Logica: %d\n", DLAllocDisponibleTablaVacia);
    CU_ASSERT_EQUAL(DLAllocDisponibleTablaVacia, 1); //al ser el primer alloc devuelve dl despues del HMD
                                                     // CU_ASSERT_EQUAL(DLAllocDisponible,25);  ---Tira segmentation fault, chequear si no es problema de getMAllocSize-----
    list_destroy(tableA.pages);
    list_destroy(tableB.pages);
    list_destroy(tableC.pages);
    list_destroy(pageTables);
}

void validar_find_page_in_pageTable()
{
    t_page_table tableA;
    tableA.process_id = 1;
    tableA.pages = list_create();

    t_page page1, page2, page3;
    page1.page_id = 0;
    page1.frame_id = 0;
    page1.presencia = true;
    page1.modificado = false;
    page1.timestamp = 1;
    page2.page_id = 1;
    page2.frame_id = 10;
    page2.presencia = true;
    page2.modificado = false;
    page2.timestamp = 1;
    page3.page_id = 2;
    page3.frame_id = 20;
    page3.presencia = true;
    page3.modificado = false;
    page3.timestamp = 1;

    list_add(tableA.pages, &page1);
    list_add(tableA.pages, &page2);
    list_add(tableA.pages, &page3);

    t_page *resultado = find_page_in_pageTable(2, tableA.pages);

    CU_ASSERT_EQUAL(resultado->page_id, 2);
    CU_ASSERT_EQUAL(resultado->frame_id, 20);

    resultado = find_page_in_pageTable(4, tableA.pages);

    CU_ASSERT_EQUAL(resultado, NULL);

    list_destroy(tableA.pages);
    free(resultado);
}

void validar_get_paginas_id_memoria()
{
    pageTables = list_create();
    t_page_table tableA;
    tableA.process_id = 1;
    tableA.pages = list_create();

    t_page_table tableB;
    tableB.process_id = 2;
    tableB.pages = list_create();

    t_page_table tableC;
    tableC.process_id = 3;
    tableC.pages = list_create();

    list_add(pageTables, &tableA);
    list_add(pageTables, &tableB);
    list_add(pageTables, &tableC);

    t_page page1, page2, page3;
    page1.page_id = 0;
    page1.frame_id = 0;
    page1.presencia = true;
    page1.modificado = false;
    page1.timestamp = 1;
    page2.page_id = 1;
    page2.frame_id = 10;
    page2.presencia = false;
    page2.modificado = false;
    page2.timestamp = 1;
    page3.page_id = 2;
    page3.frame_id = 20;
    page3.presencia = true;
    page3.modificado = false;
    page3.timestamp = 1;

    list_add(tableA.pages, &page1);
    list_add(tableA.pages, &page2);
    list_add(tableA.pages, &page3);

    t_list *paginasEnMemoria = get_paginas_id_memoria(1);
    CU_ASSERT_EQUAL(list_size(paginasEnMemoria), 2);
    t_page *respuesta_pag1 = (t_page *)list_get(paginasEnMemoria, 0);
    t_page *respuesta_pag2 = (t_page *)list_get(paginasEnMemoria, 1);

    CU_ASSERT_EQUAL(respuesta_pag1->page_id, 0);
    CU_ASSERT_EQUAL(respuesta_pag2->page_id, 2);

    list_destroy(tableA.pages);
    list_destroy(tableB.pages);
    list_destroy(tableC.pages);
    list_destroy(pageTables);
    list_destroy(paginasEnMemoria);
}

void validar_get_all_paginas_memoria()
{
    pageTables = list_create();
    t_page_table tableA;
    tableA.process_id = 1;
    tableA.pages = list_create();

    t_page_table tableB;
    tableB.process_id = 2;
    tableB.pages = list_create();

    t_page_table tableC;
    tableC.process_id = 3;
    tableC.pages = list_create();

    list_add(pageTables, &tableA);
    list_add(pageTables, &tableB);
    list_add(pageTables, &tableC);
    t_page *page1 = malloc(sizeof(t_page));
    t_page *page2 = malloc(sizeof(t_page));
    t_page *page3 = malloc(sizeof(t_page));
    //t_page *page1, *page2, *page3;
    page1->page_id = 0;
    page1->frame_id = 0;
    page1->presencia = false;
    page1->modificado = false;
    page1->timestamp = 1;
    page2->page_id = 1;
    page2->frame_id = 10;
    page2->presencia = true;
    page2->modificado = false;
    page2->timestamp = 1;
    page3->page_id = 2;
    page3->frame_id = 20;
    page3->presencia = true;
    page3->modificado = false;
    page3->timestamp = 1;

    list_add(tableA.pages, page1);
    //list_add(tableA.pages, page2);
    //list_add(tableB.pages, page3);
    list_add(tableC.pages, page2);
    list_add(tableC.pages, page3);

    t_list *paginas_en_memoria = get_all_paginas_memoria();

    CU_ASSERT_EQUAL(list_size(paginas_en_memoria), 2);

    list_destroy(tableA.pages);
    list_destroy(tableB.pages);
    list_destroy(tableC.pages);

    free(stream_memoria);
    free(page1);
    free(page2);
    free(page3);
    list_destroy(paginas_en_memoria);
    list_destroy(pageTables);
    free(bitmap->bitarray);
    bitarray_destroy(bitmap);
}
void verificar_mem_mate_init()
{
    reserved_bitmap = list_create();
    list_add(reserved_bitmap, 0);
    list_add(reserved_bitmap, 0);
    list_add(reserved_bitmap, 0);
    list_add(reserved_bitmap, 0);
    list_add(reserved_bitmap, 0);
    list_add(reserved_bitmap, 0);
    list_add(reserved_bitmap, 0);
    pageTables = list_create();
    int process_id = 3;
    mem_mate_init(process_id, 12);
    CU_ASSERT_EQUAL(1, list_size(pageTables));
    CU_ASSERT_EQUAL(process_id, list_get(reserved_bitmap, 0));
    CU_ASSERT_EQUAL(process_id, list_get(reserved_bitmap, 1));
    CU_ASSERT_EQUAL(process_id, list_get(reserved_bitmap, 2));
    CU_ASSERT_EQUAL(process_id, list_get(reserved_bitmap, 3));
    liberar_table_page(process_id);
    list_destroy(reserved_bitmap);
    list_destroy(pageTables);
}
void verificar_inicializar_entradas_tlb()
{

    inicializacion_estructuras_memoria();

    //inicializar_entradas_tlb();
    inicializacion();

    //printf("entradas:tlb:%d\n", list_size(entradas_tlb));
    //printf("entradas:tlb en funcion:%d\n", get_cantidad_entradas_tlb()); //tiene basura
    CU_ASSERT_EQUAL(list_size(entradas_tlb), get_cantidad_entradas_tlb());

    bool entrada_tlb_libre(t_entrada_tlb * una_entradax)
    {
        return una_entradax->libre;
    }

    CU_ASSERT_EQUAL(list_all_satisfy(entradas_tlb, entrada_tlb_libre), 1);
    CU_ASSERT_EQUAL(list_count_satisfying(entradas_tlb, entrada_tlb_libre), 4);

    for (int c = 0; c < list_size(entradas_tlb); c++)
    {
        t_entrada_tlb *una_entrada_tlb = list_get(entradas_tlb, c);
        free(una_entrada_tlb);
    }

    list_destroy(reserved_bitmap);
    list_destroy(entradas_tlb);
    // free(stream_memoria);

    list_destroy(pageTables);
    free(bitmap->bitarray);
    bitarray_destroy(bitmap);
}

void verificar_limpiar_entradas_tlb()
{
    void destroyer(t_entrada_tlb * item)
    {
        free(item);
    }

    inicializacion_estructuras_memoria();
    inicializacion();

    t_entrada_tlb *una_entrada = malloc(sizeof(t_entrada_tlb));
    // no se porque no me guarda correctamente la entrada en la lista
    //guarda exactamente lo mismo que la entrada1
    // pero con vexecc si lo hace
    una_entrada->process_id = 0;
    una_entrada->libre = 0;
    printf("comentario|\n");
    una_entrada->page_id = 1;
    una_entrada->timestamp_entrada = 2;
    una_entrada->timestamp = 4;
    una_entrada->frame_id = 1;

    list_replace_and_destroy_element(entradas_tlb, 0, una_entrada, destroyer);

    //ENTRADA2 de la tlb
    t_entrada_tlb *una_entrada1 = malloc(sizeof(t_entrada_tlb));

    una_entrada1->process_id = 4;
    una_entrada1->libre = 0;

    una_entrada1->page_id = 5;
    una_entrada1->timestamp_entrada = 3;
    una_entrada1->timestamp = 1;
    una_entrada1->frame_id = 2;

    list_replace_and_destroy_element(entradas_tlb, 1, una_entrada1, destroyer);

    //ENTRADA3 de la tlb
    t_entrada_tlb *una_entrada2 = malloc(sizeof(t_entrada_tlb));
    una_entrada2->process_id = 0;
    una_entrada2->libre = 0;
    una_entrada2->page_id = 3;
    una_entrada2->timestamp_entrada = 20;
    una_entrada2->timestamp = 2;
    una_entrada2->frame_id = 3;

    list_replace_and_destroy_element(entradas_tlb, 2, una_entrada2, destroyer);

    //ENTRADA3 de la tlb
    t_entrada_tlb *una_entrada3 = malloc(sizeof(t_entrada_tlb));
    una_entrada3->process_id = 0;
    una_entrada3->libre = 0;
    una_entrada3->page_id = 4;
    una_entrada3->frame_id = 27;
    una_entrada3->timestamp_entrada = 78;
    una_entrada3->timestamp = 2;
    una_entrada3->frame_id = 4;

    list_replace_and_destroy_element(entradas_tlb, 3, una_entrada3, destroyer);
    
    generar_dump_tlb_duplicado();
    printf("h\n");
    
    limpiar_entradas_tlb();
    printf("rompe la 2da vez al entrar la funcion generar_dump....\n");
    generar_dump_tlb_duplicado();

    printf("h\n");

    bool entrada_tlb_libre(t_entrada_tlb * una_entradaz)
    {
        return una_entradaz->libre;
    }
    //CU_ASSERT_EQUAL(list_size(entradas_tlb), 4);
    //CU_ASSERT_EQUAL(list_all_satisfy(entradas_tlb, entrada_tlb_libre), 1);
    printf("h final\n");
    //list_all_satisfy(entradas_tlb, entrada_tlb_libre), 1);
    // free(una_entrada);
    // free(una_entrada1);
    // free(una_entrada2);
    // free(una_entrada3);
}

void verificar_algoritmos_tlb_Y_getPaginaTLB()
{

    inicializacion_estructuras_memoria();
    inicializacion();

    void destroyer(t_entrada_tlb * item)
    {
        free(item);
    }
    //funciona para 3 entradas en la tlb

    //ENTRADA1 de la tlb
    t_entrada_tlb *una_entrada = malloc(sizeof(t_entrada_tlb));
    una_entrada->process_id = 0;
    una_entrada->libre = 0;
    una_entrada->page_id = 1;
    una_entrada->timestamp_entrada = 2;
    una_entrada->timestamp = 0;

    list_replace_and_destroy_element(entradas_tlb, 0, una_entrada, destroyer);

    //ENTRADA2 de la tlb
    t_entrada_tlb *una_entrada1 = malloc(sizeof(t_entrada_tlb));
    una_entrada1->process_id = 0;
    una_entrada1->libre = 0;
    una_entrada1->page_id = 2;
    una_entrada1->timestamp_entrada = 3;
    una_entrada1->timestamp = 1;

    list_replace_and_destroy_element(entradas_tlb, 1, una_entrada1, destroyer);

    //ENTRADA3 de la tlb
    t_entrada_tlb *una_entrada2 = malloc(sizeof(t_entrada_tlb));
    una_entrada2->process_id = 0;
    una_entrada2->libre = 0;
    una_entrada2->page_id = 3;
    una_entrada2->timestamp_entrada = 20;
    una_entrada2->timestamp = 2;

    list_replace_and_destroy_element(entradas_tlb, 2, una_entrada2, destroyer);

    //ENTRADA3 de la tlb
    t_entrada_tlb *una_entrada3 = malloc(sizeof(t_entrada_tlb));
    una_entrada3->process_id = 0;
    una_entrada3->libre = 0;
    una_entrada3->page_id = 4;
    una_entrada3->frame_id = 27;
    una_entrada3->timestamp_entrada = 78;
    una_entrada3->timestamp = 2;
    una_entrada3->frame_id = 15;

    list_replace_and_destroy_element(entradas_tlb, 3, una_entrada3, destroyer);

    CU_ASSERT_EQUAL(list_size(entradas_tlb), 4);

    int pos_reemplazo = -1;
    t_entrada_tlb *entrada_a_reemplazar_fifo = entrada_fifo_tlb(&pos_reemplazo);
    printf("pos :%d\n", pos_reemplazo);
    CU_ASSERT_EQUAL(pos_reemplazo, 0);
    printf("Entrada :%d\n", entrada_a_reemplazar_fifo->page_id);
    CU_ASSERT_EQUAL(entrada_a_reemplazar_fifo, una_entrada);

    t_entrada_tlb *entrada_a_reemplazar_lru = entrada_lru_tlb(&pos_reemplazo);
    printf("Prueba\n");
    CU_ASSERT_EQUAL(pos_reemplazo, 0);
    CU_ASSERT_EQUAL(entrada_a_reemplazar_lru, una_entrada);
    ////

    una_entrada->process_id = 0;
    una_entrada->libre = 0;
    una_entrada->page_id = 1;
    una_entrada->timestamp_entrada = 0;
    una_entrada->timestamp = 3;

    //list_replace(entradas_tlb, 0, una_entrada);
    ///
    //ist_replace_and_destroy_element(entradas_tlb, 0, una_entrada, destroyer);
    una_entrada1->process_id = 0;
    una_entrada1->libre = 0;
    una_entrada1->page_id = 2;
    una_entrada1->timestamp_entrada = 1;
    una_entrada1->timestamp = 1;
    //list_replace_and_destroy_element(entradas_tlb, 1, una_entrada, destroyer);
    //list_replace(entradas_tlb, 1, una_entrada1);

    //ENTRADA2 de la tlb

    una_entrada2->process_id = 0;
    una_entrada2->libre = 0;
    una_entrada2->page_id = 3;
    una_entrada2->timestamp_entrada = 2;
    una_entrada2->timestamp = 2;

        //list_replace_and_destroy_element(entradas_tlb, 2, una_entrada2, destroyer);
        //list_replace(entradas_tlb, 2, una_entrada2);

        //
        entrada_a_reemplazar_fifo = entrada_fifo_tlb(&pos_reemplazo);
    //printf("pos :%d\n", pos_reemplazo);
    CU_ASSERT_EQUAL(pos_reemplazo, 0);
    CU_ASSERT_EQUAL(entrada_a_reemplazar_fifo, una_entrada);

    entrada_a_reemplazar_lru = entrada_lru_tlb(&pos_reemplazo);
    CU_ASSERT_EQUAL(pos_reemplazo, 1);
    CU_ASSERT_EQUAL(entrada_a_reemplazar_lru, una_entrada1);

    //

    una_entrada->process_id = 0;
    una_entrada->libre = 0;
    una_entrada->page_id = 1;
    una_entrada->timestamp_entrada = 3;
    una_entrada->timestamp = 0;
    una_entrada->frame_id = 5;

    //list_replace_and_destroy_element(entradas_tlb, 0, una_entrada, destroyer);
    //list_replace(entradas_tlb, 0, una_entrada);
    ///
    una_entrada1->process_id = 0;
    una_entrada1->libre = 0;
    una_entrada1->page_id = 2;
    una_entrada1->timestamp_entrada = 1;
    una_entrada1->timestamp = 3;
    una_entrada1->frame_id = 6;

    //list_replace_and_destroy_element(entradas_tlb, 1, una_entrada1, destroyer);
    //list_replace(entradas_tlb, 1, una_entrada1);

    //ENTRADA2 de la tlb

    una_entrada2->process_id = 0;
    una_entrada2->libre = 0;
    una_entrada2->page_id = 3;
    una_entrada2->timestamp_entrada = 0;
    una_entrada2->timestamp = 2;
    una_entrada2->frame_id = 7;

    //list_replace_and_destroy_element(entradas_tlb, 2, una_entrada2, destroyer);
    //list_replace(entradas_tlb, 2, una_entrada2);

    //
    entrada_a_reemplazar_fifo = entrada_fifo_tlb(&pos_reemplazo);
    //printf("pos :%d\n", pos_reemplazo);
    CU_ASSERT_EQUAL(pos_reemplazo, 2);
    CU_ASSERT_EQUAL(entrada_a_reemplazar_fifo, una_entrada2);

    entrada_a_reemplazar_lru = entrada_lru_tlb(&pos_reemplazo);
    //printf("pos lru :%d\n", pos_reemplazo);
    CU_ASSERT_EQUAL(pos_reemplazo, 0);
    CU_ASSERT_EQUAL(entrada_a_reemplazar_lru, una_entrada);

    // verificando getPaginaTLB(int page,int proceso ID;)
    printf("Pre frame\n");
    int frame_entrada = getPaginaTLB(1, 0);  //frame:5
    int frame_entrada1 = getPaginaTLB(2, 0); //frame : 6
    int frame_entrada2 = getPaginaTLB(3, 0); //frame : 7

    int frame_entrada3 = getPaginaTLB(4, 0); //frame : -1
    int frame_entradax = getPaginaTLB(5, 0); //frame : -1

    printf("Post frame\n");
    printf("Frame: %d\n", frame_entradax);
    CU_ASSERT_EQUAL(frame_entrada, una_entrada->frame_id);
    CU_ASSERT_EQUAL(frame_entrada1, una_entrada1->frame_id);
    CU_ASSERT_EQUAL(frame_entrada2, una_entrada2->frame_id);
    CU_ASSERT_EQUAL(frame_entrada3, una_entrada3->frame_id);
    CU_ASSERT_EQUAL(frame_entradax, -1);

    printf("FInish\n", frame_entradax);
    /*free(una_entrada);
    free(una_entrada1);
    free(una_entrada2);*/
    free(stream_memoria);
    /*for (int c = 0; c < list_size(entradas_tlb); c++)
    {
        t_entrada_tlb *una_entrada_tlb = list_get(entradas_tlb, c);
        free(una_entrada_tlb);
    }*/

    //free(una_entrada2);
    //free(una_entrada3);

    for (int c = 0; c < list_size(entradas_tlb); c++)
    {
        t_entrada_tlb *una_entrada_tlb = list_get(entradas_tlb, c);
        free(una_entrada_tlb);
    }

    list_destroy(entradas_tlb);
    list_destroy(reserved_bitmap);
    list_destroy(pageTables);
    free(bitmap->bitarray);
    bitarray_destroy(bitmap);
}

void validar_algoritmo_MMU()
{

    inicializacion_estructuras_memoria();

    inicializacion();

    /* carpincho A */
    t_page_table tableA;

    tableA.process_id = 1;
    tableA.pages = list_create();

    t_page *pA1 = malloc(sizeof(t_page));
    t_page *pA2 = malloc(sizeof(t_page));
    t_page *pA3 = malloc(sizeof(t_page));

    pA1->page_id = 0;
    pA1->frame_id = 0;
    pA1->presencia = true;
    pA1->modificado = true;
    pA1->uso = true;
    pA1->timestamp = 1;
    pA1->process_id = 1;

    pA2->page_id = 1;
    pA2->frame_id = 1;
    pA2->presencia = false;
    pA2->modificado = true;
    pA2->uso = true;
    pA2->timestamp = 2;
    pA2->process_id = 1;

    pA3->page_id = 2;
    pA3->frame_id = 3;
    pA3->presencia = true;
    pA3->modificado = false;
    pA3->uso = true;
    pA3->timestamp = 3;
    pA3->process_id = 1;

    list_add(tableA.pages, pA1);
    list_add(tableA.pages, pA2);
    list_add(tableA.pages, pA3);

    /* carpincho B */
    t_page_table tableB;
    tableB.process_id = 2;
    tableB.pages = list_create();

    t_page *pB4 = malloc(sizeof(t_page));
    t_page *pB5 = malloc(sizeof(t_page));

    pB4->page_id = 0;
    pB4->frame_id = 5;
    pB4->presencia = true;
    pB4->modificado = false;
    pB4->uso = true;
    pB4->timestamp = 5;
    pB4->process_id = 2;

    pB5->page_id = 1;
    pB5->frame_id = 6;
    pB5->presencia = true;
    pB5->modificado = true;
    pB5->uso = false;
    pB5->timestamp = 4;
    pB5->process_id = 2;

    list_add(tableB.pages, pB4);
    list_add(tableB.pages, pB5);

    /* guardo las tablas a lista global */
    list_add(pageTables, &tableA);
    list_add(pageTables, &tableB);

    char *algoritmo = get_algoritmo_reemplazo_mmu();
    char *asignacion = get_tipo_asignacion();

    if ((strcmp(algoritmo, "LRU") == 0) && (strcmp(asignacion, "FIJA") == 0))
    {
        t_page *page_victima = busqueda_por_LRU(1);
        t_page *page_victima2 = busqueda_por_LRU(2);

        CU_ASSERT_EQUAL(page_victima->frame_id, 0);
        CU_ASSERT_EQUAL(page_victima2->frame_id, 6);
    }

    if ((strcmp(algoritmo, "LRU") == 0) && (strcmp(asignacion, "DINAMICA") == 0))
    {
        t_page *page_victima1 = busqueda_por_LRU(1);
        //t_page * page_victima2 = busqueda_por_LRU(2);

        CU_ASSERT_EQUAL(page_victima1->frame_id, 0);
        //CU_ASSERT_EQUAL(page_victima2->frame_id, 0);
    }

    if ((strcmp(algoritmo, "CLOCK-M") == 0) && (strcmp(asignacion, "FIJA") == 0))
    {
        t_page *page_victima = busquedaPorClockModificado(1);
        t_page *page_victima2 = busquedaPorClockModificado(2);

        CU_ASSERT_EQUAL(page_victima->frame_id, 3);
        CU_ASSERT_EQUAL(page_victima2->frame_id, 6);
    }

    if ((strcmp(algoritmo, "CLOCK-M") == 0) && (strcmp(asignacion, "DINAMICA") == 0))
    {

        t_page *page_victima1 = busquedaPorClockModificado(1);
        //t_page * page_victima2 = busqueda_por_LRU(2);

        CU_ASSERT_EQUAL(page_victima1->frame_id, 6);
        //CU_ASSERT_EQUAL(page_victima2->frame_id, 0);
    }
    t_entrada_tlb *una_entrada_to_free;
    printf("\nentradas:%d\n", list_size(entradas_tlb));
    for (int i = 0; i < list_size(entradas_tlb); i++)
    {

        una_entrada_to_free = list_get(entradas_tlb, i);

        free(una_entrada_to_free);
    }

    list_destroy(tableA.pages);
    list_destroy(tableB.pages);
    list_destroy(reserved_bitmap);
    list_destroy(pageTables);
    list_destroy(entradas_tlb);
    bitarray_destroy(bitmap);
    free(pA1);
    free(pA2);
    free(pA3);
    free(pB4);
    free(pB5);
}

verificar_suma_tlb_hit_miss()
{

    inicializacion_estructuras_memoria();

    t_page_table tableA;
    tableA.process_id = 1;
    tableA.pages = list_create();
    tableA.amount_tlb_hit = 0;
    tableA.amount_tlb_miss = 0;

    t_page_table tableB ;
    tableB.process_id = 2;
    tableB.pages = list_create();
    tableB.amount_tlb_hit = 0;
    tableB.amount_tlb_miss = 0;

    t_page_table tableC ;
    tableC.process_id = 3;
    tableC.pages = list_create();
    tableC.amount_tlb_hit = 0;
    tableC.amount_tlb_miss = 0;

    // t_page page1;
    // page.page_id = 1;
    // page.frame_id = 1;
    // page.presencia = true;
    // page.modificado = false;
    // page.timestamp = 1;
    // list_add(tableB.pages, &page);

    list_add(pageTables, &tableA);
    list_add(pageTables, &tableB);
    list_add(pageTables, &tableC);

    sumar_tlb_hit(1);
    sumar_tlb_hit(1);
    sumar_tlb_hit(1);
    sumar_tlb_hit(1);

    sumar_tlb_miss(2);
    sumar_tlb_miss(2);
    sumar_tlb_hit(2);
    sumar_tlb_hit(2);

    sumar_tlb_miss(3);
    sumar_tlb_miss(3);
    sumar_tlb_miss(3);
    sumar_tlb_miss(3);
    sumar_tlb_miss(3);
    //printf("%d-%d-%d-%d-%d-%d-%d-%d\n", tlb_hit_totales, tlb_miss_totales, tableA->amount_tlb_hit, tableA->amount_tlb_miss, tableB->amount_tlb_hit, tableB->amount_tlb_miss, tableC->amount_tlb_hit, tableC->amount_tlb_miss);
    CU_ASSERT_EQUAL(tlb_hit_totales, 6);
    CU_ASSERT_EQUAL(tlb_miss_totales, 7);
    CU_ASSERT_EQUAL(tableA.amount_tlb_hit, 4);
    CU_ASSERT_EQUAL(tableA.amount_tlb_miss, 0);

    CU_ASSERT_EQUAL(tableB.amount_tlb_hit, 2);
    CU_ASSERT_EQUAL(tableB.amount_tlb_miss, 2);

    CU_ASSERT_EQUAL(tableC.amount_tlb_hit, 0);
    CU_ASSERT_EQUAL(tableC.amount_tlb_miss, 5);

    mostrar_tlb_hit_miss_globales();

    mostrar_tlb_hit_miss_por_proceso();

    /*list_destroy(tableA.pages);
    list_destroy(tableB.pages);
    list_destroy(reserved_bitmap);
    list_destroy(pageTables);
    list_destroy(entradas_tlb);
    bitarray_destroy(bitmap);
    list_destroy(tableC.pages);*/

    // free(tableA);
    // free(tableB);
    // free(tableC);
    // list_destroy(pageTables);
    //free(entradas_tlb);
}

void generar_dump_tlb_duplicado()
{
    //char *format = "%d/%m/%y %H:%M:%S";

    //char *timestampT;
    // en el test se rompe aca
    printf("Test2\n");
    //char *timestampT = temporal_get_string_time("%d/%m/%y %H:%M:%S");
    printf("Test3\n");

    char *contenido = string_new();
    string_append_with_format(&contenido, "%s", "----------------------------------\n");
    //string_append_with_format(&contenido, "Dump: %s\n", timestampT);
    int cant_tablas = list_size(entradas_tlb);
    printf("Test\n");
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
    string_append_with_format(&contenido, "%s", "----------------------------------\n");
    printf("%s\n", contenido);
    
    //Mostrar dump de las entradas de la tlb
    //free(timestampT);
    // timestampT = temporal_get_string_time("");
    char *path = string_new();
    string_append_with_format(&path, "       %s", get_path_dump_tlb());
    string_append_with_format(&path, "%s", "/Dump_.tlb");
    string_trim_left(&path);
    printf("%s\n", path);

    int file_size = strlen(contenido);
    printf("\n file_size %d\n", file_size);
    
    int fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(fd, file_size);
    void *stream = mmap(NULL, file_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    strcpy(stream, contenido);
    close(fd);
    //free(contenido);
    //free(path);
    //free(timestampT);
}