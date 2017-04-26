#include "arraylist.h"

//#include "csapp.h" //wraps sem_wait and sem_pos to P and V
/**
 * @visibility HIDDEN FROM USER
 * @return     true on success, false on failure
 */
extern int errno; //from  errno.h
void unix_error(char* msg)
{
    //pass in error msg, fprintf threadsafe I think
    fprintf(stderr,"%s: %d\n",msg,errno);
}
void P(sem_t* s)
{
    //wait until value
    if(sem_wait(s)<0) // zero upon call since decrement
        unix_error("Cannot P properly");

}
void V(sem_t* s)
{
    //after done
    if(sem_post(s)<0)
        unix_error("Cannot V properly");
}
static bool resize_al(arraylist_t* self){
    sem_t mutex;
    sem_init(&mutex, 0,1); //set mutex to 1
    bool ret = false;
    //increase arraylist
    if(self->capacity == self->length)
    {
        P(&mutex);
        //P & V & resize if alloc space allowed
        self->capacity = (self->capacity*2);
        V(&mutex);
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
        P(&mutex);
        self->capacity = (self->capacity/2);
        V(&mutex);
        //V
        ret = true;
    }

    return ret;
}

arraylist_t *new_al(size_t item_size){
    void *ret = NULL;
    sem_t mutex;
    sem_init(&mutex,0,1);
    ret = malloc(sizeof(arraylist_t*)); //space for arraylist
    if( ret == NULL)
    {
        errno = ENOMEM; //when null returned, no space
        return ret;
    }
    //P
    P(&mutex);
    ((arraylist_t*)ret)->item_size = item_size;
    ((arraylist_t*)ret)->capacity = INIT_SZ;
    ((arraylist_t*)ret)->base = calloc(INIT_SZ,item_size);
  //  printf("%zu \n",((arraylist_t*)ret)->capacity);
 //   printf("%zu \n",((arraylist_t*)ret)->capacity);

    V(&mutex);
    //V
    return ret;
}

size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;
    sem_t mutex;
    sem_init(&mutex,0,1);
    P(&mutex);//modifying values P and V
    memcpy(((void*)(self->base+ (self->length *self->item_size))),data,
        self->item_size);
    self->length+=1;
    V(&mutex);
    if(self->capacity == self->length)
        resize_al(self);

    return ret;
}

size_t get_data_al(arraylist_t *self, void *data){
    size_t ret = 0;

    return ret;
}

void *get_index_al(arraylist_t *self, size_t index){
    void *ret = NULL;

    return ret;
}

bool remove_data_al(arraylist_t *self, void *data){
    bool ret = false;

    resize_al(self);

    return ret;
}

void *remove_index_al(arraylist_t *self, size_t index){
    void *ret = 0;

    resize_al(self);

    return ret;
}

void delete_al(arraylist_t *self, void (*free_item_func)(void*)){
    //for items in list, free
    //free list


    return;
}
