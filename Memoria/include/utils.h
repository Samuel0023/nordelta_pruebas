#ifndef UTILS_H_
#define UTILS_H_

#include "TAD.h"
#include "tad_memoria.h"
#include "sd_pageswamp.h"
#include "config_servicio.h"
#include <commons/bitarray.h>
#include "socket_client.h"
#include "server_utils.h"
#include "bitmap_utils.h"
#include "sd_cant_marcos_swamp.h"
#include <commons/collections/list.h>

bool esDireccionLogicaValida(int inicioDL, int contenido);
int getFramesToUse(int tamanioBytes);
int get_first_available_bitmap();
void print_bit_map(t_bitarray *bitarray);
void create_table_page(int process_id);
bool check_disp_reserved_bitmap();
void book_reserved_bitmap(int process_id);
int getcantMarcosSWAMP();
void transferir_a_swamp(t_page * page);
void sendSwamp(t_page_swap pageswamp);
t_page_table *getTablaPaginas(int procesoID);
t_list *searchPages(int DL, int processID);
int DLconvertToDF(int DL, int process_id);
t_page getPage(int page_id, int process_id);
t_page pointer_page_convert_to_page(t_page *pointer_page);
bool hayAllocDisponible(int param_alloc_size, int proceso_id);
t_page * find_page_in_pageTable(int page_id, t_list * pages);
int get_first_available_bitmap_memoria(int procesoID);

char *get_label_presencia(bool);

extern int conexionSWAMP;

#endif