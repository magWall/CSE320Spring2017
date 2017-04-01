#include "sfish.h"
#include "debug.h"

/*
 * As in previous hws the main function must be in its own file!
 */

int main(int argc, char const *argv[], char* envp[]){
    /* DO NOT MODIFY THIS. If you do you will get a ZERO. */
    rl_catch_signals = 0;
    /* This is disable readline's default signal handlers, since you are going to install your own.*/
    char *cmd;
    char readlineBuffer[384]; //chose random number 128*3 for buffer.
    char* displays = readlineBuffer;
    strcpy(displays,"<kennylee> : <");
    int len = strlen(displays);
    char* PWDpath = getenv("PWD");
    strcat(displays,PWDpath);
    int len2 = strlen(displays);
    *(displays+len2)='>';
    *(displays+len2+1)=' ';
    *(displays+len2+2)= '$';
    *(displays+len2+3)= 0;
    setenv("OLDPWD","",1);
    while((cmd = readline(displays)) != NULL) {
        if (strcmp(cmd, "exit")==0)
            break;
       // printf("%s\n",cmd);
        /* All your debug print statements should use the macros found in debu.h */
        /* Use the `make debug` target in the makefile to run with these enabled. */
      //  info("Length of command entered: %ld\n", strlen(cmd));
        char* delimiter =" ";
        char** words = strSplit(cmd,delimiter);
        int x= isValidCmd(words);
        if( x!=-1 )
        {
            if(x==1)
                cmdCd(words);
            else if(x==2)
                cmdHelp();
            else if(x==3)
                cmdPwd();
            else if(x==4)
                cmdLs();
            //asdasd
        }
        else if(x==-1)
            printf("Invalid command, use help.\n");


        /* You WILL lose points if your shell prints out garbage values. */
        free(words);
        PWDpath = getenv("PWD");
        int idx =0;
        while( *(PWDpath+idx)!=0 )
        {
            *(displays+len+idx)= *(PWDpath+idx);
            idx++;
        }
        *(displays+len+idx)='>';
        *(displays+len+idx+1)=' ';
        *(displays+len+idx+2)= '$';
        *(displays+len+idx+3)= 0;        //null terminating character.
    }

    /* Don't forget to free allocated memory, and close file descriptors. */
    free(cmd);

    return EXIT_SUCCESS;
}
