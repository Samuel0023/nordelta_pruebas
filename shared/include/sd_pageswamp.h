#ifndef SD_PAGESWAMP_H
#define SD_PAGESWAMP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <commons/config.h>
#include <string.h>
#include <TAD.h>

t_package ser_pageswamp(t_page_swap data_buffer);
t_page_swap des_pageswamp(t_package paquete);

#endif