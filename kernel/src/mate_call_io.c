#include "mate_call_io.h"

bool existe_pcb(int process_id,t_device *device){
    bool _existe_pcb(PCB_Carpincho *pcb){
        return pcb->id == process_id;
    }
    list_any_satisfy(device->pcbs,(void*)_existe_pcb);
}

t_device *get_dispositivo(char *name_device){
    bool _es_el_dispositivo(t_device *device){
        printf("%s-%s\n", device->name, name_device);
        return strcmp(device->name, name_device) == 0;
    }
    return list_find(devices,(void*)_es_el_dispositivo);
}

void mate_call_io(char *name_device,int proceso_id){
    printf("Proceso id: %d\n", proceso_id);
    t_device* device =get_dispositivo(name_device);            
    printf("\n cant %d\n",list_size(devices));
    printf("\n device %s\n",device->name);

    list_add(device->pcbs,find_pcb_in_kernel(proceso_id));
    sem_post(&device->pcb_pendientes);
}