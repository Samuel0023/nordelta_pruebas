#ifndef GET_PAGINA_H_
#define GET_PAGINA_H_

#include "tad_memoria.h"
#include "utils.h"
#include "sd_lectura_pagina.h"
#include "../include/algoritmos_de_remplazo.h"

t_page *getPagina(int pageID, int procesoID);
int getFrameLibre(int procesoid);
t_list * getPaginasEnMemoria(int procesoid); //[t_page]
void *getPageSwamp(int procesoId, int pageId);
t_list * get_paginas_id_memoria(int procesoid); //[t_page]
t_list * get_all_paginas_memoria();


#endif