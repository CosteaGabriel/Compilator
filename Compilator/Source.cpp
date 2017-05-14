#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#define SAFEALLOC(var, Type) if((var=(Type*)malloc(sizeof(Type))) == NULL) err("not enough memory");

enum{
	ID,
	CT_INT, CT_REAL, CT_CHAR, CT_STRING,
	BREAK, CHAR, INT, DOUBLE, ELSE, FOR , IF, RETURN, STRUCT, VOID, WHILE,
	COMMA, SEMICOLON, LPAR, RPAR, LBRACKET, RBRACKET, LACC, RACC,
	ADD, SUB, MUL, DIV, DOT, AND, OR, NOT, ASSIGN, EQUAL, NOTEQ, LESS, LESSEQ, GREATER, GREATEREQ,
	SPACE, LINECOMMENT, COMMENT,
	END
}; // codurile AL


char *names[] = { "ID",
"CT_INT", "CT_REAL", "CT_CHAR", "CT_STRING",
"BREAK", "CHAR", "INT", "DOUBLE", "ELSE", "FOR", "IF", "RETURN", "STRUCT", "VOID", "WHILE",
"COMMA", "SEMICOLON", "LPAR", "RPAR", "LBRACKET", "RBRACKET", "LACC", "RACC",
"ADD", "SUB", "MUL", "DIV", "DOT", "AND", "OR", "NOT", "ASSIGN", "EQUAL", "NOTEQ", "LESS", "LESSEQ", "GREATER", "GREATEREQ",
"SPACE", "LINECOMMENT", "COMMENT",
"END" };

typedef struct _Token
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
int line = 1;

char *pCrtCh;

char *int_char, *real;

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
	fprintf(stderr, "error in line %d ", tk->line);
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
	lastToken = tk;
	return tk;
}

char *createString(const char *pStartCh, char *pCrtCh)
{
	int size = (pCrtCh - pStartCh);
	int idx= 0, text_idx=0;

	int convert, caractere_skkiped = 0;
	const char *buffer;
	char *text;	

	

	if (lastToken->code == CT_STRING)
	{
		text = (char*)malloc(sizeof(char)*size);
		pStartCh++;
		while (idx < size-2)
		{
			if (*pStartCh == '\\')
			{
				buffer = pStartCh;
				buffer++;
				if (*buffer == 't'){
					text[text_idx] = '\t';
					text_idx++;
					idx+=2;
					caractere_skkiped++;
					pStartCh += 2;
				}
				else if (*buffer == 'n'){
					text[text_idx] = '\n';
					text_idx++;
					idx+=2;
					caractere_skkiped++;
					pStartCh += 2;
				}
				else if (*buffer == 'r'){
					text[text_idx] = '\r';
					text_idx++;
					idx+=2;
					caractere_skkiped++;
					pStartCh += 2;
				}
				else if (*buffer == '"'){
					text[text_idx] = '\"';
					text_idx++;
					idx+=2;
					caractere_skkiped++;
					pStartCh += 2;
				}
				else if (*buffer == '\''){
					text[text_idx] = '\'';
					text_idx++;
					idx+=2;
					caractere_skkiped++;
					pStartCh += 2;
				}
				else {
					text[text_idx] = *pStartCh;
					pStartCh++;
					idx++;
					text_idx++;
				}
			}
			else{
				text[text_idx] = *pStartCh;
				pStartCh++;
				idx++;
				text_idx++;
			}
		}
		text[size - caractere_skkiped - 2] = '\0';
	}
	else if (lastToken->code == CT_REAL || lastToken->code == CT_INT)
	{
		text = (char*)malloc(sizeof(char)*size + 1);

		while (idx < size)
		{

			text[idx] = *pStartCh;
			pStartCh++;
			idx++;
		}
	}
	else{
		text = (char*)malloc(sizeof(char)*size + 1);
		text[size] = '\0';

		while (idx <=size)
		{

			text[idx] = *pStartCh;
			pStartCh++;
			idx++;
		}
		text[size - caractere_skkiped] = '\0';
	}
	
	return text;
}

