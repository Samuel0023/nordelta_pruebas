#ifndef BITMAP_UTILS_H
#define BITMAP_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/bitarray.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "TAD.h"
#include <commons/string.h>

int get_first_available_bitmap(
	t_bitarray *bitmap, int processID,
	t_list *reserved_bitmap, int FRAMES_PER_PROCESS, char *ASSIGNATION_TYPE);

#endif