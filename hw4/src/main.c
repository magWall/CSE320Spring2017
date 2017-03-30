#include "sfish.c"
#include "debug.h"

/*
 * As in previous hws the main function must be in its own file!
 */

int main(int argc, char const *argv[], char* envp[]){
    /* DO NOT MODIFY THIS. If you do you will get a ZERO. */
    rl_catch_signals = 0;
    /* This is disable readline's default signal handlers, since you are going to install your own.*/
    char *cmd;
    while((cmd = readline("<kennylee> :")) != NULL) {
        if (strcmp(cmd, "exit")==0)
            break;
        printf("%s\n",cmd);
        /* All your debug print statements should use the macros found in debu.h */
        /* Use the `make debug` target in the makefile to run with these enabled. */
        info("Length of command entered: %ld\n", strlen(cmd));
        int x;
        if( (x= isValidCmd(cmd))!=-1 )
        {
            if(x==1)
                cmdCd();
            else if(x==2)
                cmdHelp();
            else if(x==3)
                cmdPwd();
            //asdasd
        }

        /* You WILL lose points if your shell prints out garbage values. */
    }

    /* Don't forget to free allocated memory, and close file descriptors. */
    free(cmd);

    return EXIT_SUCCESS;
}