int getNextToken()
{
	int			state = 0, nCh;
	char ch;

	bool comment = false, hex = false;
	const char	*pStartCh;
	Token		*tk;

	int baza = 0, caracter = 0;

	SAFEALLOC(tk, Token);

	while (1)
	{
		ch = *pCrtCh;

		switch (state)
		{
		case 0:
			if (ch >= '1' && ch <= '9')
			{
				pStartCh = pCrtCh;
				pCrtCh++;
				state = 1;
			}
			else if (ch == '0')
			{
				pStartCh = pCrtCh;
				pCrtCh++;
				state = 2;
			}
			else if (ch == '\'')
			{
				//pStartCh = pCrtCh;
				pCrtCh++;
				state = 12;
			}
			else if (ch == '\"')
			{
				pCrtCh++;
				pStartCh = pCrtCh;
				state = 16;
			}
			else if (ch == '/')
			{
				pStartCh = pCrtCh;
				pCrtCh++;
				state = 17;
			}
			else if (ch == ',')
			{
				pCrtCh++;
				state = 35;
			}
			else if (ch == ';')
			{
				pCrtCh++;
				state = 37;
			}
			else if (ch == ' ' || ch == '\r' || ch == '\t'){
				pCrtCh++;										// consuma caracterul si ramane in starea 0
			}
			else if (isalpha(ch) || ch == '_')
			{
				pStartCh = pCrtCh;
				pCrtCh++;
				state = 22;
			}
			else if (ch == '(')
			{
				pCrtCh++;
				state = 38;
			}
			else if (ch == ')')
			{
				pCrtCh++;
				state = 39;
			}
			else if (ch == ']')
			{
				pCrtCh++;
				state = 40;
			}
			else if (ch == ']')
			{
				pCrtCh++;
				state = 41;
			}
			else if (ch == '!')
			{
				pCrtCh++;
				state = 23;
			}
			else if (ch == '<')
			{
				pCrtCh++;
				state = 24;
			}
			else if (ch == '>')
			{
				pCrtCh++;
				state = 25;
			}
			else if (ch == '{')
			{
				pCrtCh++;
				state = 48;
			}
			else if (ch == '}')
			{
				pCrtCh++;
				state = 49;
			}
			else if (ch == '+')
			{
				pCrtCh++;
				state = 50;
			}
			else if (ch == '-')
			{
				pCrtCh++;
				state = 51;
			}
			else if (ch == '*')
			{
				pCrtCh++;
				state = 52;
			}
			else if (ch == '&')
			{
				pCrtCh++;
				state = 27;
			}
			else if (ch == '|')
			{
				pCrtCh++;
				state = 28;
			}
			else if (ch == '.')
			{
				pCrtCh++;
				state = 55;
			}
			else if (ch == '=')
			{
				pCrtCh++;
				state = 26;
			}
			else if (ch == '\n'){ // tratat separat pentru a actualiza linia curenta
				line++;
				pCrtCh++;
			}
			else if (ch == '\0'){ // sfarsit de sir
				state = 59;
			}
			else tkerr(addTk(END), "caracter invalid 0\n");
			break;
		case 1:
			if (isdigit(ch))
				pCrtCh++;
			else if (ch == 'e' || ch == 'E')
			{
				pCrtCh++;
				state = 9;
			}
			else if (ch == '.')
			{
				pCrtCh++;
				state = 7;
			}
			else if (!isdigit(ch))
				state = 29;
			else tkerr(addTk(END), "caracter invalid 1");
			break;
		case 2:
			if (ch == 'x')
			{
				pCrtCh++;
				hex = true;
				state = 4;
			}
			else if (ch == '.')
			{
				pCrtCh++;
				state = 7;
			}
			else if(ch >='1' && ch<='9'){
				state = 3;
				baza = 8;
			}
			else tkerr(addTk(END), "caracter invalid 1");
			break;
		case 3:
			if (ch >= '0' && ch <= '7')
			{
				pCrtCh++;
			}
			else{
				state = 29;

			}
			break;
		case 4:
			if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
			{
				pCrtCh++;
				state = 5;
			}
			else tkerr(addTk(END), "caracter invalid 4");
			break;
		case 5:
			if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
			{
				pCrtCh++;
			}
			else
				state = 29;
			break;
		case 6:
			if (ch >= '0' && ch <= '9')
			{
				pCrtCh++;
			}
			else if (ch == '.')
			{
				pCrtCh++;
				state = 7;
			}
			else if (ch == 'e' || ch == 'E')
			{
				pCrtCh++;
				state = 9;
			}
			else tkerr(addTk(END), "caracter invalid 6\n");
			break;
		case 7:
			if (ch >= '0' && ch <= '9')
			{
				pCrtCh++;
				state = 8;
			}
			else if (ch == 'e' || ch == 'E')
			{
				pCrtCh++;
				state = 9;
			}
			else tkerr(addTk(END), "caracter invalid 7\n");
			break;
		case 8:
			if (ch >= '0' && ch <= '9')
			{
				pCrtCh++;
			}
			else if (ch == 'e' || ch == 'E')
			{
				pCrtCh++;
				state = 9;
			}
			else 
				state=30;
			break;
		case 9:
			if (ch == '-' || ch == '+')
			{
				pCrtCh++;
				state = 10;
			}
			else
				state = 10;
			break;
		case 10:
			if (ch >= '0' && ch <= '9')
			{
				pCrtCh++;
				state = 11;
			}
			else tkerr(addTk(END), "caracter invalid 10\n");
			break;
		case 11:
			if (ch >= '0' && ch <= '9')
			{
				pCrtCh++;
			}
			else
				state = 30;
			break;
		case 12:
			if (ch != '\'' && ch != '\\')
			{
				pCrtCh++;
				caracter = (int)ch;
				state = 13;
			}
			else if (ch == '\\')
			{
				pCrtCh++;
				state = 14;
			}
			else 
				tkerr(addTk(END), "caracter invalid 12\n");
			break;
		case 13:
			if (ch == '\'')
			{
				//pCrtCh++;
				state = 32;
			}
			else
				state = 16;
			break;
		case 14:
			if (ch == 'a' || ch == 'b' || ch == 'f' || ch == 'n' || ch == 'r' || ch == 't' || ch == 't' || ch == 'v' || ch == '?' || ch == '\'' || ch == '"' | ch == '\\' || ch == '0')
			{
				pCrtCh++;
				switch (ch)
				{
				case 'n':
					caracter = '\n'; break;
				case 't':
					caracter = '\t'; break;
				case 'r':
					caracter = '\r'; break;
				default:
					break;
				}
				state = 15;
			}
			else 
				tkerr(addTk(END), "caracter invalid 14\n");
			break;
		case 15:
			if (ch == '\'')
			{
				pCrtCh++;
				caracter = (int)ch;
				state = 32;
			}
			else if (ch == '\"')
			{
				pCrtCh++;
				state = 33;
			}
			else
				state = 16;
			break;
		case 16:
			if (ch != '\'' && ch != '\\')
				pCrtCh++;
			else if (ch == '\\')
			{
				pCrtCh++;
				state = 14;
			}
			if (ch == '\"')
			{
				state = 33;
			}
			break;
		case 17:
			if (ch == '/')
			{
				pCrtCh++;
				state = 20;
			}
			else if (ch == '*')
			{
				pCrtCh++;
				state = 18;
			}
			else
				state = 60;
			break;
		case 18:
			if (ch != '*')
			{
				pCrtCh++;
			}
			else
				state = 19;
			break;
		case 19:
			if (ch == '*')
			{
				pCrtCh++;
				comment = true;
			}
			else if (ch == '/' && comment)
			{
				pCrtCh++;
				state = 34;
				comment = false;
			}
			else
				state = 18;
			break;
		case 20:
			state = 21;
			break;
		case 21:
			if (ch != ' ' && ch != '\n' && ch != '\r' && ch != '\0')
				pCrtCh++;
			else 
				state = 61;
			break;
		case 22:
			if (isalnum(ch) || ch == '_')
				pCrtCh++;
			else
				state = 36;
			break;
		case 23:
			if (ch == '=')
			{
				pCrtCh++;
				state = 43;
			}
			else
				state = 42;
			break;
		case 24:
			if (ch == '=')
			{
				pCrtCh++;
				state = 45;
			}
			else
				state = 44;
			break;
		case 25:
			if (ch == '=')
			{
				pCrtCh++;
				state = 47;
			}
			else
				state = 46;
			break;
		case 26:
			if (ch == '=')
			{
				pCrtCh++;
				state = 57;
			}
			else if (ch != '=')
				state = 56;
			else 
				tkerr(addTk(END), "caracter invalid 26\n");
			break;
		case 27:
			if (ch == '&')
			{
				pCrtCh++;
				state = 53;
			}
			else
				tkerr(addTk(END), "caracter invalid 27\n");
			break;
		case 28:
			if (ch == '|')
			{
				pCrtCh++;
				state = 54;
			}
			else
				tkerr(addTk(END), "caracter invalid 28\n");
			break;
		case 29:
			unsigned long nr;

			tk = addTk(CT_INT);
			int_char = createString(pStartCh, pCrtCh);
			if (hex)
				sscanf(int_char, "%x", &tk->i);
			else if (baza == 8){
				nr = strtoul(pStartCh, &pCrtCh, 8);
				tk->i = nr;
			}
			else 
				sscanf(int_char, "%d", &tk->i);
			return CT_INT;
		case 30:
			tk = addTk(CT_REAL);
			real = createString(pStartCh, pCrtCh);
			tk->r = atof(real);
			return CT_REAL;
		case 32:
			tk = addTk(CT_CHAR);
			tk->i = caracter;
			pCrtCh++;
			return CT_CHAR;
		case 33:
			tk = addTk(CT_STRING);
			tk->text = createString(pStartCh, pCrtCh);
			return CT_STRING;
		case 34:
			tk = addTk(COMMENT);
			tk->text = createString(pStartCh, pCrtCh);
			return COMMENT;
		case 35:
			addTk(COMMA);
			return COMMA;
		case 36:
				nCh = pCrtCh - pStartCh;
				
				if (nCh == 4 && !memcmp(pStartCh, "void", 4)) tk = addTk(VOID);
				else  if (nCh == 4 && !memcmp(pStartCh, "char", 4)) tk = addTk(CHAR);
				else  if (nCh == 3 && !memcmp(pStartCh, "int", 3)) tk = addTk(INT);
				else  if (nCh == 4 && !memcmp(pStartCh, "else", 4)) tk = addTk(ELSE);
				else  if (nCh == 6 && !memcmp(pStartCh, "struct", 6)) tk = addTk(STRUCT);
				else  if (nCh == 5 && !memcmp(pStartCh, "break", 5)) tk = addTk(BREAK);
				else  if (nCh == 6 && !memcmp(pStartCh, "double", 6)) tk = addTk(DOUBLE);
				else  if (nCh == 3 && !memcmp(pStartCh, "for", 3)) tk = addTk(FOR);
				else  if (nCh == 5 && !memcmp(pStartCh, "while", 5)) tk = addTk(WHILE);
				else  if (nCh == 6 && !memcmp(pStartCh, "return", 6)) tk = addTk(RETURN);
				else  if (nCh == 2 && !memcmp(pStartCh, "if", 2)) tk = addTk(IF);
				else  {
					tk = addTk(ID);
					tk->text = createString(pStartCh, pCrtCh);
				}
			return tk->code;
		case 37:
			addTk(SEMICOLON);
			return SEMICOLON;
		case 38:
			addTk(LPAR);
			return LPAR;
		case 39:
			addTk(RPAR);
			return RPAR;
		case 40:
			addTk(LBRACKET);
			return LBRACKET;
		case 41:
			addTk(RBRACKET);
			return RBRACKET;
		case 42:
			addTk(NOT);
			return NOT;
		case 43:
			addTk(NOTEQ);
			return NOTEQ;
		case 44:
			addTk(LESS);
			return LESS;
		case 45:
			addTk(LESSEQ);
			return LESSEQ;
		case 46:
			addTk(GREATER);
			return GREATER;
		case 47:
			addTk(GREATEREQ);
			return GREATEREQ;
		case 48:
			addTk(LACC);
			return LACC;
		case 49:
			addTk(RACC);
			return RACC;
		case 50:
			addTk(ADD);
			return ADD;
		case 51:
			addTk(SUB);
			return SUB;
		case 52:
			addTk(MUL);
			return MUL;
		case 53:
			addTk(AND);
			return AND;
		case 54:
			addTk(OR);
			return OR;
		case 55:
			addTk(DOT);
			return DOT;
		case 56:
			addTk(ASSIGN);
			return ASSIGN;
		case 57:
			addTk(EQUAL);
			return EQUAL;
		case 58:
			addTk(SPACE);
			return SPACE;
		case 59:
			addTk(END);
			return END;
		case 60:
			addTk(DIV);
			return DIV;
		}
	}
}

void showAtoms()
{
	Token *tk;
	int old_line = 1;
	printf("1->");
	for (tk = tokens; tk != NULL; tk = tk->next)
	{
		if (old_line != tk->line)
		{
			printf("\n\n");
			printf("%d->", tk->line);
			old_line = tk->line;
		}
		printf("%s (%d) ",names[tk->code], tk->code);
			switch (tk->code)
		{
		case ID: printf(":\"%s\"  ", tk->text); break;
		case CT_STRING: printf(": \"%s\" ", tk->text); break;
		case CT_CHAR: printf(": \"%d\"   ", tk->i); break;
		case CT_INT: printf(": \"%d\"  ", tk->i); break;
		case CT_REAL: printf(": \"%f\"  ", tk->r); break;
		}
			
	}
	printf("\n\n");
}
int main()
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

	fclose(fis);

	return 0;
}
