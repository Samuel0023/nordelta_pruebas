#include "operationIO.h"

void *lectura_pagina(PageSwap pagina_swap)
{
	PageTable *tabla_pagina_swap = getPageTable(pagina_swap.proceso_id);
	Page *pagina = get_page(tabla_pagina_swap, pagina_swap.page_id);

	int marco = pagina->marco;
	char *path = list_get(t_paths, tabla_pagina_swap->indiceArchivo);

	// Apertura de archivo y lectura
	int TAMANIO_PAGINA = config->TAMANIO_PAGINA;
	void *stream = get_stream_file(path);
	char *contenido = malloc(TAMANIO_PAGINA);

	memcpy(contenido, stream + marco * TAMANIO_PAGINA, TAMANIO_PAGINA);
	return contenido;
}

void guardarContenido(PageSwap page_swap)
{
	PageTable *table = getPageTable(page_swap.proceso_id);
	t_file *file = list_get(t_files, table->indiceArchivo);

	int bits = bitarray_get_max_bit(file->bitmap);
	t_bitarray *bitmap = file->bitmap;
	int marcoID = get_first_available_bitmap(file->bitmap, table->procesoID, file->bitmap_reservado, MARCOS_POR_CARPINCHO, config->TIPO_ASIGNACION);
	void *contenido = page_swap.contenido;

	Page *page = malloc(sizeof(Page));
	page->pageID = page_swap.page_id;
	page->marco = marcoID;

	void *buffer = get_stream_file(file->path);
	memcpy(buffer + marcoID * config->TAMANIO_PAGINA, contenido, config->TAMANIO_PAGINA);

	list_add(table->paginas, page);
}