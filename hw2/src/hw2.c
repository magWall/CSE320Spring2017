#include "../include/hw2.h"
#include "../include/mispelling.h"
#include "../include/debug.h"
/* Great filename. */

void processDictionary(FILE* f){
    dict->num_words = 0;
    while(!feof(f))
    {
        //initialize the current word.
        struct dict_word* currWord;
        if((currWord = (struct dict_word*) malloc(sizeof(struct dict_word))) == NULL)
        {
            printf("OUT OF MEMORY.\n");
            return;
        }
        currWord->num_misspellings = 0;
        currWord->misspelled_count = 0;

        //variables
        char word[WORDLENGTH]; //WORDLENGTH VS MAX_SIZE, other instances made it WORDLENGTH, change it to wordlength
        char* wdPtr = word;
        char line[MAX_SIZE];
        char* character = line;
        *line =0;

//        char word_list[MAX_MISSPELLED_WORDS+1][MAX_SIZE];      //COMMENTED OUT
        int counter = 0;
        int firstWord = 1;

        fgets(line, MAX_SIZE+1, f); //to catch for empty line
        if(*line==0)
        {
            free(currWord);
            return;
        }
        //if there isn't a space at the end of the line, put one there
        if((line[strlen(line)-2] != ' ' && line[strlen(line)-1] == '\n') || (line[strlen(line)-1] != ' ' && line[strlen(line)-1] != '\n'))
            strcat(line, " ");      //I feel like this is more convoluted than it needs to be...*SELF NOTE TO CHECK THIS STATEMENT*

        while(*character != 0) //while character isn't null, NULL is treated as pointer so change it to 0 bcz 0= NULL
        {
            *character = tolower(*character);
            if(counter >= MAX_MISSPELLED_WORDS+1) //only take up to 5, don't break at 5
                break;
            //if the character is a space, add the word in word_list and make word NULL.
            if(*character == ' ')
            {
                *wdPtr = 0;     //word is done, end it by terminating it **** check****
                wdPtr = word;   //reset pointer to beginning of address
                if(firstWord) //if not zero, then true
                {
                    addWord(currWord, wdPtr);
                    dict->num_words++;

                    firstWord = 0;
                }
                else                                    //**** only the first word is the correct spelling, the rest is the counter ****///
                {
                    struct misspelled_word* currMisspelling;
                    if((currMisspelling = malloc(sizeof(struct misspelled_word))) == NULL)
                    {
                        printf("ERROR: OUT OF MEMORY.");
                        return;
                    }
                //    debug("m_list before adding mispelled word to dict: %s\n",m_list->word);
                    addMisspelledWord(currMisspelling, currWord, wdPtr);
                //    debug("m_list after adding mispelled word to dict: %s\n",m_list->word);
                    counter++; //needs to increment somehow...
                }
            }
            //if the character isn't a space or a new line, add the character to word.
            else if(*character != '\n')
                *(wdPtr++) = *character; //adds letter to wordPtr, then increment
            character++;
        }
    }
}

void addWord(struct dict_word* dWord, char* word){
    //setting up dWord fields
    dWord->misspelled_count = 0;
    dWord->num_misspellings = 0;
    dWord->next = dict->word_list;
    strcpy(dWord->word, word);
    dict->word_list = dWord;
}

void addMisspelledWord(struct misspelled_word* misspelledWord, struct dict_word* correctWord, char* word){
    //setting up misspelledWord fields
    strcpy(misspelledWord->word, word);
    misspelledWord->misspelled = 0;
    misspelledWord->correct_word = correctWord;
    misspelledWord->next = m_list;
   // debug("m_list's curr word:%s\n", m_list->word);
    //debug("misspelledWord->word %s,->next %s\n",misspelledWord->word,misspelledWord->next->word);

    (correctWord->misspelled)[correctWord->num_misspellings] = misspelledWord; //inrement num mispellings, add to array
    debug("correctword:%s,num_mispellings %d\n",correctWord->word,correctWord->num_misspellings);
    debug("correctword's mispelled[%d],%s\n",correctWord->num_misspellings,correctWord->misspelled[correctWord->num_misspellings]->word);
    ++correctWord->num_misspellings;
    m_list = misspelledWord;
}

