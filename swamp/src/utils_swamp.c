#include "utils_swamp.h"

void recibir_mensaje_tp0(int socket_cliente)
{
	int size;
	char *buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

void creacion_archivos_swap(int posicion)
{

	int archivo = open(list_get(t_paths, posicion), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	if (archivo == -1)
	{
		perror("Error creacion Archivo");
	}

	int trunqueador = ftruncate(archivo, config->TAMANIO_SWAP);

	if (trunqueador != 0)
	{
		perror("Error ftruncate");
	}

	close(archivo);
}

void inicializacion()
{
	int cantidad_de_archivos;

	for (int c = 0; (c < list_size(t_paths)); c++)
	{
		t_file *tfile = malloc(sizeof(t_file));
		tfile->bitmap = initialize_bitmap(marcos_en_swamp);

		char *path = list_get(t_paths, c);
		tfile->path = path;

		//bitmap_reservado es una lista de enteros
		tfile->bitmap_reservado = list_create();

		for (int h = 0; h < marcos_en_swamp; h++)
		{
			list_add(tfile->bitmap_reservado, 0);
		}

		creacion_archivos_swap(c);
		list_add(t_files, tfile);
	}
}

bool _file_bigger(t_file *file, t_file *file_less_bigger)
{
	return (file->bitmap)->size <= (file_less_bigger->bitmap)->size;
}

int find_better_file()
{

	t_file *file = (t_file *)list_get_maximum(t_files, (void *)_file_bigger);

	for (int i = 0; i < list_size(t_files); i++)
	{
		if (file == list_get(t_files, i))
		{
			return i;
		}
	}
}
void asignacionPageTable(PageTable *table, code_asig cod_op)
{

	if (cod_op == ASIGNACION_FIJA)
	{
		t_file *file = list_get(t_files, table->indiceArchivo);
		t_bitarray *bitmap = file->bitmap;
		t_list *bitmapReservados = file->bitmap_reservado;
		int inicio = 0;
		int elem;
		for (int c = 0; c < bitarray_get_max_bit(bitmap); c++)
		{
			if (bitarray_test_bit(bitmap, c) == false)
			{
				inicio = c;
				break;
			}
		}
		for (int c = inicio; c < inicio + MARCOS_POR_CARPINCHO; c++)
		{
			list_replace(bitmapReservados, c, table->procesoID);
		}
	}
}
PageTable *getPageTable(int proceso_id)
{
	PageTable *table;
	PageTable *find_tabla(int proceso_id)
	{
		bool _es_el_mismo_id(PageTable * pageTable)
		{
			return proceso_id == pageTable->procesoID;
		}
		return list_find(tabla_paginas, (void *)_es_el_mismo_id);
	}

	table = find_tabla(proceso_id);
	if (table == NULL)
	{
		table = malloc(sizeof(PageTable));
		table->procesoID = proceso_id;
		table->indiceArchivo = find_better_file();
		asignacionPageTable(table, ASIGNACION_FIJA);
		list_add(tabla_paginas, table);
	}
	return table;
}

int obtener_cantidad_argumentos(char **tokens)
{
	int i = 0;
	int cantidad = 0; /*  */

	while (*(tokens + i) != NULL)
	{
		cantidad++;
		i++;
	}

	return cantidad;
}

void liberar_puntero_doble(char **puntero_doble)
{

	int i = 0;
	while (*(puntero_doble + i) != NULL)
	{

		free(*(puntero_doble + i));
		i++;
	}

	free(puntero_doble);
}

char *llenar_archivo(int indice, char *dato)
{
	int length_input = strlen(dato);

	char *path_archivo = list_get(t_paths, indice);

	int archivoFD = open(path_archivo, O_RDWR, S_IRUSR | S_IWUSR);
	int file_size = 100;
	ftruncate(archivoFD, file_size);

	void *stream = mmap(NULL, file_size, PROT_WRITE | PROT_READ, MAP_SHARED, archivoFD, 0);
	memcpy(stream, dato, length_input);

	return dato;
}

Page *get_page(PageTable *tabla_pagina_swap, int pageID)
{
	bool misma_pagina_id(Page * item)
	{
		return item->pageID == pageID;
	}
	Page *pagina = list_find(tabla_pagina_swap->paginas, &misma_pagina_id);
	return pagina;
}

void *get_stream_file(char *path)
{
	int archivoFD = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (archivoFD <= -1)
	{
		printf("[LECTURA] Fallo el abrir el archivo\n");
		//return;
		//break;
	}

	int file_size = config->TAMANIO_SWAP;
	ftruncate(archivoFD, file_size);
	void *stream = mmap(NULL, file_size, PROT_WRITE | PROT_READ, MAP_SHARED, archivoFD, 0);
	if (stream == MAP_FAILED)
	{
		printf("[LECTURA] Fallo el map!\n");

		if (errno == EINVAL)
		{
			printf("[LECTURA] Tamanio del archivo es 0\n");
		}

		return;
	}
	return stream;
}

int get_cant_marcos_swamp()
{
	int cant_marcos = config->TAMANIO_SWAP;

	if (strcmp(config->TIPO_ASIGNACION, "FIJA"))
		cant_marcos = config->MARCOS_MAXIMOS;
	return cant_marcos;
}