#ifndef SD_LECTURA_PAGINA_H
#define SD_LECTURA_PAGINA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <TAD.h>
#include <commons/string.h>

/* ******* MEMORIA ******* */
t_package ser_lectura_pagina(PageSwap);
respuesta_lectura_pagina des_respuesta_lectura_pagina(t_package paquete);
t_package ser_page_swamp(PageSwap data_buffer, int opCode);
t_package ser_lectura_pagina_swamp(PageSwap data_buffer);
t_package ser_escritura_pagina_swamp(PageSwap data_buffer);
t_package ser_borrado_pagina_swamp(PageSwap data_buffer);
/* ******* SWAMP ******* */
PageSwap des_page_swamp(t_package paquete);
PageSwap des_lectura_pagina(t_package);
t_package ser_respuesta_lectura_pagina(respuesta_lectura_pagina data_buffer);
#endif