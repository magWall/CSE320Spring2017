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
else if(strcmp(cmd,"ls")==0) //test for ls
	return 4;
else if(strstr(cmd,"/")!=0) //relative bin/whatever
	return 5;
else //assumes exists inside call
	return 6;
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
void cmdNotRelative(char** words)
{
		int argsLen = 0;
		while( *(words+argsLen)!=0)
			argsLen++;
		char* args[argsLen+1];//+1 for null
		char somechar[256] = "/bin/";
		strcat(somechar, *words);
        args[0] = somechar; //executable needs keyvalue, filename with args, and filename path
        debug("%s\n",args[0]);
        int idx =1;
        while( *(words+idx)!=0) //for args -i -r -etc
		{
			args[idx]=*(words+idx);
			idx++;
		}
		args[argsLen+1]=NULL;
        char* paths = getenv("PATH");
    	char* delimiter2 = ":";
    	char** allDir = strSplit(paths, delimiter2);

        pid_t pid = Fork();
        int status;
	if(pid ==0)
	{
       	execve(args[0],args,allDir);//path, arg
       	perror("failed");
		exit(EXIT_SUCCESS);
	}
	wait(&status);

}
void cmdExecutable(char** words) //runs through relative path ---- FIXED[?]
{
	int argsLen = 0;
	while( *(words+argsLen)!=0)
		argsLen++;
	char* args[argsLen+1];//+1 for null
	//args[0]= "/";
	args[0]= *words;

	debug("%s\n",args[0]);
	int idx = 1;

	while( *(words+idx)!=0) //for args -i -r -etc
	{
		args[idx]=*(words+idx);
		idx++;
	}
	args[argsLen+1]=NULL;
//	char* envp[2];
//	envp[0] = "PATH=";

	char* paths = getenv("PATH");
    char* delimiter2 = ":";
    char** allDir = strSplit(paths, delimiter2);


//	envp[1] = NULL;
	pid_t pid = Fork();
    int status;
	if(pid ==0)
	{
       	execve(args[0],args,allDir);//path, arg
       	perror("failed");
		exit(EXIT_SUCCESS);
	}
	wait(&status);

}
void cmdLs() //test for ls to see if it works
{
	  /*  char* delimiter2 = ":";
        char* paths = getenv("PATH");
      bu
      strcat(envp[0],getenv("PWD"));g("%s \n",paths);
        char** allDir = strSplit(paths, delimiter2);
        int idxDir =0;
        while(*(allDir+idxDir)!=0)
        {
        	debug("%s\n",*(allDir+idxDir));
        	idxDir++;

        }
        */
        char* args[2];
        args[0] = "/bin/ls"; //executable needs keyvalue, filename with args, and filename path
        args[1] = NULL;
        char* envp[2];
        envp[0] = "/bin";
        envp[1] = NULL;

        pid_t pid = Fork();
        int status;
	if(pid ==0)
	{
       	execve("/bin/ls",args,envp);//path, arg
       	perror("failed");
		exit(EXIT_SUCCESS);
	}
	wait(&status);


}
void cmdCd(char** words)
{

	if(*(words+1)!=NULL) //if not just cd
	{
		if(  strcmp(*(words+1),"-")==0 ) //go back prev directory
		{
			char* oldpwdPath = getenv("OLDPWD");
			if( strcmp(oldpwdPath,"")!=0 )
			{
				char* pwdPath = getenv("PWD");
				chdir(oldpwdPath);
				setenv("PWD",oldpwdPath,1); //overwrite prev. working directory.
				setenv("OLDPWD",pwdPath,1);
			}
			else
				printf("OLDPWD not set.");

		}	//
		else if(strncmp(*(words+1),"..",2)==0)
		{
			//go back one path and check if file exists
			char* pwdPath = getenv("PWD");
			debug("PWD: %s\n",pwdPath);

			char oldPWD[256];//oldpwd buffer
			oldPWD[0]=0;
			char* oldpwdPath= oldPWD;
			strcpy(oldpwdPath,pwdPath);
			debug("PWD: %s\n",oldpwdPath);

			char* splitToken= "/";
			char** returnedPath = strSplit(pwdPath,splitToken);
			int idx=1; //can't go back to prev directory if there's no previous directory.
			char buffer[256];
			buffer[0]= 0; //make buffer set to 0 so strcat will not be affected.

			char* newPathPtr = buffer; //    0 1 2 3
			strcat(newPathPtr,"/");
			strcat(newPathPtr, *returnedPath);
			debug("newPath: %s\n",newPathPtr);
//			debug("pwdPath: %s\n",pwdPath); //path got destroyed by calling tok
			while( *(returnedPath+idx+1)!=NULL) //getting directory before
			{
				debug("path:%s\n",*(returnedPath+idx));
				strcat(newPathPtr,"/");
				strcat(newPathPtr,*(returnedPath+idx));
			//	debug("newPath: %s\n",newPath);
				idx++;
			}
			idx = 1;
			debug("newPath: %s\n",newPathPtr);
//			debug("pwdPath: %s\n",pwdPath); //path got destroyed by calling tok

			idx = 2;
			int lenPwd = strlen(newPathPtr);
			//debug("strlen of %s is %d\n",buffer, lenPwd);
			while( *(*(words+1)+idx)!=0 )//getting next input directory
			{
				*(newPathPtr+lenPwd+idx-2) = *(*(words+1)+idx);
				idx++;
			}
			DIR* tmpDir = opendir(newPathPtr);
			if(tmpDir !=NULL)
			{
				debug("oldPwd:%s\n",oldpwdPath);
				setenv("OLDPWD",oldpwdPath,1);
				chdir(newPathPtr);
				setenv("PWD",newPathPtr,1);//overwrite old path
				debug("newPath final:%s\n",newPathPtr);
				debug("PWD: %s\n",getenv("PWD"));
				debug("OLPWD%s\n",getenv("OLDPWD"));
			}
			else if( 2 == errno)
			{
				setenv("PWD",oldpwdPath,1); //to fix strtok
				perror("Invalid path or directory.");
				//printf("Invalid Path");
			}
		}
		else if( strncmp(*(words+1),".",1)==0 ) //current directory
		{
			//read whole path, if file exist, go to that path
			char* pwdPath = getenv("PWD");
			char newPath[256];
			char* newPathPtr = newPath;
			strcpy(newPathPtr,pwdPath);


			int lenPwd= strlen(pwdPath);
			int idx =1;
			while( *(*(words+1)+idx)!=0 )
			{
				*(newPathPtr+lenPwd+idx-1) = *(*(words+1)+idx);
				idx++;
			}
			*(pwdPath+lenPwd+idx-1) =0; //add null terminator
			debug("Prev:%s\n",pwdPath);
			debug("Next PWD:%s\n",newPathPtr);
			DIR* tmpDir = opendir(pwdPath);
			if(tmpDir!=NULL) //if you can't open the directory, then it doesn't exist
			{
				closedir(tmpDir);
				chdir(newPathPtr);
				setenv("OLDPWD",pwdPath,1);
				setenv("PWD",newPathPtr,1);

			}
			else if(2 == errno)//tmpDir doesn't exist, invalid directory
			{
				perror("Invalid directory");
			//	printf("Invalid directory."); //to stdout for ease of seeing
			}


		}	//

		else //cd something
		{
			char* pwdPath = getenv("PWD");
			//one case where if you pass in a fake directory, your oldpwd may mess up
			char newPath[256];
			char* newPathPtr = newPath;
			strcpy(newPathPtr,pwdPath);
			strcat(newPathPtr,"/");
			strcat(newPathPtr,*(words+1));
			debug("pwd %s\n",newPathPtr);
			debug("pwd %s\n",pwdPath);
			DIR* tmpDir = opendir(newPathPtr);
			if(tmpDir!=NULL)
			{

				setenv("OLDPWD",pwdPath,1);
				closedir(tmpDir);
				chdir(newPathPtr);
				setenv("PWD",newPathPtr,1);
				debug("CurrDir: %s\n",getenv("PWD"));
				debug("OPWD: %s\n",getenv("OLDPWD"));
			}
			else if(2 == errno) //google said 2 = invalid nonexistent directory
			{
				perror("Invalid directory.");
				//printf("Invalid directory."); //to stdout for ease of seeing
			}
			//same as  . , check if there's a path to there
			//check if path exists to there, if it does, change to that directory
		}
	}
	else // it's just cd
	{
		char* pwdPath = getenv("PWD");
//		debug("PWD: %s\n",pwdPath);
		setenv("OLDPWD",pwdPath,1);
		char* envirHome = getenv("HOME"); //path to home if this directory
		chdir(envirHome);//change directory to home
		setenv("PWD",envirHome,1); //1 to overwrite
		//pwdPath = getenv("PWD");
		//debug("PWD(home): %s\n",pwdPath);
		//char* oldPWD = getenv("OLDPWD");
		//debug("OLDPWD:%s\n",oldPWD);
		//char* user = getenv("USER");
		//debug("user: %s\n",user);
		//char* term = getenv("TERM");
		//debug("term: %s\n",term);


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
			while(outputPtr == NULL && errno == ERANGE)
			{
				size+=256;
				if( (outputBuffer=realloc(outputBuffer,size*sizeof(char*)) )!=NULL)
				{
					outputPtr =getcwd(outputBuffer,size);
				}
				else//handle getcwd failure
				{
					perror("Size for PWD failed, unable to get proper size.");
					exit(EXIT_FAILURE);
				}

			}
			fprintf(stdout,"%s\n",outputPtr);
			free(outputBuffer);
		}
		else
		{
			perror("Malloc for PWD failed.");
			exit(EXIT_FAILURE);
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





