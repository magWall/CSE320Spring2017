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
    void* tmpPtr;
    //increase arraylist
    if(self->capacity == self->length)
    {
        P(&mutex);
        //P & V & resize if alloc space allowed
        self->capacity = (self->capacity*2);
        if((tmpPtr = realloc(self->base, self->capacity*self->item_size)
         ) == NULL)
        {
            errno = ENOMEM;
            unix_error("Out of memory");
            ret = false;
            return ret;
        }
        self->base = tmpPtr;
        V(&mutex);
            ret = true;

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
        tmpPtr = realloc(self->base, self->capacity*self->item_size);

        //V
        if(tmpPtr == NULL)
        {
            errno = ENOMEM;
            unix_error("Realloc failed, out of memory");
            ret = false;
            V(&mutex);//end mutex if it goes into error
            return ret;
        }
        self->base = tmpPtr; //this must be locked
        V(&mutex);

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
        unix_error("out of memory");
        return ret;
    }
    //P
    P(&mutex);
    ((arraylist_t*)ret)->item_size = item_size;
    ((arraylist_t*)ret)->capacity = INIT_SZ;
    void* tmpPtr= calloc(INIT_SZ,item_size);
    if(tmpPtr == NULL)
    {
        errno = ENOMEM;
        unix_error("out of memory");
        V(&mutex); //gotta close mutex here
        return ret;
    }
    ((arraylist_t*)ret)->base = tmpPtr;
    //printf("MemAddrret: %p\n",((arraylist_t*)ret)->base);

  //  printf("%zu \n",((arraylist_t*)ret)->capacity);
 //   printf("%zu \n",((arraylist_t*)ret)->capacity);
    V(&mutex);
    //V
    return ((arraylist_t*)ret);
}

size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;
    bool tmpBool = false;

    if(self->capacity == self->length)
        tmpBool = resize_al(self); //false when out of mem

    sem_t mutex;
    sem_init(&mutex,0,1);

    if(self->length < self->capacity)
    {
        P(&mutex);//modifying values P and V
        memcpy((char*)self->base+ (self->length *self->item_size),data,
            self->item_size);
        self->length+=1;
        tmpBool = true;
        V(&mutex);
        return self->length;
    }
    //capacity same as lengths, some error
   // reach here if(tmpBool==false)
    if(tmpBool==false)
    {
        errno= ENOMEM;
        unix_error("out of memory to insert?");
    }
    return ret;
}

size_t get_data_al(arraylist_t *self, void *data){
    size_t ret = 0;
    if(data == NULL)
        return ret;
    sem_t mutex;
    ret = -1; //set -1 to be out of idx
    sem_init(&mutex,0,1);
    int num = self->length;
    for(int i=0;i<num;i++)
    {
        P(&mutex);
        if(memcmp((char*)self->base+(i*self->item_size),data,self->item_size)==0)
            ret = i;
        V(&mutex);
    }
    if(ret == -1)//not in list
    {
        errno= ENODATA;//
        unix_error("Not in list");
    }

    return ret;
}

void *get_index_al(arraylist_t *self, size_t index){
    void *ret= malloc(self->item_size);
    if(ret==NULL)
    {
        errno=ENOMEM;
        unix_error("No memory left to return idx");
        return NULL;
    }
    if(self->length < index)//idx+1 == length if they  are equal
    {
        //length == current num elements, but idx starts at 0
        memcpy(ret, (char*)self->base+((self->length-1)*self->item_size),self->item_size);
        return ret;
    }
    //self->length > idx
    memcpy(ret, (char*)self->base+(index*self->item_size),self->item_size);
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
