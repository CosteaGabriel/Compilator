#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "header.h"


  Token *consumedTk;


  int consume(int code){
    if(crtTk->code==code){
		consumedTk=crtTk;
		crtTk=crtTk->next;
		return 1;
    }
    return 0;
  }

int unit(){
	while(1){
		if(declStruct()){
		}
		else if(declFunc()){
		}
		else(declVar()){
		}
		else break;
	}
	if(consume(END)){
		return 1;
	}
	else tkerr(crtTk,"Missing END");

	return 0;
}

int declStruct(){
	if(consume(STRUCT)){
		if(consume(ID)){
			while(1){
				if(declVar()){
				}
				else break;
			}
			if(consume(RACC)){
				if(consume(SEMICOLON)){
					return 1;
				}else tkerr(crtTk,"Missing SEMICOLON");
			}else tkerr(crtTk,"Missing RACC");
		}else tkerr(crtTk,"Missing ID");
	}
	return 0;
}

int declVar(){
	Token *startTk = crtTk;
	if(typeBase()){
		if(consume(ID)){
			arrayDecl();
			while(1){
				if(consume(COMMA)){
					int comma = 1;
					if(consume(ID)){
						arrayDecl();
					}else tkerr(crtTk,"Missing ID");
				}else break;
			}
			if(consume(SEMICOLON)){
				return 1;
			}else tkerr(crtTk,"Missing ;");
		}else tkerr(crtTk,"Missing ID statement");
	}else tkerr(crtTk,"Missing typeBase");
	crtTk = startTk;
	return 0;
}

int typeBase(){
	
	if(consume(INT))
		return INT;
	if(consume(DOUBLE))
		return DOUBLE;
	if(consume(CHAR))
		return CHAR;
	if(consume(STRUCT))
		return STRUCT;
	return 0;
}

int arrayDecl(){
	if(consume(LBRACKET)){
		expr();
		if(consume(RBRACKET)){
			return 1;
		}else tkerr(crtTk,"Missing RBRACKET");
	}else tkerr(crtTk,"Missing LBRACKET");
	return 0; 
}

int typeName(){
	if(typeBase()){
		arrayDecl();
		return 1;
	}
	return 0;
}

int declFunc(){

}

int funcArgc(){
	if(typeBase()){
		if(consume(ID)){
			arrayDecl():
			return 1;
		}else tkerr(crtTk, "Missing ID");
	}else tkerr(crtTk,"Missing typeBase");
	return 0;
}

int stm(){

}

int stmCompound(){
	if(consume(LACC)){
		while(1){
			if(declVar()){
			}
			else if(stm()){
			}
			else break;
		}
		if(consume(RACC)){
			return 1;
		}else tkerr(crtTk,"Missing RACC");
	}else tkerr(crtTk,"Missing LACC");
	return 0;
}

int expr(){
	if(exprAssign()){
		return 1;
	}
	return 0;
}

int exprAssign(){
	if(exprUnary()){
		
	}
}

int ruleWhile(){
    Token *startTk = crtTk;
    if(consume(WHILE)){
      if(consume(LPAR)){
      	if(expr()){
      		if(consume(RPAR)){
      			if(stm()){
      				return 1;
      			}else tkerr(crtTk, "Missing WHILE statement");
      		}else tkerr(crtTk, "Missing (");
      	}else tkerr(crtTk, "Invalid expression after (");
    }else tkerr(crtTk, "Missing ) after WHILE");
  }
  crtTk = startTk;
  return 0;
  }

  int ruleFor(){
  	Token *startTk = crtTk;
  	if(consume(FOR)){
  		if(consume(LPAR)){
  			if(expr()){
  				if(consume(RPAR)){
  					if(stm()){
  						return 1;
  					}else tkerr(crtTk,"Missing FOR statement");
  				}else tkerr(crtTk, "Missing (");
  			}else tkerr(crtTk,"Invalid expression after )");
  		}else tkerr(crtTk, "Missing ) after FOR");
  	}
  	crtTk = startTk;
  	return 0;
  }
