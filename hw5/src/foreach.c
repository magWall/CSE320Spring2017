#include "debug.h"
#include "arraylist.h"
#include "foreach.h"

void *foreach_init(arraylist_t *items){
    void *ret = NULL;
   // pthread_t someThread;
   // pthread_create(&someThread, NULL, foreach_next, NULL);
    //pthread_key k= pthread_key_create();
    //pthread_setspecific(k,items->curr_idx);

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
    //all data here is read-write, and cheaper to call P and V once
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

    //error case returns UINT_MAX

    return ret;
}

bool foreach_break_f(){
    bool ret = true;
    //always true
    /* clean up loop */

    return ret;
}
#define foreach_break if (foreach_break_()) break

int apply(arraylist_t *items, int (*application)(void*)){
    int ret = 0;

    return ret;
}
