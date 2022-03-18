#include "config_swamp.h"

t_config_swamp *leer_config_swamp(t_config *config)
{
	t_config_swamp *config_swamp = malloc(sizeof(t_config_swamp));

	config_swamp->IP = config_get_string_value(config, "IP");
	config_swamp->PUERTO = config_get_int_value(config, "PUERTO");
	config_swamp->TAMANIO_SWAP = config_get_int_value(config, "TAMANIO_SWAP");
	config_swamp->TAMANIO_PAGINA = config_get_int_value(config, "TAMANIO_PAGINA");

	config_swamp->ARCHIVOS_SWAP = config_get_string_value(config, "ARCHIVOS_SWAP");

	config_swamp->MARCOS_MAXIMOS = config_get_int_value(config, "MARCOS_MAXIMOS");
	config_swamp->RETARDO_SWAP = config_get_int_value(config, "RETARDO_SWAP");

	config_swamp->TIPO_ASIGNACION = config_get_string_value(config, "TIPO_ASIGNACION");

	return config_swamp;
}
