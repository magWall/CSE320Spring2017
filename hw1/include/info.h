/*#ifndef INFO_H
#define INFO_H
#include "const.h"

#ifdef DINFO
#define INFO(shiftedAlphabet, n, inputfile,outputfile,operation) do
{fprintf(stderr, 														\
	"INFO: shifted alphabet: %s\n"										\
	"INFO: shift amount: %d\n"											\
	"INFO: input file: %s\n"											\
	"INFO: output file: %s\n"											\
	"INFO: operation: %s\n"												\
	,shiftedAlphabet, n, inputfile, outputfile, operation);

} while (0);
#else
	#define INFO(shiftedAlphabet, n, inputfile,outputfile,operation)


#endif*/
