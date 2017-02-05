#include "hw1.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;

    /* Note: create a variable to assign the result of validargs */
    char data = validargs(argc, argv, &in, &out);
    if(data == 0)
    	return EXIT_FAILURE;
   // char maskedBits = 0x1F;
   // int n = (int)(data &maskedBits);
    int c = getc(in);
    while( c !=EOF)
    {
    	putchar(c);
    	fputc(c, out);
    	c = getc(in);
    }
    fclose(in);
    //substitutionCypher(FILE* in, FILE* out, n)
    return EXIT_SUCCESS;
}
