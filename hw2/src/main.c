#include "../include/hw2.h"
#include <unistd.h>
char DEFAULT_DICT_FILE[]= "./rsrc/dictionary.txt"; //default should be rsrc dictionary directory
FILE* DEFAULT_INPUT= NULL;      //I hate this error
FILE* DEFAULT_OUTPUT= NULL;     //had to be set to null

int main(int argc, char *argv[]){
    DEFAULT_INPUT = stdin;
    DEFAULT_OUTPUT = stdout;
    //create dictionary
    if((dict = (struct dictionary*) malloc(sizeof(struct dictionary))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }

    if((m_list = (struct misspelled_word*) malloc(sizeof(struct misspelled_word*))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }
    m_list = NULL;

    struct Args* args;  //change all . into -> by making it a pointer
    if( (args = (struct Args*) malloc(sizeof(struct Args))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        return EXIT_FAILURE;
    }
    // Set struct default values
    args->d = false;
    args->i = false;
    args->o = false;
    strcpy(args->dictFile, DEFAULT_DICT_FILE);

    char line[MAX_SIZE];
    //Declare Files
    FILE* dFile = NULL; //set this to null
    FILE* iFile = NULL; //only set this when there's no default inputs and outputs
    FILE* oFile = NULL;   //put this at the bottom and check after if args.input is null or something

    int optChosen = 0;
    int numMispellings =0; //check An, An will be 0  unless otherwise stated
    while( (optChosen = getopt(argc, argv, "ho:i:d:A:")) != -1)       //two colons indicate optional, 1 is required argument
    {
        opterr=0;                                 //getopt == -1 when exhausted all args, opterr = 0 to stop all error messages from appearing
        switch(optChosen){
            case 'h': USAGE(EXIT_SUCCESS);     //if help, print help, then exit
                return EXIT_SUCCESS;
                break;      //may not be needed
            case 'o': strcpy(args->output, optarg);  //optarg is supposedly the argument being returned if I understand this correctly if flag raised
                        args->o = true;
                        oFile = fopen(optarg, "w");
                break;
            case 'i': strcpy(args->input, optarg);
                        args->i = true;
                        iFile = fopen(optarg, "r");
                break;
            case 'd': strcpy(args->dictFile, optarg); //take a new input or just use default dictionary
                        args->d = true;
                break;
            case 'A': numMispellings = atoi(optarg);
            if( numMispellings>5 || numMispellings<0)
                {
                    USAGE(EXIT_FAILURE);
                    return (EXIT_FAILURE);
                }                           //range of 0-5 mispellings
                break;
            default:
                USAGE(EXIT_FAILURE);
                return (EXIT_FAILURE);
        }

    }
    dFile = fopen(args->dictFile, "r");

    if(iFile == NULL && args->i == true)
    {
        printf("Unable to open: %s.\n", args->input);
        return EXIT_FAILURE;
    }
        else if(args->i == false)
            iFile = DEFAULT_INPUT;
    if(oFile == NULL && args->o == true)
    {
        printf("Unable to open: %s.\n", args->output);
        return EXIT_FAILURE;
    }
        else if(args->o == false)
            oFile = DEFAULT_OUTPUT;
    if(dFile == NULL)
    {
        printf("Unable to open: %s.\n", args->dictFile);
    }
    else
    {
        processDictionary(dFile); //sets up dictionary
    }


    // Make a loop index
    //    int i;

    //use bottom code as reference
    /*    char opt = '\0';

    for(i = 1; i< argc; i++)
    {
        char* currArg = argv[i];
        //there's a flag
        if(opt != '\0')
        {
            if(opt == 'd')
            {
                strcpy(args.dictFile, currArg);
                args.d = true;
            }
            if(opt == 'i')
            {
                strcpy(args.input, currArg);
                args.i = true;
                iFile = fopen(currArg, "r");
            }
            if(opt == 'o')
            {
                strcpy(args.output, currArg);
                args.o = true;
                oFile = fopen(currArg, "w");
            }
            opt = '\0';
        }
        else
        {
            if(strcmp(currArg, "-d") == 0)
                opt = 'd';
            if(strcmp(currArg, "-i") == 0)
                opt = 'i';
            if(strcmp(currArg, "-o") == 0)
                opt = 'o';
        }
    }
    dFile = fopen(args.dictFile, "r");

    if(iFile == NULL && args.i == true)
    {
        printf("Unable to open: %s.\n", args.input);
        return EXIT_FAILURE;
    }
    if(dFile == NULL)
    {
        printf("Unable to open: %s.\n", args.dictFile);
    }
    else
    {
        processDictionary(dFile);

    }*/

    strcpy(line,"\n--------INPUT FILE WORDS--------\n");
    fwrite(line, strlen(line)+1, 1, oFile);         //writes INPUT FILE WORDS or whatever to the outputFile

    while(!feof(iFile))                             //if !endofINPUTFILE
    {
        char word[MAX_SIZE];
        char* wdPtr = word;
        char line[MAX_SIZE];        //uncommented out
        char* character = line;

        fgets(line, MAX_SIZE+1, iFile);

        //if there isn't a space or newline at the end of the line, put one there
        if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n'))
            strcat(line, " ");
        //replaces spaces within a line with new lines
        while(*character != 0)
        {
            if(*character == ' ' || *character == '\n')
            {
                char* punct = wdPtr-1;
                    printf("char:%c",*punct);//dereferenced this to print char
                while(!((*punct>='a' && *punct<='z') || (*punct>='A' && *punct<='Z')))
                {
                    punct--;
                }
                punct++;
                printf("%d", (int)(strlen(wdPtr)-strlen(punct)) ); //cast strlen into int from long


                *wdPtr = 0;             //trying to reset the value ?
                wdPtr = word;

                processWord(wdPtr);

                strcat(wdPtr, " ");
                fwrite(wdPtr, strlen(wdPtr)+1, 1, oFile);
            }
            else
            {
                *(wdPtr++) = *character;
            }
            character++;
        }

        if(iFile == stdin)
            break;
    }

    strcpy(line, "\n--------DICTIONARY WORDS--------\n");
    fwrite(line, strlen(line)+1, 1, oFile);
    printWords(dict->word_list , stderr); //print it to  stderr as stats

    //printf("\n--------FREED WORDS--------\n");
    freeWords(dict->word_list);
    //free dictionary
    free(dict);
    //free m_list
    free(m_list);

    fclose(dFile);
    fclose(iFile);
    fclose(oFile);
    return EXIT_SUCCESS;
}
