#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "header.h"



void sintactic(){
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