void freeWords(struct dict_word* currWord){
    if(currWord != NULL)
    {
        freeWords(currWord->next); //loop and free all the words in the linkedlist

    //    int i;                                    COMMENTED OUT
        //free word
    //    printf("FREED %s\n", currWord->word);
        free(currWord);
    }
}
void freeMWords(struct misspelled_word* currWord){
    if(currWord != NULL)
    {
        freeMWords(currWord->next); //loop and free all the words in the linkedlist

    //    int i;                                    COMMENTED OUT
        //free word
    //    printf("FREED %s\n", currWord->word);
        free(currWord);
    }
}
int totalMisspellings(struct dict_word* currWord){

    if(currWord != NULL)
    {
        return currWord->num_misspellings+ totalMisspellings(currWord->next);
        //loop and free all the words in the linkedlist
    }
    return 0;
}
void printDictionary(struct dict_word* currWord, FILE* f)
{
    if(currWord != NULL)
    {
        printDictionary(currWord->next, f);

        char line[MAX_SIZE];
        int i;
        sprintf(line, "%s", currWord->word);
        fprintf(f,"%s",line);
       if(currWord->num_misspellings>0)
        fprintf(f,"%s"," ");

       for(i = 0; i<currWord->num_misspellings; i++)
        {
            sprintf(line, "%s", currWord->misspelled[i]->word);
            fprintf(f,"%s",line);
            if(i !=currWord->num_misspellings)
                fprintf(f,"%s"," ");
        }
        fprintf(f,"%s","\n");
    }

}
void printWords(struct dict_word* currWord, FILE* f){
    if(currWord != NULL)
    {
        printWords(currWord->next, f);

        char line[MAX_SIZE];
        int i;

        sprintf(line, "%s\n", currWord->word);
        fwrite(line, strlen(line)+1, 1, f);

        sprintf(line, "\tNUMBER OF TIMES WORD IS MISSPELLED: %d\n", currWord->misspelled_count); // puts string into buffer
        fwrite(line, strlen(line)+1, 1, f);

        sprintf(line, "\tNUMBER OF MISSPELLINGS: %d\n", currWord->num_misspellings);
        fwrite(line, strlen(line)+1, 1, f);

        for(i = 0; i<currWord->num_misspellings; i++)
        {
         //   debug("goes into here %d\n",i);
            sprintf(line, "\tMISPELLED WORD #%d: %s\n", i,currWord->misspelled[i]->word);   //number
            fwrite(line, strlen(line)+1, 1, f);

            sprintf(line,"\t\tMISPELLED?: %d\n", ((currWord->misspelled)[i])->misspelled);      //should be each individual word
            fwrite(line, strlen(line)+1, 1, f);

            sprintf(line, "\t\tACTUAL WORD: %s\n", ((currWord->misspelled)[i])->correct_word->word); //needs to point to char word
            fwrite(line, strlen(line)+1, 1, f);

            if(((currWord->misspelled)[i])->next != NULL) //same problem as below
            {
                sprintf(line, "\t\tNEXT MISPELLED WORD: %s\n", ((currWord->misspelled)[i])->next->word);
                fwrite(line, strlen(line)+1, 1, f);
            }
        }

        if((currWord->next)!= NULL)//can't check next word if currWord->next is already null, modified next->word to next
        {
            sprintf(line,"\tNEXT WORD: %s\n", (currWord->next)->word);
            fwrite(line, strlen(line)+1, 1, f);
        }
    }
}

