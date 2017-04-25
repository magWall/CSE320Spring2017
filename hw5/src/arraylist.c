#include <arraylist.h>
#include <errno.h>

/**
 * @visibility HIDDEN FROM USER
 * @return     true on success, false on failure
 */
static bool resize_al(arraylist_t* self){
    bool ret = false;
    //increase arraylist
    if(self->capacity == self->length)
    {
        //P & V & resize if alloc space allowed
    }
    //decrease arraylist
    else if( (self->capacity/2)-1 == self->length)
    {
        if( (self->capacity /2)-1 <INIT_SZ)//if less than original size, do not shrink
        {
            return false;
        }
        //if not less, lock, write new stuff, shrink, unlock
        //P
        self->capacity = (self->capacity/2);
        //V
        ret = true;
    }

    return ret;
}

arraylist_t *new_al(size_t item_size){
    void *ret = NULL;
    ret = calloc(item_size, INIT_SZ); //space for arraylist
    if( ret == NULL)
    {
        errno = ENOMEM; //when null returned, no space
        return ret;

    }
    ((arraylist_t*)ret)->item_size = item_size;
    return ret;
}

size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;

    resize_al(self);

    return ret;
}

void *get_data_al(arraylist_t *self, void *data){
    void *ret = NULL;

    return ret;
}
void *get_index_al(arraylist_t *self, size_t index){
    void *ret = NULL;

    return ret;
}

void *remove_data_al(arraylist_t *self, void *data){
    void *ret = 0;

    resize_al(self);

    return ret;
}
void *remove_index_al(arraylist_t *self, size_t index){
    void *ret = 0;

    resize_al(self);

    return ret;
}

void delete_al(arraylist_t *self, void (*free_item_func)(void*)){

    return;
}
