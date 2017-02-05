#include "hw1.h"

// For your helper functions (you may add additional files also)
// DO NOT define a main function here!
int argSize(char* ptr);
int alphabetSize();
int isNum(char t); //checks if char between ascii 48 to 57, 0 is false, 1 is true
int findAlphabet(char c); //finding alphabet position, -1 if not existent
char alphabetLowerToUpperCase(char lower);//converts lower to uppercase if it's a lowercase
int isValidArg6(char* ptr); //checks char array to see all ascii betwene 48 to 57, 0 is false, 1 is true
int asciiToDecimal(char* ptr); //takes in char arr

char validargs(int argc, char** argv, FILE** in, FILE** out) {
	char ret = 0;
/*
if invalid argument,  return 0
if invalid argument order, return 0,
if error opening input or output file, return 0

*/

/*test for if -h, else test for if 5 or 6, then test for combinations*/

int i = argSize(*(argv+1)); // argSize funct = length of argument used to check if this is -h with nothing trailing behind


//printf("kappa, i = %d\n",i);
//printf("Args: %d\n",argc);
int arg6 = 320;

if(argc == 5 || argc == 6)
{
	//more code to check combinations, accept
	/*$ bin/hw1 -s -e infile outfile
$ bin/hw1 -s -d infile outfile
$ bin/hw1 -s -e infile outfile n
$ bin/hw1 -s -d infile outfile n
$ bin/hw1 -t -e infile outfile
$ bin/hw1 -t -d infile outfile*/
	if( *(*(argv+1))=='-' && *((*(argv+1))+1) =='s') //check s
	{
		ret = ret|0x40;		//second bit becomes 1, or them together

		if( *(*(argv+2))=='-' && *((*(argv+2))+1) =='d')
		{
			ret = ret|0x20;	//third bit becomes 1, or the values
		}
		else if( *(*(argv+2))=='-' && *((*(argv+2))+1) =='e')
		{
			ret = ret|0x0;	//third bit is 0, don't really do anything, placeholder code here for reading purposes
		}
		else
		{
			ret = 0;
			return ret;
					//the other arguments are invalid if it isn't -d or -e and thus throws error
		}
		if( argc ==6)		//MIGHT NEED TO CHECK FOR INVALIC CHARACTERS
		{
			if( isValidArg6(*(argv+5)) == 0 ) //not valid arg (not a numeric) for arg6 if isValidArg6 returns 0
			{
				ret =0;
				return ret;
			}

			arg6 = asciiToDecimal(*(argv+5)); //de-reference once to get n  ***MIGHT WANT TO RECHECK THIS

//			printf("argv+5 %p\n",(void*)(argv+5));
//			printf("(*(argv+5)) %s\n",(*(argv+5)));
		}
	//	printf("alphabetSize %d Arg6 %d\n",alphabetSize(), arg6);
		int retDec = arg6 % alphabetSize(); //take the alphabetsize, and mod that with n (which is arg6, 320 default, else n)
	//	printf("retDec %d\n",retDec);

		ret = ret | (char)retDec;

	}
	else if(argc ==5 && *(*(argv+1))=='-' && *((*(argv+1))+1) =='t') //check t
	{
		if( *(*(argv+2))=='-' && *((*(argv+2))+1) =='d')
		{
			ret= ret|0x20;

		}
		else if( *(*(argv+2))=='-' && *((*(argv+2))+1) =='e')
		{
			ret+=0; //third bit is 0, don't really do anything, placeholder code here for reading purposes
		}
		else
		{
			ret = 0;
			return ret;
					//the other arguments are invalid if it isn't -d or -e and thus throws error
		}
			int retDec = arg6 % alphabetSize();

			ret = ret |(char)retDec;	//last 5 lsb must be non-zero

	}
	else
		{
			ret = 0;
			return ret;
					//the other arguments are invalid (not -s ||-t) and thus throws error
		}

	//open files, if inputfile cannot be opened, fail, if outputfile DNE, create, else overwrite it, if special value stdout / stdin

	if( *(*(argv+3)) =='-')
	{
		*in = stdin;
	}
	else
		*in = fopen( (*(argv+3)),"r"); //infile

	if( *(*(argv+4)) =='-')
	{
		*out = stdout;
	}
	else
		*out = fopen( (*(argv+4)), "w");	//outfile


}
else if(i==2 && *(*(argv+1))=='-' && *((*(argv+1))+1) =='h')
{
	 ret = ret|0x80; // 1000 0000 = 0x80

}
else ret = 0;


/*printf("File Pointer %p\n",(FILE)**in);*/




	/*void* tmp_ptr = &argv;
	int i;
	for (i=0;i<argc;i++)
	{
		(void*)tmp_ptr++;
		printf(tmp_ptr);
	}*/
	/* code here */

	return ret;
}

