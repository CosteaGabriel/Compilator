#ifndef header_h
#define header_h


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

extern char *names[];

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

extern Token *tokens;
extern Token *lastToken;


extern char input[50001];
extern int line;
extern char *pCrtCh;


void sintactic();
void showAtoms();
int getNextToken();
#endif 