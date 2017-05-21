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

int exprPrimary(){
	if(consume(ID)){
		if(consume(LPAR)){
			if(expr()){
				while(1){
					if(consume(COMMA)){
						if(expr()){
						}else tkerr (crtTk,"Missingexpr after ,");
					}else break;
				}
			}
			if(consume(RPAR)){
				return 1;
			}else tkerr(crtTk,"Missing ) AFTER (");
		}else return 1;
	}else if(consume(CT_INT)){
		return 1;
	}else if(consume(CT_REAL)){
		return 1;
	}else if(consume(CT_CHAR)){
		return 1;
	}else if(consume(CT_STRING)){
		return 1;
	}else if(consume(LPAR)){
		if(expr()){
			if(consume(RPAR)){
				return 1;
			}else tkerr(crtTk,"Missing ) after expr");
		}else tkerr(crtTk,"Missing expr after (");
	}
	return 0;
}

int exprPostfix(){
	if(exprPrimary()){
		exprPostfixAux();
		return 1;
	}
	return 0;
}

int exprPostfixAux(){
	if(consume(LBRACKET)){
		if(expr()){
			if(consume(RBRACKET)){
				exprPostfixAux();
				return 1;
			}else tkerr(crtTk,"Missing ] after expr");
		}else tkerr(crtTk,"Missing expr after [");
	}else if(consume(DOT)){
		if(consume(ID)){
			exprPostfixAux();
			return 1;
		}else tkerr(crtTk,"Missing ID after .");
	}
	return 1;
}

int exprUnary(){
	if(consume(SUB)){
		if(exprUnary()){
			return 1;
		}else if(exprPostfix()){
			return 1;
		}else tkerr(crtTk,"Missing exprUnary or exprPostfix after -");
	}else if(consume(NOT)){
		if(exprUnary()){
			return 1;
		}else if(exprPostfix()){
			return 1;
		}else tkerr(crtTk,"Missing exprUnary or exprPostfix after -");
	}
	return 0;
}

int exprCast(){
	if(consume(LPAR)){
		if(typeName()){
			if(consume(RPAR)){
				if(exprCast()){
					return 1;
				}else if(exprUnary()){
					return 1;
				}
			}else tkerr(crtTk,"Missing exprCast after )");
		}else tkerr(crtTk,"Missing typeName after (");
	}
	return 0;
}


int exprMulAux(){
	if(consume(MUL)){
		if(exprCast()){
			exprMulAux();
			return 1;
		}else tkerr(crtTk, "Missing exprCast after *");
	}else if(consume(DIV)){
		if(exprCast()){
			exprMulAux();
			return 1;
		}else tkerr(crtTk, "Missing exprCast after *");
	}
	return 1;
}


int exprMul(){
	if(exprCast()){
		exprMulAux();
		return 1;
	}
	return 0;
}

int exprAddAux(){
	if(consume(ADD)){
		if(exprMul()){
			exprAddAux();
			return 1;
		}else tkerr(crtTk, "Missing ExprMul after +");
	}else if(consume(SUB)){
		if(exprMul()){
			exprAddAux();
			return 1;
		}else tkerr(crtTk, "Missing ExprMul after -");
	}
	return 1;
}

int exprAdd(){
	if(exprMul()){
		exprAddAux();
		return 1;
	}
	return 0;
}

int exprRelAux(){
	if(consume(LESS)){
		if(exprAdd()){
			exprRelAux();
			return 1;
		}else tkerr(crtTk, "Missing ExprAdd after <");
	}else if(consume(LESSEQ)){
		if(exprAdd()){
			exprRelAux();
			return 1;
		}else tkerr(crtTk, "Missing ExprAdd after <=");
	}else if(consume(GREATER)){
		if(exprAdd()){
			exprRelAux();
			return 1;
		}else tkerr(crtTk, "Missing ExprAdd after >");
	}else if(consume(GREATEREQ)){
		if(exprAdd()){
			exprRelAux();
			return 1;
		}else tkerr(crtTk, "Missing ExprAdd after <");
	}
	return 1;
}

int exprRel(){
	if(exprAdd()){
		exprRelAux();
		return 1;
	}
	return 0;
}

int exprEqAux(){
	if(consume(EQUAL)){
		if(exprRel()){
			exprEqAux();
			return 1;
		}else tkerr(crtTk, "Missing ExprRel after =");
	}else if(consume(NOTEQ)){
		if(exprRel()){
			exprEqAux();
			return 1;
		}else tkerr(crtTk, "Missing ExprRel after !=");
	}
	return 1;
}

int exprEq(){
	if(exprRel()){
		exprEqAux();
		return 1;
	}
	return 0;
}

int exprAndAux(){

	if(consume(AND)){
		if(exprEq()){
			exprAndAux();
			return 1;
		}else tkerr(crtTk, "Missing ExprEq after &&");
	}
	return 1;
}

int exprAnd(){
	if(exprEq()){
		exprAndAux();
		return 1;
	}
	return 0;
}

int exprOrAux(){
	if(consume(OR)){
		if(exprAnd()){
			exprOrAux();
			return 1;
		}else  tkerr(crtTk, "Missing ExprAnd after ||");
	}
	return 1;
}

int exprOr(){
	if(exprAnd()){
		exprOrAux();
		return 1;	
	}
	return 0;
}

int exprAssign(){
	if(exprUnary()){
		if(consume(ASSIGN)){
			if(exprAssign()){
				return 1;
			}else if(exprOr()){
				return 1;
			}else tkerr(tkerr,"Missing exprAssign or exprOr");
		}else tkerr(crtTk,"Missing ASSIGN"); 
	}else tkerr(crtTk,"Missing exprUnary");
	return 0;
}

int expr(){
	if(exprAssign()){
		return 1;
	}
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

int declFunc(){

}

int funcArgc(){
	if(typeBase()){
		if(consume(ID)){
			arrayDecl();
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

int unit(){
	while(1){
		if(declStruct()){
		}
		else if(declFunc()){
		}
		else if(declVar()){
		}
		else break;
	}
	if(consume(END)){
		return 1;
	}
	else tkerr(crtTk,"Missing END");

	return 0;
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
