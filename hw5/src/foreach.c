#include "debug.h"
#include "arraylist.h"
#include "foreach.h"

void *foreach_init(arraylist_t *items){
    void *ret = NULL;
    //my semaphores are already initalized when I create arraylist
    P(&items->mutex);
    if(items->length ==0)
        return NULL;
    ((arraylist_t*)ret)->curr_idx = 0;//initalized
    ret = get_index_al(items,0);
    V(&items->mutex);
    return ret;
}

void *foreach_next(arraylist_t *items, void* data){
    void *ret = NULL;
    P(&items->mutex);
    if(data == NULL)
    {
        items->curr_idx+=1;
        if(items->curr_idx<items->length)
            ret = get_index_al(items,items->curr_idx);
        else if(items->curr_idx>=items->length)//out of bounds
            ret = NULL;
        //do not update value, grab next item in list
        V(&items->mutex);
        return ret;
    }
    //modify since data is not null
    memcpy((char*)items->base+(items->curr_idx*items->item_size),data,
        items->item_size);//replace dest with new data
    items->curr_idx+=1;
    if(items->curr_idx<items->length)
        ret = get_index_al(items,items->curr_idx);
    else if(items->curr_idx>=items->length)//out of bounds
         ret = NULL;

    V(&items->mutex);
    return ret;
}

size_t foreach_index(){
    size_t ret = 0;

    return ret;
}

bool foreach_break_f(){
    bool ret = false;

    return ret;
}

int32_t apply(void *self, int32_t (*application)(void*)){
    int32_t ret = 0;

    return ret;
}
