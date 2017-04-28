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
   // printf("waiting in P\n");
    if(sem_wait(s)<0) // zero upon call since decrement
        unix_error("Cannot P properly");
   // printf("done\n");

}
void V(sem_t* s)
{
    //after done
    if(sem_post(s)<0)
        unix_error("Cannot V properly");
}
static bool resize_al(arraylist_t* self){
    bool ret = false;
    void* tmpPtr;
    //increase arraylist
    if(self->capacity == self->length)
    {

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
        self->capacity = (self->capacity/2);
        tmpPtr = realloc(self->base, self->capacity*self->item_size);

        //V
        if(tmpPtr == NULL)
        {
            errno = ENOMEM;
            unix_error("Realloc failed, out of memory");
            ret = false;
            return ret;
        }
        self->base = tmpPtr; //this must be locked

        ret = true;
    }

    return ret;
}

arraylist_t *new_al(size_t item_size){
    arraylist_t *ret = NULL;
    ret = malloc(sizeof(arraylist_t)); //space for arraylist
    if( ret == NULL)
    {
        errno = ENOMEM; //when null returned, no space
        unix_error("out of memory");
        return ret;
    }
    //P
    sem_init(&ret->mutex,0,1);

    // P(&((arraylist_t*)ret)->mutex);
    ((arraylist_t*)ret)->item_size = item_size;
    ((arraylist_t*)ret)->capacity = INIT_SZ;
    void* tmpPtr= calloc(INIT_SZ,item_size);
    if(tmpPtr == NULL)
    {
        errno = ENOMEM;
        unix_error("out of memory");
        // V(&((arraylist_t*)ret)->mutex); //gotta close mutex here
        return ret;
    }
    ((arraylist_t*)ret)->base = tmpPtr;
 //   printf("MemAddrret: %p\n",((arraylist_t*)ret)->base);

  //  printf("%zu \n",((arraylist_t*)ret)->capacity);
 //   printf("%zu \n",((arraylist_t*)ret)->capacity);
    // V(&((arraylist_t*)ret)->mutex); //if reach here, close
    //V
    return ((arraylist_t*)ret);
}

size_t insert_al(arraylist_t *self, void* data){
    size_t ret = UINT_MAX;
    bool tmpBool = false;

 //   printf("waiting\n");
    P(&self->mutex);
//    printf("dec mutex\n");
    if(self->capacity == self->length)
        tmpBool = resize_al(self); //false when out of mem

  //  size_t tmpVal = self->length;
  //  size_t tmpCap = self->capacity;

    if(self->length < self->capacity)
    {
    //modifying values P and V
        memcpy((char*)self->base+ (self->length *self->item_size),data,
            self->item_size);
        self->length+=1;
        ret = self->length;
        tmpBool = true;
        V(&self->mutex); //mutex doesnt work
        return ret;
    }
    //capacity same as lengths, some error
   // reach here if(tmpBool==false)
    V(&self->mutex);    //mutex doesnt work

    if(tmpBool==false)
    {
        errno= ENOMEM;
        unix_error("out of memory to insert?");
    }
    //printf("exit\n");
    return ret;
}

size_t get_data_al(arraylist_t *self, void *data){
    size_t ret = 0;
    if(data == NULL)
        return ret;
    ret = UINT_MAX; //set -1 to be out of idx
    P(&self->mutex);
    int num = self->length;
    for(int i=0;i<num;i++)
    {
        if(memcmp((char*)self->base+(i*self->item_size),data,self->item_size)==0)
            ret = i;
    }
    V(&self->mutex);
    if(ret == UINT_MAX)//not in list
    {
        errno= ENODATA;//
        unix_error("Not in list");
    }

    return ret;
}