int isNum(char t)		//checks if char between ascii 48 to 57, 0 is false, 1 is true
{
	if( (t - 48)>=0 && (t-48)<=9)
		return 1;
	return 0;
}
//check if character is in the alphabet array, if so, return the position of the char, else return -1. Assumes character is case-sensitive

int findAlphabet(char c)
{
	int i=0;
	while( *(Alphabet+i) !=0)
	{
		if (c == *(Alphabet+i) )		//essentially if  A[i] == c
			return i;
		i++;
	}
	return -1;
}
void shiftStringDecr(FILE* in, FILE* out, int n) //alphabet shift to right, strings shift left
{
	int c = getc(in);
	int lengthofAlphabet = alphabetSize();
	while( c!= EOF)
	{
		c = alphabetLowerToUpperCase((char)c); //change to uppercase
		int alphabetPos = findAlphabet((char)c);
		if(alphabetPos != -1)
		{
			if((alphabetPos-n)%lengthofAlphabet >=0) //if parsing doesn't go negative, keep the equation as is
				c =  *((Alphabet)+((alphabetPos-n)%lengthofAlphabet) );
			else
				c =  *((Alphabet)+lengthofAlphabet+((alphabetPos-n)%lengthofAlphabet) ); //<=0, accomodate by adding the length to make the change positive
		}
		//putchar(c);//print to terminal
		fputc(c, out); //write to stream
		c = getc(in);
	}
	fclose(in);
}
void shiftStringEncr(FILE* in, FILE* out, int n)   //assuming it's the alphabet array that should be shifted, and strings just  take char positions
{
	int c = getc(in);
	int lengthofAlphabet = alphabetSize();
	while( c!= EOF)
	{

		c = alphabetLowerToUpperCase((char)c); //change to uppercase
		int alphabetPos = findAlphabet((char)c);

		if(alphabetPos != -1)
		{
			c =  *((Alphabet)+((alphabetPos+n)%lengthofAlphabet) );
		}
		//putchar(c);//print to terminal
		fputc(c, out); //write to stream
		c = getc(in);
	}
	fclose(in);
	//scan file or whatnot

	//take the in ptr's address, loop and see if it's part of the alphabet, if it is , shift left by taking the position of that char in arr+n %alphabetsize
	//else leave it alone and store keep it in the next char, then write that new char array into output file
}
int isValidArg6(char* ptr) //checks char array to see all ascii betwene 48 to 57, 0 is false, 1 is true
{
	int i=0;
	while(*((ptr)+i)!=0)
	{
		if( isNum(*((ptr)+i)) ==0)
			return 0;
		i++;
	}
	return 1;
}
int asciiToDecimal(char* ptr) //takes in char arr
{
	int i = 1;
	int tmp = ((int)(*((ptr)+0)))-48; //point to first char in the arr,take it's ascii value, then -48 to get 0-9
	while( *((ptr)+i) != 0)	//atoui??? while  arr does not terminate
	{
		tmp*=10;			//increment, so if char arr = 55, 5 * 10 , then add 5
		tmp+=((int)(*((ptr)+i)))-48;
		i++;
	}
	return tmp;

}
//probably do not need this function, piazza just said read buffer + immediately output
void convertLowerAlphabetChars(char* ptr)		//may need to check this out, potentially erroronous with pass by ref, not entirely sure
{
	int i=0;
	while ( *((ptr)+i) != 0)
		*((ptr)+i) = alphabetLowerToUpperCase(*((ptr)+i));
}
char alphabetLowerToUpperCase(char lower)
{
	if(lower >= 97 && lower <=122)
		return (lower - 32);		//if character is an lower case (97 =a, 122 = z), subtract 32 to make it upper case, else ignore
	return lower;
}
//return size of alphabet
int alphabetSize()
{
	int i =0;
	while(*(Alphabet+i) != 0)
		i++;
	return i;
}

// argSize funct = length of argument used to check if this is -h with nothing trailing behin
int argSize(char* ptr)
{
	int tmp =0;
	while( *((ptr)+tmp) != 0 )
	{
		//	printf("char at this place is: %c\n",*((*(argv+1))+tmp));
		tmp++;
	}
	return tmp;
}

