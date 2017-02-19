#include <hw2.h>
#include <unistd.h>
#include <libgen.h>
#include "../include/debug.h"

char DEFAULT_DICT_FILE[]= "./rsrc/dictionary.txt"; //default should be rsrc dictionary directory
FILE* DEFAULT_INPUT= NULL;      //I hate this error
FILE* DEFAULT_OUTPUT= NULL;     //had to be set to null

void printDictionary(struct dict_word* currWord, FILE* f); //it wouldn't register hw2.h's statement and I tried include <hw2.h>
//and the one above
char* theMisspelledWord(char* inputWord); //neither this one
void nullifySpace(char* arr, int size); //qq
void printTopThreeMispelledWords(struct dict_word* a);

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
        free(dict);
        return EXIT_FAILURE;
    }
    m_list = NULL;

    struct Args* args;  //change all . into -> by making it a pointer
    if( (args = (struct Args*) malloc(sizeof(struct Args))) == NULL)
    {
        printf("ERROR: OUT OF MEMORY.\n");
        free(dict);
        free(m_list);
        return EXIT_FAILURE;
    }
    // Set struct default values
    args->d = false;
    args->i = false;
    args->o = false;
    args->aFlag = false;
    strcpy(args->dictFile, DEFAULT_DICT_FILE);

   // char line[MAX_SIZE];
    //Declare Files
    FILE* dFile = NULL; //set this to null
    FILE* iFile = NULL; //only set this when there's no default inputs and outputs
    FILE* oFile = NULL;   //put this at the bottom and check after if args.input is null or something
    FILE* dNewFile = NULL; //make a new dictFile if An is valid and we can add new words
    int optChosen = 0;
    int numMispellings =0; //check An, An will be 0  unless otherwise stated
    while( (optChosen = getopt(argc, argv, "ho:i:d:A::")) != -1)       //two colons indicate optional, 1 is required argument
    {
        opterr=0;                                 //getopt == -1 when exhausted all args, opterr = 0 to stop all error messages from appearing
        switch(optChosen){
            case 'h': USAGE(EXIT_SUCCESS);     //if help, print help, then exit
                return EXIT_SUCCESS;
                break;      //may not be needed
            case 'o': strcpy(args->output, optarg);  //optarg is supposedly the argument being returned if I understand this correctly if flag raised
                        args->o = true;
             //           debug("reached here, optarg: %s",optarg);
                        oFile = fopen(optarg, "w");
                break;
            case 'i': strcpy(args->input, optarg);
                        args->i = true;
                        iFile = fopen(optarg, "r");
                break;
            case 'd': strcpy(args->dictFile, optarg); //take a new input or just use default dictionaryarg
                        args->d = true;
                break;
            case 'A': if(optarg == 0 || *optarg == ' ')
            {
                USAGE(EXIT_FAILURE);
                return (EXIT_FAILURE);
            }
            numMispellings = atoi(optarg);
            if( numMispellings>5 || numMispellings<0)
                {
                    USAGE(EXIT_FAILURE);
                    return (EXIT_FAILURE);
                }
            else
            {
                args->aFlag = true;
            }
               //range of 0-5 mispellings
                break;
            default:
                USAGE(EXIT_FAILURE);
                return (EXIT_FAILURE);
        }

    }
    dFile = fopen(args->dictFile, "r");
    if(iFile == NULL && args->i == true)
    {
        //printf("Unable to open: %s.\n", args->input);
        USAGE(EXIT_FAILURE);
        return EXIT_FAILURE;
    }
        else if(args->i == false)
            iFile = DEFAULT_INPUT;
    if(oFile == NULL && args->o == true)
    {
        //printf("Unable to open: %s.\n", args->output);
        USAGE(EXIT_FAILURE);
        return EXIT_FAILURE;
    }
        else if(args->o == false)
            oFile = DEFAULT_OUTPUT;
    if(dFile == NULL)
    {
        //printf("Unable to open: %s.\n", args->dictFile);
        USAGE(EXIT_FAILURE);
        return EXIT_FAILURE; /*THIS SHOULD NOT HAPPEN BECAUSE
         DEFAULT FILE EXISTS BUT SAFETY PRECAUTION */
    }
    else
    {
        processDictionary(dFile); //sets up dictionary
    }
    int originalDictWords = dict->num_words;
    //add  more new words with An, afterwards, if new dictionary words are added and is valid case



 //   strcpy(line,"\n--------INPUT FILE WORDS--------\n");
 //   fwrite(line, strlen(line)+1, 1, oFile);         //writes INPUT FILE WORDS or whatever to the outputFile

    while(!feof(iFile))                             //if !endofINPUTFILE
    {
        char word[WORDLENGTH];
        char* wdPtr = word;
        char line[MAX_SIZE];        //uncommented out
        char* character = line;
    //    *line = 0;

        fgets(line, MAX_SIZE+1, iFile);
      //  if(*line ==0)
       //     break;
        //if there isn't a space or newline at the end of the line, put one there
        if((line[strlen(line)-1] != ' ') && (line[strlen(line)-1] != '\n'))
            strcat(line, " ");
        //replaces spaces within a line with new lines

        while(*character != 0)
        {
            *character = tolower(*character); //convert all characters to lowercase
            if(*character == ' ' || *character == '\n')
            {
                if(*word != 0)
                {
                    char* punct = wdPtr-1; //dont look at space or \n
                    debug("punct:%c\n",*punct);
                    char* beginPunct = word;//beginning punctuations
                    while(!((*beginPunct>='a' && *beginPunct<='z') || (*beginPunct>='A' && *beginPunct<='Z')))
                    {
                        fprintf(oFile,"%c",*beginPunct);
                        beginPunct++;

                    }//prints all before word
                    int pointerLength=0; //punctuation length until end of wdptr
                    while(!((*punct>='a' && *punct<='z') || (*punct>='A' && *punct<='Z')))
                    {
                        debug("currentPunct: %c\n",*punct);
                        pointerLength++;
                        punct--;
              //          if(punct == word)
              //              break;

                    }
                    punct++;//might need to save the value of whatever punctuation was here
                    char tmpLostPunct;
                   // if(punct-1 != word)
                   // {
                        tmpLostPunct = *punct;
                    //}
                    //else
                    //    tmpLostPunct = *(punct+pointerLength);
                    debug("tLP is: %c\n",tmpLostPunct);

    //                printf("%d", (int)(strlen(wdPtr)-strlen(punct)) ); //cast strlen into int from long

                    debug("Before 0 word is: %s\n",word);
                   // if(punct-1 != word)
                        *punct = 0;             //set terminator here to end string
                   // else
                        *(punct+pointerLength)=0; //if the string is only **!@#&!@^$!@# cause ffs edge cases
                    debug("After 0 word is: %s\n",word);

                   // if(punct-1!=word)
                        wdPtr = beginPunct;    //skip all the punctuation before the word --t** --is skipped
                   // else
                   //     wdPtr = punct;
                    debug("word is: %s\n",word);
                    debug("wdPtr is: %s\n",wdPtr);
                    //if(punct != word) //dont add random nonalphabets to word
                        processWord(wdPtr, numMispellings);

                    if(foundMisspelledMatch(wdPtr)==true) //if word x
                        {
                            fprintf(oFile,"%s",theMisspelledWord(wdPtr));//findCorrectWord
                        }
                    else
                        fprintf(oFile,"%s",wdPtr);//else it's the correct word
                    //print punctuation after, make sure to end the punctuation with null character after
                   // if(punct!=word) //if the whole thing isn't just random punctuations
                   // {
                        *punct = tmpLostPunct;
                       *(punct+pointerLength)=0; //null character
                        fprintf(oFile,"%s",punct);

                   // }

                    if(*character == ' ')
                       fprintf(oFile,"%c",*character);
                    else if(*character == '\n')
                        fprintf(oFile,"%c",*character);
                    //fwrite(wdPtr, strlen(wdPtr)+1, 1, oFile);
                *word = 0; //reset to handle extra spaces and potential punctuation clash
//                nullifySpace(wdPtr, strlen(wdPtr));
                wdPtr = word;//punctToIgnore; //move word pointer to point back to the beginning to fix **(wdPtr++) handler
               }
               else
               {
                fprintf(oFile,"%c",*character);//print out white space
               }

            }
            else
            {
                *(wdPtr++) = *character; //gets rid of the space or whatever, ex ---it---\n will add \n to wdPtr
            }
            character++;
        }

        if(iFile == stdin)
            break;

    } //self reference to undo up to here if you screw up
    if (args->aFlag == true && (dict->num_words > originalDictWords) ) //if new words are added due to An
    {

       // debug("dirname:%s,basename:%s\n",dirname(args->dictFile),basename(args->dictFile));
        //debug("concat:%s",strcat(basename(args->dictFile),dirname(args->dictFile)));
        char newDictSrc[MAX_SIZE];
        char tmpNameHolder[MAX_SIZE];
        strcpy(tmpNameHolder, args->dictFile);
        strcpy(newDictSrc,dirname(tmpNameHolder));
        strcpy(tmpNameHolder, args->dictFile);
        //debug("%s\n",newDictSrc);
        //debug("%s\n",args->dictFile);

        strcat(newDictSrc, "/new_");
       // debug("%s\n",newDictSrc);
       // debug("%s\n",basename(args->dictFile));
        strcat(newDictSrc,basename(tmpNameHolder));
//        debug("%s\n",newDictSrc);
        debug("%s\n",newDictSrc);
        dNewFile = fopen(newDictSrc,"w");
        printDictionary(dict->word_list , dNewFile); //print new dictionary . edit this
    }
    fprintf(stderr, "Total number of words in dictionary: %d\n"
                    "Size of dictionary (in bytes): %d\n"
                    "Total number of misspelled words: %d\n"
                    "Top 3 misspelled words:\n", dict->num_words,
                    (int)(sizeof(struct dictionary) + sizeof(struct dict_word) * dict->num_words),
                    totalMisspellings(dict->word_list));

 /** IMPLICIT DECLARATION broke the code **/
 //   struct dict_word* topW = (struct dict_word*)topMisspelled(dict->word_list);
  //  struct dict_word* topW2 =(struct dict_word*)topTwoMisspelled(dict->word_list,topW);
  //  struct dict_word* topW3 =(struct dict_word*)topThreeMisspelled(dict->word_list,topW,topW2);
 //   printTopThreeMispelledWords(topW);
 //   printTopThreeMispelledWords(topW2);
 //   printTopThreeMispelledWords(topW3);

//    strcpy(line, "\n--------DICTIONARY WORDS--------\n");
 //   fwrite(line, strlen(line)+1, 1, oFile);

    /* *******
       ************************

    */
        //printWords(dict->word_list , oFile); //print new dictionary as well into oFile? edit this
    if (args->aFlag == true && (dict->num_words > originalDictWords) ) //if new words are added due to An
        fclose(dNewFile);
    freeSpace(args);
    fclose(dFile);
    fclose(iFile);
    fclose(oFile);
    return EXIT_SUCCESS;
}