void *get_index_al(arraylist_t *self, size_t index){
    P(&self->mutex);
    void *ret= malloc(self->item_size);

    if(ret==NULL)
    {
        errno=ENOMEM;
        unix_error("No memory left to return idx");
        V(&self->mutex);
        return NULL;
    }

    if(self->length < index)//idx+1 == length if they  are equal
    {
        //length == current num elements, but idx starts at 0
        memcpy(ret, (char*)self->base+((self->length-1)*self->item_size),self->item_size);
        V(&self->mutex);
        return ret;
    }
    //self->length > idx
    memcpy(ret, (char*)self->base+(index*self->item_size),self->item_size);
    V(&self->mutex);
    return ret;
}

bool remove_data_al(arraylist_t *self, void *data){
    bool ret = false;

     if(data == NULL)
    {
        P(&self->mutex);
        if(self->length==0)
        {
            V(&self->mutex);
            return false;//cannot remove blank
        }
        else if(self->length==1)//1 element in list
        {
            memset(self->base,0,self->item_size);
            self->length-=1;
            V(&self->mutex);
            return true;
        }
        memmove(self->base, (char*)self->base+self->item_size,(self->length-1)*self->item_size);
        self->length-=1;
        if(self->length==(self->capacity/2) -1)
            resize_al(self);
        V(&self->mutex);
        return true;
        //moves everything besides element 0 , takes dest, item_size+dest, shift by all items-1*item_size
        //whe NULL, remove first element
        //memcpy everything but first element over to base
    }
    size_t idx = get_data_al(self,data);
    if(idx ==UINT_MAX)
    {
        return false;
    }
    P(&self->mutex);
    if(idx+1 == self->length)//last element to remove, just memset that
    {
        memset((char*)self->base+idx*self->item_size,0,self->item_size);
        self->length-=1;
        if(self->length==(self->capacity/2) -1)
            resize_al(self);
        V(&self->mutex);
        return true;
    }
    //less than last element, between elements
    memmove((char*)self->base+idx*self->item_size,(char*)self->base+(idx+1)*self->item_size,(self->length-(idx+1))*self->item_size);
    self->length-=1;
    ret= true;
    if(self->length==(self->capacity/2) -1)
        resize_al(self);
    V(&self->mutex);
    return ret;
}

void *remove_index_al(arraylist_t *self, size_t index){
    P(&self->mutex);
    // P on top, reading data into malloc
    void *ret = malloc(self->item_size);
    if(ret == NULL)
        {
            errno = ENOMEM;
            unix_error("out of memory,cannot return value");
            V(&self->mutex);
            return NULL;
        }
    if(self->length==0)//failure condition
        {
            errno = EINVAL; // invalid idx =0
            unix_error("Arraylist empty, cannot remove blank idx");
            V(&self->mutex);
            return NULL;
        }
        //check if length >0,
    if(index>self->length)//if index greater than length, remove last element
    {
        //if length ==0, return null

        memcpy(ret,(char*)self->base+(self->length-1)*self->item_size, self->item_size);
        memset((char*)self->base+(self->length-1)*self->item_size,0,self->item_size);
        self->length-=1;
        if(self->length==(self->capacity/2) -1)
            resize_al(self);
        V(&self->mutex);
        return ret;
    }
    //idx in between length
    if(index+1 == self->length)//last element
    {
        memcpy(ret,(char*)self->base+(index*self->item_size), self->item_size);
        memset((char*)self->base+(index)*self->item_size,0,self->item_size);
        self->length-=1;
        if(self->length==(self->capacity/2) -1)
            resize_al(self);
        V(&self->mutex);
        return ret;
    }
    memcpy(ret,(char*)self->base+(index*self->item_size), self->item_size);
    memmove((char*)self->base+(index)*self->item_size,(char*)self->base+(index+1)*self->item_size,(self->length-(index+1))*self->item_size);
    self->length-=1;

    if(self->length==(self->capacity/2) -1)
        resize_al(self);

    V(&self->mutex);
    return ret;
}

void delete_al(arraylist_t *self, void (*free_item_func)(void*)){
    //for items in list, free
    //free list


    return;
}
