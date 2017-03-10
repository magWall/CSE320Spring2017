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

	size_t sizeAndBlocks = size + 16;//8 bytes for header and footer, so 16 added, counted by bytes
	int padding = sizeAndBlocks%16; //add the remainder of the modulus of 16 to align it and make it divisible by 16, ex; 44, 44%16 = 4, 44+4 = 48

	sizeAndBlocks += padding;

	if(sizeAndBlocks>4*4096) //greater than maximum allotted space
	{
		errno = ENOMEM;
		return NULL;
	}
	if(freelist_head == NULL && numPages <5) //checks to see if all conditions there is a free block of memory
	{
		addrPtr = sf_sbrk(4096);
		sbrkPtr = (char*)addrPtr+4096;
		debug("addrPtr %p",addrPtr);
		debug("sbrkPtr %p",sbrkPtr);
		numPages++;
		sf_header* sfheaderPtr = (sf_header*) addrPtr;
		sfheaderPtr->alloc= 0;
		sfheaderPtr->splinter = 0;
		sfheaderPtr->block_size= 4096>>4; //shared 32 bits with alloc and splinter
		//sfheaderPtr->unused_bits ignore unused unless realloc, set0
		sfheaderPtr->requested_size=4096;
		sfheaderPtr->splinter_size=0; //originally, new page = no splinter, else is blockSize-sizeAndblocks
		sfheaderPtr->padding_size=0;

		sf_footer* sffooterPtr = (sf_footer*) ((char*)sbrkPtr-8);//get beginning of struct footer, then cast into footer
		sffooterPtr->alloc = 0;
		sffooterPtr->splinter=0;
		sffooterPtr->block_size=4096>>4;//header and footer included in block size, shared with alloc + splinter
		freelist_head = addrPtr;
	}
	else if(freelist_head== NULL && numPages >=4) //all 4 pages used, memory is full
	{
		errno = ENOMEM;
		return NULL;
	}
	sf_free_header* bestFitHeader = NULL;

	while(numPages<5 && bestFitHeader == NULL)
	{

		sf_free_header* listToLoop = freelist_head;
		while(listToLoop->next != NULL)
		{
			if(bestFitHeader == NULL)
			{
				if( ((sf_header*)listToLoop)->block_size >= sizeAndBlocks) //if fits sizeAndBlock's requirement, set to sizeAndBlocks
				{
					bestFitHeader = listToLoop;
				}
			}
			else if( ((sf_header*)listToLoop)->block_size >= sizeAndBlocks &&((sf_header*)listToLoop)->block_size < ((sf_header*)bestFitHeader)->block_size)
				bestFitHeader = listToLoop; //assign if this mem is lower than prev. free block (best fit)
			else if( ((sf_header*)listToLoop)->block_size >= sizeAndBlocks &&((sf_header*)listToLoop)->block_size == ((sf_header*)bestFitHeader)->block_size)
				if(listToLoop<bestFitHeader)//if the blocksize is the same, get lower address
					bestFitHeader =listToLoop;
			listToLoop = listToLoop->next; //go to next iteration

		}
		//if we break out of loop and bestFitHeader is still null, we must add a page and coaelsce
		if(numPages+1 <5 && bestFitHeader == NULL)
		{
			numPages++;
			addrPtr = sf_sbrk(4096);
			sbrkPtr = (char*)addrPtr +4096;
			//coaelsce here with address of last free
			sf_free_header* listToLoop = freelist_head;
			int tmpBool=0; //false not found
			while(listToLoop->next != NULL)
			{
				if( ((char*)listToLoop+ ((sf_header*)listToLoop)->block_size)+1==addrPtr )
				{
					//coaelsce if found, bool is true
					((sf_footer*)((char*)listToLoop+ ((sf_header*)listToLoop)->block_size))->block_size=0; //remove old block size
					((sf_header*)listToLoop)->block_size=(((sf_header*)listToLoop)->block_size+4096 )>>4; //add new block size
					((sf_footer*)((char*)listToLoop) + ((sf_header*)listToLoop)->block_size - 8)->block_size = ((sf_header*)listToLoop)->block_size; //update new blocksize
					((sf_footer*)((char*)listToLoop) + ((sf_header*)listToLoop)->block_size - 8)->alloc=((sf_header*)listToLoop)->alloc;
					((sf_footer*)((char*)listToLoop) + ((sf_header*)listToLoop)->block_size - 8)->splinter=((sf_header*)listToLoop)->splinter;
					tmpBool = 1;
					break;

				}
				listToLoop = listToLoop->next;
			}
			//if last block is allocated, then it won't appear in free list, then bool is false and we add new page
			if(tmpBool==0)
			{
				//add new page of memory
				sf_header* sfheaderPtr = (sf_header*) addrPtr;
				sfheaderPtr->alloc= 0;
				sfheaderPtr->splinter = 0;
				sfheaderPtr->block_size= 4096>>4; //shared 32 bits with alloc and splinter
				//sfheaderPtr->unused_bits ignore unused unless realloc, set0
				sfheaderPtr->requested_size=4096;
				sfheaderPtr->splinter_size=0; //originally, new page = no splinter, else is blockSize-sizeAndblocks
				sfheaderPtr->padding_size=0;

				sf_footer* sffooterPtr = (sf_footer*) ((char*)sbrkPtr-8);//get beginning of struct footer, then cast into footer
				sffooterPtr->alloc = 0;
				sffooterPtr->splinter=0;
				sffooterPtr->block_size=4096>>4;//header and footer included in block size, shared with alloc + splinter
				((sf_free_header*)sfheaderPtr)->next=freelist_head;
				freelist_head->prev = (sf_free_header*) sfheaderPtr;
				freelist_head = (sf_free_header*) sfheaderPtr; // move list to head

			}
		}
		else if(numPages+1>=5 && bestFitHeader == NULL) //condition cannot allow you to loop any further
			break;

	}
		if(bestFitHeader == NULL)
		{
			errno = ENOMEM; // numPages > 5 and still cannot find page for blocks given
			return NULL;
		}
		//bestFitHeader != NULL, so valid block found, now check if after calculations, if splintered, if splinter > 32,
		//try to coaelsce with other free blocks nearby, or keep it as separate free if blocks next to it are allocated

		int splinterSize =((sf_header*)bestFitHeader)->block_size - sizeAndBlocks;
		((sf_header*)bestFitHeader)->alloc = 1;
		((sf_header*)bestFitHeader)->requested_size = size;
		((sf_header*)bestFitHeader)->padding_size = padding;
		if(splinterSize>0 && splinterSize<32)
		{
			((sf_header*)bestFitHeader)->splinter_size = splinterSize;
			((sf_header*)bestFitHeader)->splinter = 1;

			sf_free_header* listToLoop = freelist_head;
			while(listToLoop->next != NULL)
			{
				if(listToLoop == bestFitHeader)
				{
					if(listToLoop->prev !=NULL)
					{
						if(listToLoop->next != NULL) // somethigng remove/modify somethnig
						{
							bestFitHeader->prev->next = bestFitHeader->next;
							bestFitHeader->next->prev = bestFitHeader->prev;
						}
						else							//something remove,  			bestFitHeader at the end of list
							bestFitHeader->prev->next = NULL;
					}
					else if(listToLoop->prev == NULL) //front
					{
						if(listToLoop->next != NULL)
						{
							listToLoop->next->prev = NULL;
							freelist_head= listToLoop->next;
						}
						else if(listToLoop->next == NULL) //only 1 element in list
						{
							freelist_head = NULL;
							break;
						}
					}
				}

					listToLoop= listToLoop->next;
			}

		}
		else if(splinterSize>32)
		{
			((sf_header*)bestFitHeader)->splinter_size = 0;
			((sf_header*)bestFitHeader)->splinter = 0;
			sf_footer* tmpAddr = ((sf_footer*) ((char*) bestFitHeader) + ((sf_header*)bestFitHeader)->block_size - splinterSize -8);
			//effectively  tmpAddr = new footer    |  code   |footer|newHeader|payloadSplinter|footer|
			tmpAddr->block_size =  ((sf_header*)bestFitHeader)->block_size-splinterSize;
			tmpAddr->splinter = 0;
			tmpAddr-> alloc = 1;
			((sf_header*)bestFitHeader)->block_size = ((sf_header*)bestFitHeader)->block_size - splinterSize; //|header payload footer|
			//block 1 finished allocation
			//beginning of second block
			tmpAddr = ((sf_footer*) (char*) bestFitHeader + ((sf_header*)bestFitHeader)->block_size +1 );
			((sf_header*)tmpAddr )-> block_size = splinterSize;
			((sf_header*)tmpAddr) -> alloc = 0;
			((sf_header*)tmpAddr) -> splinter =0;
			((sf_header*)tmpAddr) -> requested_size=0;
			((sf_header*)tmpAddr) -> padding_size = 0;
			((sf_header*)tmpAddr) -> splinter_size = 0;

			tmpAddr = (sf_footer*)((char*)tmpAddr + splinterSize - 8);
			tmpAddr -> alloc = 0;
			tmpAddr -> splinter =0;
			tmpAddr -> block_size = splinterSize;
			tmpAddr = (sf_footer*)((char*)tmpAddr - splinterSize + 8);

			sf_free_header* listToLoop = freelist_head;
			while(listToLoop->next != NULL)
			{
				if(listToLoop == bestFitHeader)
				{
					if(listToLoop->prev !=NULL)
					{
						if(listToLoop->next != NULL) // somethigng remove/modify somethnig
						{
							bestFitHeader->prev->next = (sf_free_header*)tmpAddr;
							bestFitHeader->next->prev = (sf_free_header*)tmpAddr;
							((sf_free_header*)tmpAddr)->prev = bestFitHeader->prev;
							((sf_free_header*)tmpAddr)->next = bestFitHeader->next;
						}
						else							//something remove,  			bestFitHeader at the end of list
							{
								((sf_free_header*)tmpAddr)->next = NULL;
								((sf_free_header*)tmpAddr)->prev = listToLoop->prev;
								listToLoop->prev->next = (sf_free_header*)tmpAddr;
							}
					}
					else if(listToLoop->prev == NULL) //front of list
					{
						if(listToLoop->next != NULL)
							{
								listToLoop->next->prev = (sf_free_header*)tmpAddr;
								((sf_free_header*)tmpAddr)->next = listToLoop->next;
								((sf_free_header*)tmpAddr)->prev = NULL;
								freelist_head = (sf_free_header*)tmpAddr;
							}
							else //only bestFitHeader exists
							{
								((sf_free_header*)tmpAddr)->next = NULL;
								((sf_free_header*)tmpAddr)->prev = NULL;
								freelist_head = (sf_free_header*)tmpAddr;
							}
					}
				}

					listToLoop= listToLoop->next;
			}
			//splitBlock second allocation block done
			//normally you would coaeslce, but in this case, it's not when you called a free block so this is fine
		}
		//eventually, calculate, make header + footer + padding
		//then find splinter if there is one, coaelsce with previous one
		//change values of previous freed block used if it is greater than 32 and make block smaller
		//add new allocated block to list, but when running checks, you look to see if list is free
		return  (void*)((char*) bestFitHeader + 8);

	return NULL;
}

void *sf_realloc(void *ptr, size_t size) {
	return NULL;
}

void sf_free(void* ptr) {
	return;
}

int sf_info(info* ptr) {
	//0 success, -1 failure
	return -1;
}
