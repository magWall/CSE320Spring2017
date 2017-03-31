#include <sfish.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <debug.h>
#include <dirent.h>

int isValidCmd(char** words){
char* cmd = *words;
if(strcmp(cmd,"cd")==0)
	return 1;
else if(strcmp(cmd,"help")==0) //only 1 argument
{
	if( *(words+1)!= NULL)
		return -1;
	return 2;
}

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

void cmdCd(char** words)
{

	if(*(words+1)!=NULL)
	{
		if(  strcmp(*(words+1),"-")==0 ) //go back prev directory
		{
			char* oldpwdPath = getenv("OLDPWD");
			char* pwdPath = getenv("PWD");
			chdir(oldpwdPath);
			setenv("PWD",oldpwdPath,1); //overwrite prev. working directory.
			setenv("OLDPWD",pwdPath,1);

		}	//
		else if(strncmp(*(words+1),"..",2)==0)
		{
			//go back one path and check if file exists
			char* pwdPath = getenv("PWD");
			debug("PWD: %s\n",pwdPath);
			char* splitToken= "/";
			char** returnedPath = strSplit(pwdPath,splitToken);
			int idx=1; //can't go back to prev directory if there's no previous directory.
			char buffer[256];
			buffer[0]= 0; //make buffer set to 0 so strcat will not be affected.
			char* newPath = buffer; //    0 1 2 3
			strcat(newPath,"/");
			strcat(newPath, *returnedPath);
			debug("newPath: %s\n",newPath);

			while( *(returnedPath+idx+1)!=NULL)
			{
				debug("path:%s\n",*(returnedPath+idx));
				strcat(newPath,"/");
				strcat(newPath,*(returnedPath+idx));
			//	debug("newPath: %s\n",newPath);
				idx++;
			}
			chdir(newPath);
			setenv("PWD",newPath,1);//overwrite old path
			debug("newPath final:%s\n",newPath);
			debug("PWD: %s\n",getenv("PWD"));
		}
		else if( strncmp(*(words+1),".",1)==0 ) //current directory
		{

			//read whole path, if file exist, go to that path
		}	//

		else
		{
			char* pwdPath = getenv("PWD");
			strcat(pwdPath,"/");
			strcat(pwdPath,*(words+1));
			DIR* tmpDir = opendir(pwdPath);
			if(tmpDir!=NULL)
			{
				closedir(tmpDir);
				chdir(pwdPath);
				char* oldpwdPath = getenv("PWD");
				setenv("PWD",pwdPath,1);
				setenv("OLDPWD",oldpwdPath,1);
				//debug("PWD: %s\n",getenv("PWD"));
			}
			else if(ENONET == errno)
			{
				perror("Invalid directory");
			}
			//same as  . , check if there's a path to there
			//check if path exists to there, if it does, change to that directory
		}
	}
	else // it's just cd
	{
		char* pwdPath = getenv("PWD");
		debug("PWD: %s\n",pwdPath);
		char* envirHome = getenv("HOME"); //path to home if this directory
		chdir(envirHome);//change directory to home
		setenv("PWD",envirHome,1); //1 to overwrite
		pwdPath = getenv("PWD");
		debug("PWD(home): %s\n",pwdPath);
		char* user = getenv("USER");
		debug("user: %s\n",user);
		char* term = getenv("TERM");
		debug("term: %s\n",term);


	}
	/*debug("Home: %s\n",envirHome);
	char* pwdPath = getenv("PWD");
	debug("PWD: %s\n",pwdPath);
	char* pathPath = getenv("PATH");
	debug("Path: %s\n", pathPath);*/

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

{
	int status;
	char*outputBuffer;
	long size = 256;
	char* outputPtr;
	pid_t pid;
	if( (pid=fork())==0 )
	{
		if( (outputBuffer=malloc(size*sizeof(char*)) )!=NULL)
		{
			outputPtr =getcwd(outputBuffer,size); //get working directory, put into outputPtr
			fprintf(stdout,"%s\n",outputPtr);
			free(outputBuffer);
		}
		exit(EXIT_SUCCESS);
	}
	wait(&status);//when terminates, status becomes 256
	outputPtr = "";

}

char** strSplit(char* line,char* tokenDelimiter) //split string into str array, or char** and malloc.
{
	int outputBufferSize = 256; //set a good buffer size
	char** tmpResult = malloc(sizeof(char*)*outputBufferSize);
	if(tmpResult == NULL) //failed to malloc
	{
		printf("FAILED TO MALLOC WHEN PARSING.");
		exit(1); //I believe 1 is for failure, 0 is EXIT_SUCCESS
	}
//	char* tokenDelimiter="  "; //for strTok
//	*tokenDelimiter = (char)delimiter;
//	*(tokenDelimiter+1)=0; //null character to make it only grab delimiter
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





