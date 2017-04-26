#include "debug.h"
#include "arraylist.h"
#include "foreach.h"

int main(int argc, char *argv[]){

	unix_error("help");

	arraylist_t* tmp = new_al(sizeof(int*));
	//int a =12;
	//insert_al(tmp,&a);
	//printf(" %zu\n",tmp->length);
	//printf("%d\n",*((int*)(tmp->base)) );
	free(tmp);


}
