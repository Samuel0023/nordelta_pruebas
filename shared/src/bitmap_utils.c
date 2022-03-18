#include "bitmap_utils.h"

//Bitmap reservados: lista de int
int get_first_available_bitmap(
	t_bitarray* bitmap, int processID, 
	t_list* reserved_bitmap, int FRAMES_PER_PROCESS, char* ASSIGNATION_TYPE
){
	int start = 0;
	int length = bitarray_get_max_bit(bitmap);
	if(strcmp(ASSIGNATION_TYPE, "FIJA") == 0){
		int auxiliar = 0;
		for(int d=0; d<list_size(reserved_bitmap);d++){
			int value_reserved_bitmap = (int)list_get(reserved_bitmap, d);
			if(value_reserved_bitmap == processID){
				auxiliar = d;
				break;
			}
		}
		start = auxiliar;
		length = FRAMES_PER_PROCESS;
	}

    int index = -1;
    for(int c = start; c < start + length ; c++){
        if(!bitarray_test_bit(bitmap,c)){
            index = c;
            break;
        }
    }
	if(index != -1)  bitarray_set_bit(bitmap, index);
    return index;
}