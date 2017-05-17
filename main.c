#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "header.h"


char *names[] = { "ID",
"CT_INT", "CT_REAL", "CT_CHAR", "CT_STRING",
"BREAK", "CHAR", "INT", "DOUBLE", "ELSE", "FOR", "IF", "RETURN", "STRUCT", "VOID", "WHILE",
"COMMA", "SEMICOLON", "LPAR", "RPAR", "LBRACKET", "RBRACKET", "LACC", "RACC",
"ADD", "SUB", "MUL", "DIV", "DOT", "AND", "OR", "NOT", "ASSIGN", "EQUAL", "NOTEQ", "LESS", "LESSEQ", "GREATER", "GREATEREQ",
"SPACE", "LINECOMMENT", "COMMENT",
"END" };

Token *tokens = NULL;
Token *lastToken = NULL;
Token *crtTk = NULL;

char input[50001];
int line = 1;
char *pCrtCh;



 main()
{
	FILE *fis;
	int err;
	if ((fis = fopen("8.txt", "r")) == NULL){
		perror("Fisier inexistent");
		exit(1);
	}

	int nc = fread(input, sizeof(char), 50000, fis);

	input[nc] = '\0';
	pCrtCh = input;

	do{
		getNextToken();
	} while (lastToken->code != END);

	showAtoms();
	printf("urmeaza sintactic\n");
	

	fclose(fis);

	return 0;
}