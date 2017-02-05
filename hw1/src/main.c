#include "hw1.h"

int main(int argc, char **argv) {

    FILE* in;
    FILE* out;

    /* Note: create a variable to assign the result of validargs */
    char data = validargs(argc, argv, &in, &out);
    if(data == 0)
    	{
	    	USAGE(data); //call usage if it's a zero
	    	return EXIT_FAILURE;
	    }
    char maskedBits = 0x80;
    int hFlag = (int)(data & maskedBits);
    if(hFlag == 128)
    	{
    		USAGE(data);
    		return EXIT_SUCCESS;
    	}
    maskedBits = 0x40; //1 = substitution, 0 = tut
	int sTflag = (int)(data&maskedBits);
	if(sTflag == 64) //if sub
	{
		maskedBits = 0x20;
		int encrDecr = (int)(data&maskedBits);
		maskedBits = 0x1F; //get last 5 lsb bits
	    int n = (int)(data &maskedBits);
	    if(encrDecr==32) //if decr
			shiftStringDecr(in, out, n);
		else
			shiftStringEncr(in, out, n);

	}
    else
    {
    	//assuming validargs works perfectly, then the only last outcome can be  tut
    } /*
    int c = getc(in);
    while( c !=EOF)
    {
    	putchar(c);
    	fputc(c, out);
    	c = getc(in);
    }
    fclose(in); */
    //substitutionCypher(FILE* in, FILE* out, n)
    return EXIT_SUCCESS;
}
