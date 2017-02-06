#ifndef INFO_H
#define INFO_H
#include "const.h"
#define

#ifdef DINFO
#define INFO(shiftedAlphabet, n, inputfile,outputfile,operation,STfFlag) do
{	fprintf(stderr, 														\
	"INFO: shifted alphabet:");												\
/*	int i=0;																\
	while(*(Alphabet+i)!=0 )												\
	{																		\
		printf("%s\n", );													\
		i++;																\
	}*/																		\
	fprintf(stderr, 														\
	"\n"																	\
	"INFO: shift amount: %d\n"											\
	"INFO: input file: %s\n"											\
	"INFO: output file: %s\n"											\
	"INFO: operation: %s\n"												\
	,n, inputfile, outputfile, operation);

} while (STfFlag==64)
#else
	#define INFO(shiftedAlphabet, n, inputfile,outputfile,operation,STfFlag)


#endif
