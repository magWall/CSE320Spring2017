#include "hw1.h"

// For your helper functions (you may add additional files also)
// DO NOT define a main function here!
int argSize(char* ptr);
int alphabetSize();

char validargs(int argc, char** argv, FILE** in, FILE** out) {
	char ret = 0;
/*
if invalid argument,  return 0
if invalid argument order, return 0,
if error opening input or output file, return 0

*/

/*test for if -h, else test for if 5 or 6, then test for combinations*/

int i = argSize(*(argv+1)); // argSize funct = length of argument used to check if this is -h with nothing trailing behind


printf("kappa, i = %d\n",i);
printf("Args: %d\n",argc);
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
			ret+=0;			//third bit is 0, don't really do anything, placeholder code here for reading purposes
		}
		else
		{
			ret = 0;
			return ret;
					//the other arguments are invalid if it isn't -d or -e and thus throws error
		}
		if( argc ==6)// SOMETHING WENT WRONG HERE
		{
			arg6 = **(argv+5); //de-reference once to get n  ***MIGHT WANT TO RECHECK THIS
			printf("argv+5 %p\n",(void*)(argv+5));
			printf("(*(argv+5)) %s\n",(*(argv+5)));
		}
		printf("alphabetSize %d Arg6 %d\n",alphabetSize(), arg6);
		int retDec = arg6 % alphabetSize(); //take the alphabetsize, and mod that with n (which is arg6, 320 default, else n)
		printf("retDec %d\n",retDec);

		ret = ret | (char)retDec;

	}
	else if(argc ==5 && *(*(argv+1))=='-' && *((*(argv+1))+1) =='t') //check t
	{
		if( *(*(argv+2))=='-' && *((*(argv+2))+1) =='d')
		{
			ret= ret|0x20;
			ret = ret |0x1;	//last 5 lsb must be non-zero
		}
		else if( *(*(argv+2))=='-' && *((*(argv+2))+1) =='e')
		{
			ret+=0; //third bit is 0, don't really do anything, placeholder code here for reading purposes
			ret = ret |0x1;	//last 5 lsb must be non-zero
		}
		else
		{
			ret = 0;
			return ret;
					//the other arguments are invalid if it isn't -d or -e and thus throws error
		}
	}
	else
		{
			ret = 0;
			return ret;
					//the other arguments are invalid (not -s ||-t) and thus throws error
		}


	//open files, if inputfile cannot be opened, fail, if outputfile DNE, create, else overwrite it, if special value stdout / stdin
/*
	if( *(*(argv+4)) =='-')
	{
		*in = stdin;
	}
	else
		*in = fopen( (*(argv+4)),"r"); //infile

	if( *(*(argv+5)) =='-')
	{
		*out = stdout;
	}
	else
		*out = fopen( (*(argv+5)), "w");	//outfile
*/

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

