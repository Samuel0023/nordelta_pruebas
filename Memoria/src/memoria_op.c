#include "memoria_op.h"
bool es_DL_valida(int dir_logica,int tam);
int lectura_contenido(t_msj_memwrite data);
bool es_DL_valida(int dir_logica,int tam){
    if(strcmp(get_tipo_asignacion(),"FIJA") == 0){
        return esDireccionLogicaValida(dir_logica,tam);
    }else{
        //asignacion dinamica
        return false;
    }
}
int lectura_contenido(t_msj_memwrite data){
    int dir_logica = data.dir_logica;
    int pid = data.pid_carpincho;
    void* contenido = data.contenido;
    int tam_contenido = data.tam_contenido;
    int tamanio_pagina = get_tamanio_pagina();

    //-----------------------------------------------------------------------------
    int pageid = dir_logica / tamanio_pagina;

    int df = DLconvertToDF(dir_logica, pid);
    int df_siguiente;

    int original = df/tamanio_pagina;
    int original_con_hmd = (df+9)/tamanio_pagina;
    if(original!=original_con_hmd){
        df_siguiente = DLconvertToDF((pageid+1)*tamanio_pagina, pid);
    }

    t_heap_meta_data hmd = deserializarHeapMetaData(df);
    int size = getMallocSize(df, hmd);
    //-----------------------------------------------------------------------------
    int df_alloc = df+9;
    

    //30-60

    //32 pagina
    //df_alloc =>50
    //Tam => 100, 150
    //Pagina inicial: 1 =>32-64-96-128
    //Pagina: 5
    int pagina_inicio = df_alloc / tamanio_pagina;
    int pagina_fin = (df_alloc + tam_contenido) / tamanio_pagina;

    int inicio;
    int tamanio_a_leer;
    int acumulativo = tam_contenido;   
    void *respuesta = malloc(tam_contenido);
    for(int c=pagina_inicio; c<pagina_fin;c++){
        int df_auxiliar = DLconvertToDF((c)*tamanio_pagina, pid);
        
        if(c==pagina_inicio){
            inicio = df_alloc;
            tamanio_a_leer = tamanio_pagina-df_alloc%tamanio_pagina;
        }else{
            inicio = df_auxiliar;
            tamanio_a_leer = tamanio_pagina;
        }
        acumulativo -= tamanio_a_leer;
        if(acumulativo<0){
            tamanio_a_leer += tamanio_pagina;
            acumulativo += tamanio_pagina;
        }
        memcpy(respuesta+tam_contenido-acumulativo, stream_memoria+inicio, tamanio_a_leer);
    }

    return df_alloc;
}
void mem_mate_init(int process_id,int cliente_fd){
    create_table_page(process_id);
    //asignacion fija
    printf("\n tipo de asignacion %s\n",get_tipo_asignacion());
    
    if(strcmp(get_tipo_asignacion(),"FIJA")==0){
        if(check_disp_reserved_bitmap()){
            book_reserved_bitmap(process_id); 
            //comentar para la ejecucion de las pruebas unitarias
            sendMessage(make_res("mate_init successful"),cliente_fd);  
        }else{
            sendMessage(make_res("espacio no disponible(bitmap)"),cliente_fd);
        }
    }
}
int memalloc(char* size){
   // primero que debemos hacer es buscar si ya hay algún alloc libre en el espacio de direcciones de p donde quepa el size solicitado.
    
    return 1;
}

void memfree(){
}

void* memread(t_msj_memwrite data){
    if(es_DL_valida(data.dir_logica,data.tam_contenido)){
       return lectura_contenido(data);
    }
    return -5;
}

int memwrite(t_msj_memwrite memwrite){
    int dir_logica = memwrite.dir_logica;
    int pid = memwrite.pid_carpincho;
    void* contenido = memwrite.contenido;
    int tam_contenido = memwrite.tam_contenido;
    int tamanio_pagina = get_tamanio_pagina();

    printf("Dir logica: %d PId: %d %d\n", dir_logica, pid, tam_contenido);
    //-----------------------------------------------------------------------------
    int pageid = dir_logica / tamanio_pagina;

    int df = DLconvertToDF(dir_logica, pid);
    int df_siguiente;

    int original = df/tamanio_pagina;
    int original_con_hmd = (df+9)/tamanio_pagina;
    if(original!=original_con_hmd){
        df_siguiente = DLconvertToDF((pageid+1)*tamanio_pagina, pid);
    }

    t_heap_meta_data hmd = deserializarHeapMetaData(df);
    int size = getMallocSize(df, hmd);
    //-----------------------------------------------------------------------------
    int df_alloc = df+9;
    
    int pagina_inicio = df_alloc / tamanio_pagina;
    int pagina_fin = (df_alloc + tam_contenido) / tamanio_pagina;

    int inicio;
    int tamanio_a_leer;
    int acumulativo = tam_contenido;   
    void *respuesta = malloc(tam_contenido);
    for(int c=pagina_inicio; c<pagina_fin;c++){
        int df_auxiliar = DLconvertToDF((c)*tamanio_pagina, pid);
        
        if(c==pagina_inicio){
            inicio = df_alloc;
            tamanio_a_leer = tamanio_pagina-df_alloc%tamanio_pagina;
        }else{
            inicio = df_auxiliar;
            tamanio_a_leer = tamanio_pagina;
        }
        acumulativo -= tamanio_a_leer;
        if(acumulativo<0){
            tamanio_a_leer += tamanio_pagina;
            acumulativo += tamanio_pagina;
        }
        memcpy(stream_memoria+inicio, respuesta+tam_contenido-acumulativo, tamanio_a_leer);
    }

    return df_alloc;
}

