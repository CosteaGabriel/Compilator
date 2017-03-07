#include <stdio.h>
#include <stdlib.h>

#define SAFEALLOC(var, Type) if((var=(Type*)malloc(sizeof(Type))) == NULL) err("not enough memory");

enum{
	ID,
	CT_INT, CT_REAL, CT_CHAR, CT_STRING,
	COMMA, SEMICOLON, LPAR, RPAR, LBRACKET, RBRACKET, LACC, RACC,
	ADD, SUB, MUL, DIV, DOT, AND, OR, NOT, ASSIGN, EQUAL, NOTEQ, LESS, LESSEQ, GREATER, GREATEREQ,
	SPACE, LINECOMMENT, COMMENT,
	END
}; // codurile AL

typedef struct _TOKEN
{
	int code;				// codul (numele)

	union
	{
		char *text;			// folosit pentru ID, CT_STRING (alocat dinamic)
		long int i;			// folosit pentru CT_INT, CT_CHAR
		double r;			// folosit pentru CT_REAL
	};

	int line;				// linia din fisierul de intrare
	struct _Token *next;	// inlantuire la urmatorul AL
}Token;

char input[50001];
int line;

char *pCrtCh;

Token *tokens = NULL;
Token *lastToken = NULL;

void err(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "error: ");
	vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
	va_end(va);
	exit(-1);
}

void tkerr(const Token *tk, const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	fprintf(stderr, "error in line %d", tk->line);
	vfprintf(stderr, fmt, va);
	fputc('\n', stderr);
	va_end(va);
	exit(-1);
}

Token *addTk(int code)
{
	Token *tk;

	SAFEALLOC(tk, Token);

	tk->code = code;
	tk->line = line;
	tk->next = NULL;

	if (lastToken)
	{
		lastToken->next = tk;
	}
	else
	{
		tokens = tk;
	}

	return tk;
}

int getNextToken()
{
	int			state = 0, nCh;
	char		*ch;
	const char	*pStartCh;
	Token		*tk;

	while (1)
	{
		ch = *pCrtCh;

		switch (state)
		{
		case 0:
			if (isalpha(ch) || ch == '_')
			{
				pStartCh = pCrtCh;
				pCrtCh++;
				//state = ;
			}
			else tkerr(addTk(END), "caracter invalid");
			break;

		}
	}
}

int main()
{
	FILE *fis;

	if ((fis = fopen("pr.c", "r")) == NULL){
		perror("Fisier inexistent");
		exit(1);
	}

	int nc = fread(input, sizeof(char), 50000, fis);

	input[nc] = '\0';
	fclose(fis);

	return 0;
}

