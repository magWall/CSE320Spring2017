#include "debug.h"
#include "arraylist.h"
#include "foreach.h"

pthread_t threadArr[2];

void* functA(void* vargp);


void* functA(void* vargp)
{
	//printf("Hello World\n");
	arraylist_t* tmp = new_al(sizeof(int*));
	//int a =12;
	//insert_al(tmp,&a);
	//a = 13;
	//insert_al(tmp,&a);
	for(int i=20;i<30;i++)
		insert_al(tmp,&i);

	int i=20;
	printf("%zu\n",tmp->length);
	printf("functA data20: %zu\n",get_data_al(tmp,&i));
	printf("functA %d\n",*((int*)get_index_al(tmp,100)));
	printf("functAidx0 %d\n",*((int*)get_index_al(tmp,0)));

	return NULL;
}
void* functB(void* vargp)
{
	arraylist_t* tmp = new_al(sizeof(int*));
	int a =24;
	insert_al(tmp,&a);
	a = 35;
	insert_al(tmp,&a);
	printf("%zu\n",tmp->length);
	printf("functB data %zu\n",get_data_al(tmp,&a));
	printf("functB %d\n",*((int*)get_index_al(tmp,100)));
	printf("functBidx0: %d\n",*((int*)get_index_al(tmp,0)));



//	printf("Hello World2\n");
	return NULL;
}

int main(int argc, char *argv[]){

//	unix_error("help");

	pthread_create(&threadArr[0],NULL,functA,NULL);//thread, attribute, routine, arguments
	pthread_create(&threadArr[1],NULL,functB,NULL);
	pthread_join(threadArr[0],NULL); //thread, return type
	pthread_join(threadArr[1],NULL);


	//printf(" %zu\n",tmp->length);
	//printf("%d\n",*((int*)(tmp->base)) );
	//printf("%d\n",*((int*)get_index_al(tmp,0)));
	//printf("%d\n",*((int*)get_index_al(tmp,1)));

	//free(tmp);



}
