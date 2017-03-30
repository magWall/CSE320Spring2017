#include <sfish.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

int isValidCmd(char* cmd){
if(strcmp(cmd,"cd")==0)
	return 1;
else if(strcmp(cmd,"help")==0)
	return 2;
else if(strcmp(cmd,"pwd") ==0)
	return 3;
return -1;
}
void unix_error(char* msg)
{
	fprintf(stderr, "%s:%s\n", msg, strerror(errno));
	exit(0);
}
pid_t Fork(void)
{
	pid_t pid;
	if( (pid = fork())<0)
	{
		unix_error("fork error");
	}
	return pid;
}

void cmdCd()
{

}
void cmdHelp()
{
	pid_t pid =Fork();
	int status;
	if(pid ==0)
	{

		fprintf(stdout,"%s",
	    "Kenny's Shell v.failure.beta \n"
		"These shell commands are built-in.  Type `help' to see this list.\n"
		"cd [-L|[-P [-e]] [-@]] [dir]      ->change directory\n "
		"pwd [-LP];  ->Print working directory\n"
		"exit [n]    ->Exits the Shell\n"
		);
		exit(EXIT_SUCCESS);
	}
	pid = wait(&status);


}
void cmdPwd()
{}
/*
{
	char*outputBuffer;
	long size = 512;
	char* outputPtr = output;
	pid_t pid;
	while( (pid=fork())==0 )
	{
		if( (outputBuffer=malloc((size_t)size))!=NULL)
		{
			outputPtr =getcwd(outputBuffer,size); //get working directory, put into outputPtr
		}
		wait()
	}


}*/

char** strSplit(char* line, const char delimiter) //split string into str array, or char** and malloc.
{
	int outputBufferSize = 256; //set a good buffer size
	char** tmpResult = malloc(sizeof(char*)*outputBufferSize);
	if(tmpResult == NULL) //failed to malloc
	{
		printf("FAILED TO MALLOC WHEN PARSING.");
		exit(1); //I believe 1 is for failure, 0 is EXIT_SUCCESS
	}
	char* tokenDelimiter = NULL; //for strTok
	*tokenDelimiter = delimiter;
	*(tokenDelimiter+1)=0; //null character to make it only grab delimiter
	char* word;
	int tmpCount=0;

	word = strtok(line,tokenDelimiter);//may have to check for delimiter same spaces
	if(word!=NULL && strlen(word)!=0)
	{
		*(tmpResult+tmpCount)=word;
		tmpCount++;
	}

	while(word!=NULL)
	{
		if(tmpCount>outputBufferSize) //increase size of malloc
		{
			outputBufferSize+= 256;
			tmpResult= realloc(tmpResult,sizeof(char*)*outputBufferSize);
			if(tmpResult ==NULL)
			{
				printf("FAILED TO REALLOC WHEN PARSING.");
				exit(1); //I believe 1 is for failure, 0 is EXIT_SUCCESS
			}
		}

		word = strtok(NULL,tokenDelimiter);
		*(tmpResult+tmpCount)=word;
		tmpCount++;
	}
	return tmpResult;

}
//return error





