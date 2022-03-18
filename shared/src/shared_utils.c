#include "../include/shared_utils.h"
#include <string.h>

//Funcion de ejemplo del git de VSCODE
char *mi_funcion_compartida()
{
    return "Hice uso de la shared!";
}

t_log *iniciar_logger(char *logfile, char *logtag) //Formato: "/xxxxxx.log"
{
    char *current_working_directory = getcwd(NULL, 0);

    char *path_log = string_new();
    string_append_with_format(&path_log, "%s", current_working_directory);
    string_append_with_format(&path_log, "%s", logfile);

    t_log *logger = log_create(path_log, logtag, 1, LOG_LEVEL_INFO);

    free(path_log);
    free(current_working_directory);

    return logger;
}

void escribir_log_info(t_log *logger, char *textoppal, char *variable)
{
    log_info(logger, textoppal, variable);
}
void escribir_log_warning(t_log *logger, char *textoppal, char *variable)
{
    log_warning(logger, textoppal, variable);
}
void escribir_log_error(t_log *logger, char *textoppal, char *variable)
{
    log_error(logger, textoppal, variable);
}

void borrar_log(t_log *logger)
{
    log_destroy(logger);
}

char *get_string_operacion(op_code code)
{
    switch (code)
    {
    //case SALIR: return "SALIR";
    //case TAREA: return "TAREA";
    default:
        return "NO DEFINIDO";
    }
}

t_bitarray *initialize_bitmap(int cant_bits)
{
    void *puntero_bitmap = malloc(cant_bits / 8);
    t_bitarray *bitmap = bitarray_create_with_mode(puntero_bitmap, cant_bits / 8, LSB_FIRST);

    int bits = bitarray_get_max_bit(bitmap);
    for (int c = 0; c < bits; c++)
    {
        bitarray_clean_bit(bitmap, c);
    }
    //free(puntero_bitmap);
    return bitmap;
}
