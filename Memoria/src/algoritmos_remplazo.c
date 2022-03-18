#include "../include/algoritmos_de_remplazo.h"

t_page * page_menor_timestamp(t_page * page1, t_page * page2){
    if (page1->timestamp < page2->timestamp)
    {
        return page1;
    }
    else
    {
        return page2;
    }  
}

t_page * busqueda_por_LRU(int proceso_id){

    t_list * list_pages_mp=  getPaginasEnMemoria(proceso_id);
    t_page * page_menor = list_get_minimum(list_pages_mp, page_menor_timestamp);
    
    return page_menor;
}

t_page * busqueda_caso_primario_clock(t_list * paginasEnMemoria){	
    int limiteMaximo = list_size(paginasEnMemoria);
    bool seEncontro;
    t_page * seleccion;

    for(int c=puntero_clock; c <limiteMaximo; c++){
        t_page * page = list_get(paginasEnMemoria, puntero_clock);	
        if(!page->uso && !page->modificado)
                seEncontro = true;

        seleccion = page;
        puntero_clock++;

        if(puntero_clock == limiteMaximo)
            puntero_clock = 0;

        if(seEncontro){
            return seleccion;
        }
        
    }

    return NULL;
}

t_page * busqueda_caso_segundo_clock(t_list * paginasEnMemoria){	
    int limiteMaximo = list_size(paginasEnMemoria);
    t_page * seleccion;
    bool seEncontro;
        
    for(int c=puntero_clock; c <limiteMaximo; c++){
        t_page * page = list_get(paginasEnMemoria, puntero_clock);	
        if(!page->uso && page->modificado){
            seEncontro = true;
        }
        else {
            page->presencia = false;
        }
        seleccion = page;
        puntero_clock++;

        if(puntero_clock == limiteMaximo)
            puntero_clock = 0;

        if(seEncontro){
            return seleccion;
        }
        
    }
    return NULL;
}

t_page * busquedaPorClockModificado(int procesoID){
    t_list * paginasEnMemoria = getPaginasEnMemoria(procesoID);
        
    //Notas: la memoria se encuentra llena
    t_page* page=  NULL;
	do{
        page = busqueda_caso_primario_clock(paginasEnMemoria);
        if(page == NULL)
            page=busqueda_caso_segundo_clock(paginasEnMemoria);
    }while(page == NULL);

	return page;
}