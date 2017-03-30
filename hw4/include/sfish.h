#ifndef SFISH_H
#define SFISH_H
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string.h>


int isValidCmd(char** words);
void unix_error(char* msg);
pid_t Fork(void);
void cmdCd(char** words);

void cmdHelp();

void cmdPwd();
char** strSplit(char* line, const char delimiter);
#endif