void processWord(char* inputWord, int numMisspellings){
    if(foundMisspelledMatch(inputWord))
        return;
    if(foundDictMatch(inputWord))
        return;
    else
    {
    /*    char ch;
        char conf;

        do
        {
            printf("\"%s\" was not found in the dictionary. Do you want to add it (Y/N)? ", inputWord);
            scanf("%c", &conf);
            while ((ch = getchar()) != '\n' && ch != EOF);
        }while(conf!='Y' && conf!='N');

        if(conf == 'Y')
        {*/
            struct dict_word* newWord;
            //int counter = 0;  COMMENTED OUT

            if((newWord = (struct dict_word*) malloc(sizeof(struct dict_word))) == NULL)
            {
                printf("ERROR: OUT OF MEMORY.\n");
                return;
            }
               newWord->num_misspellings = 0;
               newWord->misspelled_count = 0;

            addWord(newWord, inputWord);
            dict->word_list = newWord;
            dict->num_words++;//increment

            //remove user based input, only add if An is passed
         /*   printf("Added \"%s\" to Dictionary. Add misspellings (Y/N)? ", inputWord);

            do
            {
                scanf("%c", &conf);
                while ((ch = getchar()) != '\n' && ch != EOF);
            }while(conf!='Y' && conf!='N');

            if(conf=='Y')
            {*/
  /*              int numMisspellings=0;
                do
                {
                    printf("How many misspellings (1-5)?");
                    scanf("%d", &numMisspellings);
                    while ((ch = getchar()) != '\n' && ch != EOF);
                }while(numMisspellings<1 || numMisspellings>5);
   */
          /*      while(numMisspellings > 0)
                {
                    char word[WORDLENGTH];
                    char* wdPtr = word;*/
                    int i=0;
                    if(numMisspellings !=0)
                    {
                        char** wdPtr =gentypos(numMisspellings,inputWord);     //if I understand this correctl, gentypos gives
                        struct misspelled_word* newMWord;



       //                 printf("Enter misspelling: ");
       //                 scanf("%s", word); puts next input into scanf
                         while(i<numMisspellings)                                //an array of misspelling strings
                         {
                            if((newMWord = (struct misspelled_word*) malloc(sizeof(struct misspelled_word))) == NULL)
                            {
                                printf("ERROR: OUT OF MEMORY.");
                                return;
                            }
                           // debug("wordPtr at i: %s,%d\n",*(wdPtr+i),i );
                           // debug("m_list before add:%s %s\n ",m_list->word,m_list->next->word);
                            addMisspelledWord(newMWord, newWord, *(wdPtr+i));
                            //printf("Misspelling added\n");
                            //debug("new m_list: %s %s\n",m_list->word,m_list->next->word);

                            i++;
                          }
                     }
//                    while ((ch = getchar()) != '\n' && ch != EOF);
                //    numMisspellings--;
               // }
            //}
        //}
    }
}
void freeSpace(struct Args* args)
{
    //printf("\n--------FREED WORDS--------\n");
    free(args);
    freeWords(dict->word_list);
    freeMWords(m_list);
    //free dictionary
    free(dict);
    //free m_list
    free(m_list);
}
bool foundMisspelledMatch(char* inputWord){
    struct misspelled_word* listPtr = m_list;
/*    debug("This is foundMisspelledMatch, %s\n",inputWord);
    debug("misspelled word: %s\n",listPtr->word);
    debug("misspelled word: %s\n",listPtr->next->word);
*/
    while(listPtr != NULL)
    {
        if(strcasecmp(inputWord, listPtr->word) == 0)
        {
            strcpy(inputWord, listPtr->correct_word->word);
            listPtr->misspelled = 1;
            listPtr->correct_word->misspelled_count++;
            return true;
        }
        listPtr = listPtr->next; //---> is not right, it's ->
    }
    return false;
}

bool foundDictMatch(char* inputWord){
    struct dict_word* listPtr = dict->word_list;
    while(listPtr != NULL)
    {
        if(strcasecmp(inputWord, listPtr->word) == 0)
            return true;
        listPtr = listPtr->next;
    }
    return false;
}
