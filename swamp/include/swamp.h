#ifndef SWAMP_H_
#define SWAMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include "utils_swamp.h"
#include "server_utils.h"
#include "server.h"
#include "config_utils.h"
#include "tad_swamp.h"
#include <commons/config.h>/*  */
#include "../include/sd_messages.h"
#include "../include/config_swamp.h"
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/tad_swamp.h"
#include <signal.h>
#include <commons/collections/list.h>
#include "shared_utils.h"
#include <sys/mman.h>

t_log *logger;
t_config_swamp *config;

#endif /* SWAMP_H_ */