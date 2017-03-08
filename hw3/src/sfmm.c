/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <debug.h>
/**
 * You should store the head of your free list in this variable.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
sf_free_header* freelist_head = NULL;
void* addrPtr = NULL; // addressPointer at the beginning of heap
void* sbrkPtr = NULL;
int numPages = 0;
void *sf_malloc(size_t size) {
	if(size==0)
	{
		errno = EINVAL; // when 0 , throw this
		return NULL;
	}

	if(freelist_head == NULL)
	{
		addrPtr = sf_sbrk(4096);
		sbrkPtr = (char*)addrPtr+4096;
		debug("addrPtr %p",addrPtr);
		debug("sbrkPtr %p",sbrkPtr);
		numPages++;
	}
	size_t sizeAndBlocks = size + 16;//8 bytes for header and footer, so 16 added

	if(sizeAndBlocks>=numPages*4096 || ((char*)addrPtr+sizeAndBlocks>= (char*)sbrkPtr && numPages<4) )
	{

	}
	else if(sizeAndBlocks<4096 && (int*)addrPtr+sizeAndBlocks<= (int*)sbrkPtr) //write into page
	{

	}
	//if not enough space to add to list, check if sfsbrk goes over 4 pages
	// if not over 4 pages, then add the sfbrk page(s) you need to the curr pages and coaelsce,
	//then malloc
	else //else >5 , invalid case
	{
		errno = ENOMEM;// ERRNO is set to ENOMEM on valid

	}
	return NULL;
}

void *sf_realloc(void *ptr, size_t size) {
	return NULL;
}

void sf_free(void* ptr) {
	return;
}

int sf_info(info* ptr) {
	return -1;
}
