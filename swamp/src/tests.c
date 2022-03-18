#include "../include/tests.h"
int run_tests()
{
    CU_initialize_registry();
    CU_pSuite tests = CU_add_suite("Cliente Suite", NULL, NULL);
    CU_add_test(tests, "Verificar serializacion y deserializacion lectura_pagina", verificarLecturaPagina);
    CU_add_test(tests, "Verificar serializacion y deserializacion respuesta_lectura_pagina", verificar_respuesta_lectura_pagina);
    CU_add_test(tests, "Verificar llenar archivo", verificar_llenar_archivo);
    CU_add_test(tests, "Verificar agregado correcto de la list t_paths", verificar_devolver_paths);
    CU_add_test(tests, "Verificar funcion inicializacion", verificar_inicializacion);
    CU_add_test(tests, "Verificar escritura", check_write_page);
    CU_add_test(tests, "Verificar cant marcos swamp", verificar_cant_marcos);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

void verificarLecturaPagina()
{
    inicializacion_de_recursos();
    inicializacion();

    PageTable *table = malloc(sizeof(PageTable));
    table->procesoID = 1;
    table->indiceArchivo = 1;
    table->paginas = list_create();

    list_add(tabla_paginas, table);

    PageSwap pagina_swap;
    pagina_swap.tamanioContenido = 2;
    pagina_swap.proceso_id = 1;
    pagina_swap.page_id = 1;
    pagina_swap.contenido = malloc(pagina_swap.tamanioContenido);

    memcpy(pagina_swap.contenido, "1", pagina_swap.tamanioContenido);
    guardarContenido(pagina_swap);

    t_package paquete = ser_lectura_pagina(pagina_swap);
    PageSwap pagina_swap2 = des_lectura_pagina(paquete);
    void *content2 = lectura_pagina(pagina_swap2);

    CU_ASSERT_EQUAL(pagina_swap.tamanioContenido, pagina_swap2.tamanioContenido);
    CU_ASSERT_EQUAL(pagina_swap.proceso_id, pagina_swap2.proceso_id);
    CU_ASSERT_EQUAL(pagina_swap.page_id, pagina_swap2.page_id);
    CU_ASSERT_STRING_EQUAL(pagina_swap.contenido, pagina_swap2.contenido);
    CU_ASSERT_STRING_EQUAL(pagina_swap2.contenido, content2);
    free(paquete.buffer);
    free(pagina_swap.contenido);
    free(pagina_swap2.contenido);
    for (int c = 0; c < list_size(t_files); c++)
    {
        t_file *file = list_get(t_files, c);
        list_destroy(file->bitmap_reservado);
        free(file->bitmap->bitarray);
        bitarray_destroy(file->bitmap);
        free(file);

        free(paths_string[c]);
    }

    free(content2);
    free(paths_string);
    list_destroy(t_paths);
    list_destroy(t_files);
    for (int c = 0; c < list_size(table->paginas); c++)
    {
        Page *page = list_get(table->paginas, c);
        free(page);
    }
    list_destroy(table->paginas);
    list_destroy(tabla_paginas);
    free(table);
}

void verificar_respuesta_lectura_pagina()
{
    char *content = "Hola mundo";
    respuesta_lectura_pagina respuesta_swap;
    respuesta_swap.tamanio = strlen(content) + 1;

    respuesta_swap.contenido = malloc(respuesta_swap.tamanio);
    memcpy(respuesta_swap.contenido, content, respuesta_swap.tamanio);

    t_package paquete = ser_respuesta_lectura_pagina(respuesta_swap);
    respuesta_lectura_pagina respuesta_swap2 = des_respuesta_lectura_pagina(paquete);

    CU_ASSERT_EQUAL(respuesta_swap.tamanio, respuesta_swap2.tamanio);

    CU_ASSERT_STRING_EQUAL(respuesta_swap.contenido, respuesta_swap2.contenido);

    free(paquete.buffer);
    free(respuesta_swap.contenido);
    free(respuesta_swap2.contenido);
}

void verificar_devolver_paths()
{
    inicializacion_de_recursos();
    inicializacion();

    char *string_parcial2 = string_substring(config->ARCHIVOS_SWAP, 1, string_length(config->ARCHIVOS_SWAP) - 2);
    char **paths_string2 = string_split(string_parcial2, ",");
    int cantidad_de_archivos = obtener_cantidad_argumentos(paths_string2);

    CU_ASSERT_EQUAL(cantidad_de_archivos, list_size(t_paths));
    for (int i = 0; i < cantidad_de_archivos; i++)
    {

        string_trim(&paths_string2[i]);
        CU_ASSERT_STRING_EQUAL(paths_string[i], paths_string2[i]);
    }
    liberar_puntero_doble(paths_string2);
    free(string_parcial2);

    for (int c = 0; c < list_size(t_files); c++)
    {
        t_file *file = list_get(t_files, c);
        list_destroy(file->bitmap_reservado);
        free(file->bitmap->bitarray);
        bitarray_destroy(file->bitmap);
        free(file);

        free(paths_string[c]);
    }

    free(paths_string);
    list_destroy(t_paths);
    list_destroy(t_files);
    list_destroy(tabla_paginas);
}

void verificar_inicializacion()
{
    int cantidad_de_archivos = list_size(t_paths);
    CU_ASSERT_EQUAL(cantidad_de_archivos, list_size(t_files));
    for (int c = 0; c < cantidad_de_archivos; c++)
    {

        t_file *tfile = malloc(sizeof(t_file));
        tfile->path = list_get(t_paths, c);

        t_file *tfile2 = list_get(t_files, c);

        CU_ASSERT_STRING_EQUAL(tfile->path, tfile2->path);
        CU_ASSERT_EQUAL(bitarray_get_max_bit(tfile2->bitmap), marcos_en_swamp);
        CU_ASSERT_EQUAL(list_size(tfile2->bitmap_reservado), marcos_en_swamp);
        free(tfile);
    }
}

void verificar_llenar_archivo()
{
    inicializacion_de_recursos();
    inicializacion();

    char *dato = llenar_archivo(1, "HolaCoo");
    int lengthDato = strlen(dato);

    CU_ASSERT_STRING_EQUAL(dato, "HolaCoo");

    char *path_archivo = list_get(t_paths, 1);

    int archivoFD = open(path_archivo, O_RDONLY, S_IRUSR);

    if (archivoFD <= -1)
    {
        printf("[LECTURA] Fallo el abrir el bloque\n");
    }

    int file_size = config->TAMANIO_SWAP;
    ftruncate(archivoFD, file_size);

    void *stream = mmap(NULL, file_size, PROT_READ, MAP_SHARED, archivoFD, 0);
    char *output = malloc(lengthDato);
    output[lengthDato] = '\0';
    memcpy(output, stream, lengthDato);

    CU_ASSERT_STRING_EQUAL(output, "HolaCoo");

    free(output);
    close(archivoFD);

    for (int c = 0; c < list_size(t_files); c++)
    {
        t_file *file = list_get(t_files, c);
        list_destroy(file->bitmap_reservado);
        free(file->bitmap->bitarray);
        bitarray_destroy(file->bitmap);
        free(file);
        free(paths_string[c]);
    }
    free(paths_string);
    list_destroy(t_paths);
    list_destroy(t_files);
    list_destroy(tabla_paginas);
}

void check_write_page()
{
    inicializacion_de_recursos();
    inicializacion();

    t_list *paginas = list_create();

    PageSwap page_swap;
    page_swap.tamanioContenido = 2;
    page_swap.proceso_id = 1;
    page_swap.page_id = 1;
    page_swap.contenido = malloc(page_swap.tamanioContenido);

    Page *pag1 = malloc(sizeof(Page));
    pag1->pageID = 1;
    pag1->marco = 2;

    list_add(paginas, pag1);

    PageTable *table = malloc(sizeof(PageTable));
    table->procesoID = 1;
    table->indiceArchivo = 1;
    table->paginas = paginas;

    list_add(tabla_paginas, table);

    int cantidad_de_archivos = list_size(t_paths);
    guardarContenido(page_swap);
    
    free(page_swap.contenido);

    for (int c = 0; c < list_size(table->paginas); c++)
    {
        Page *page = list_get(table->paginas, c);
        free(page);
    }
    list_destroy(table->paginas);

    free(table);
    for(int c=0; c<list_size(t_files);c++){
        t_file* file = list_get(t_files, c);
        printf("Test: %s\n", file->path);
        list_destroy(file->bitmap_reservado);
        free(file->bitmap->bitarray);
        bitarray_destroy(file->bitmap);
        free(file);
    }
    list_destroy(t_paths);
    list_destroy(tabla_paginas);
}

void verificar_cant_marcos(){
    int cant_marcos = get_cant_marcos_swamp();
    CU_ASSERT_EQUAL(cant_marcos, 10);
}