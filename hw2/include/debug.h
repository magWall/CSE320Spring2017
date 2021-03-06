#ifndef DEBUG_H
#define DEBUG_H
#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
	#define debug(fmt, ...) do{fprintf(stderr, "DEBUG: %s:%s:%d " fmt, __FILE__, __extension__ __FUNCTION__, __LINE__, ##__VA_ARGS__);}while(0)
#else
	#define debug(fmt, ...)													//may need to make __FUNCTION__ into __extension__ __FUNCTION__
#endif

#endif /* DEBUG_H */
