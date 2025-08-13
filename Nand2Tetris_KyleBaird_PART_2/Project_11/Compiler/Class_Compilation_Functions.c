#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>

#include "GLOBALS.h"
#include "Token_Creation_Functions.h"
#include "Class_Compilation_Functions.h"

char token[LEN];
int stringFlag;

void printXmlToken(int tokenDefinedbyX, FILE *outXML){	
	if(tokenDefinedbyX == SYMBOL){
		char c = symbol(token);
		if(c == '<'){
			fprintf(outXML, "<symbol> &lt; </symbol>\n");
		}else if(c == '>'){
			fprintf(outXML, "<symbol> &gt; </symbol>\n");
		}else if(c == '&'){
			fprintf(outXML, "<symbol> &amp; </symbol>\n");
		}else{
			fprintf(outXML, "<symbol> %c </symbol>\n",c);
		}
	}else if(tokenDefinedbyX == KEYWORD){				
		int y = keyword(token);
		if( y == CLASS){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}else if(y == METHOD ){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == FUNCTION){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == CONSTRUCTOR){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == INTE){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);	
		}
		else if(y == BOOLEA){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == CHARA){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == VOIDS){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == VAR){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == STATIC){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == FIELD){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == LET){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == DO){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == IF){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == ELSE){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == WHILE){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == FALS){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == RETURN){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == NUL){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == THIS){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
	}else if(tokenDefinedbyX == STRING_CONST){
		fprintf(outXML, "<stringConstant> %s </stringConstant>\n",token);
	}else if(tokenDefinedbyX == INT_CONST){
		fprintf(outXML, "<intConstant> %s </intConstant>\n",token);
	}else if(tokenDefinedbyX == IDENTIFIER){
		fprintf(outXML, "<identifier> %s </identifier>\n",token);
	}
}	

void process(char *process,  FILE *in,  FILE *out) {
	
	if(strcmp(token,process)==0) {

		int x = tokenType(token, stringFlag);
		printXmlToken(x, out);//might not need this for compiler??

		printf("Before advance: token = '%s'\n", token);
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
			printf("After advance: token = '%s'\n", token);
		}
		else{
        // Debug info: print error message showing expected vs actual token
        printf("Syntax error: expected token '%s' but could not advance\n", process);
		}
	}else{
		fprintf(out,"Syntax error: expected token '%s' but found '%s'\n", process,token);
	}
}
void compileExpressionList(FILE *in,FILE *out){
	printf("<expressionList>\n");
	fprintf(out, "<expressionList>\n");
	
	if(strcmp(token,")")==0){
		fprintf(out, "</expressionList>\n");
		printf("</expressionList>\n");
		return;
	}
	compileExpression(in,out);
	while(1){
		if(strcmp(token,",")==0){
			process(",",in,out);
			//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		   //printf("token read: %s\n",token );
		//}
			compileExpression(in,out);
		}else{
		break;
		}
	}
	fprintf(out, "</expressionList>\n");
	printf("</expressionList>\n");
}


//term: integerConstant |stringConstant |keywordConstant |varName |varName '[' expression ']'| 
//	'(' expression ')' |(unaryOp term) |subroutineCall
void compileTerm(FILE *in, FILE *out){
	fprintf(out, "<term>\n");
	printf("<term>\n");
	

	int x = tokenType(token, stringFlag);

	if(x == INT_CONST){
		fprintf(out, "<integerConstant> %s </integerConstant>\n",token);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//    printf("token read: %s\n",token );
		//}
	}else if(x == STRING_CONST){
		fprintf(out, "<stringConstant> %s </stringConstant>\n",token);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//    printf("token read: %s\n",token );
	}else if(strcmp(token, "true")==0||strcmp(token,"false")==0||
	strcmp(token,"null")==0||strcmp(token,"this")==0){
		fprintf(out, "<keyword> %s </keyword>\n",token);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//    printf("token read: %s\n",token );
		//}
	}else if(x == IDENTIFIER){

		char identifier[250];
		strcpy(identifier, token); // save it

		int flagL1 = 0;
		char tokenL1[250];

		long savedPos = ftell(in);  // save current file position
		if(hasMoreTokens(in) && advance(in, tokenL1, &flagL1)){	// lookahead	
			fseek(in, savedPos, SEEK_SET); // restore position
		}

		//these two are the irregular lookahead. We must lookahead to determine wether we are
		//dealing with an array[] or a subroutineCall. We lookead only when token is a varName
		//or subroutine name. In other words, when we have an identifier as our term.



		if(strcmp(tokenL1,"[")==0){
			fprintf(out, "<identifier> %s </identifier>\n",identifier);
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process("[", in, out);
			compileExpression(in,out);
			process("]", in, out);
			fprintf(out, "</term>\n");
			return;
		}else if(strcmp(tokenL1,"(")==0){
			fprintf(out, "<identifier> %s </identifier>\n",identifier);
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process("(", in,out);
			compileExpressionList(in, out);
			process(")", in, out);
			fprintf(out, "</term>\n");
			return;

		}else if(strcmp(tokenL1,".")==0){
			
			fprintf(out, "<identifier> %s </identifier>\n",identifier);
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process(".", in, out);
			fprintf(out, "<identifier> %s </identifier>\n",token);
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process("(", in,out);
			compileExpressionList(in,out);
			process(")", in,out);
			fprintf(out, "</term>\n");
			return;
		}else {
			fprintf(out, "<identifier> %s </identifier>\n", identifier);
			strcpy(token, tokenL1);  // restore token for parent context
			stringFlag = flagL1;
		}

	}else if(strcmp(token,"~")==0 || strcmp(token,"-")==0){
		fprintf(out, "<symbol> %s </symbol>\n",token);
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    		printf("token read: %s\n",token );
		}
		compileTerm(in,out);
		fprintf(out, "</term>\n");
		return;
	}
	else if(strcmp(token,"(")==0){
		process("(",in,out);
		compileExpression(in, out);
		process(")", in, out);
		fprintf(out, "</term>\n");
		return;
	}else {
    	fprintf(stderr, "Syntax error in term: unexpected token '%s'\n", token);
	}
	if (hasMoreTokens(in)) {
		advance(in, token, &stringFlag);
	}
	fprintf(out, "</term>\n");
	printf("</term>\n");
}

//expression: term(op term)*
void compileExpression(FILE *in, FILE *out){
	fprintf(out, "<expression>\n");
	printf("<expression>\n");

	compileTerm(in,out);
	
	while(strcmp(token,"+")==0 || strcmp(token,"-")==0 || strcmp(token,"*")==0 ||
		  strcmp(token,"/")==0 || strcmp(token,"&")==0 || strcmp(token,"|")==0 ||
		  strcmp(token,"<")==0 || strcmp(token,">")==0 || strcmp(token,"=")==0) {
		
		if(strcmp(token,"<")==0){
			fprintf(out, "<symbol> &lt; </symbol>\n");
		}
		else if(strcmp(token,">")==0){
			fprintf(out, "<symbol> &gt; </symbol>\n");
		}else if(strcmp(token,"&")==0){
			fprintf(out, "<symbol> &amp; </symbol>\n");
		}else{
			fprintf(out, "<symbol> %s </symbol>\n", token);  
		}
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			//fprintf(out,"token read after symbol: %s\n",token );
			}

		compileTerm(in, out);
	}

	fprintf(out, "</expression>\n");
	printf("</expression>\n");
}



/*  letStatement: 'let' varName ('[' expression ']')? '=' expression ';'  */
void compileLet(FILE *in,FILE *out){

	fprintf(out, "<letStatement>\n");
	printf("<letStatement>\n");
	//let
	process("let",in,out);

	//varName
	int x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out,"<identifier> %s </identifier>\n",token);
	}else{
		printf("ERROR\n");
	}

	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}


	if(strcmp(token,"[")==0){
		process("[",in,out);
		compileExpression(in, out);
		process("]",in,out);
	}

			
	//equal sign
	if(strcmp(token,"=")==0){
		process("=", in, out);
	
		compileExpression(in,out);
		//;
		if(strcmp(token,";")==0){
		process(";",in,out);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//	printf("After advance: token = '%s'\n", token);
		//}
		//fprintf(out,"process ; complete. token now =%s\n",token );
		}
	}
	else if(strcmp(token,";")==0){
		process(";", in, out);
	}

	fprintf(out, "</letStatement>\n");
	printf("</letStatement>\n");
}

void compileIf(FILE *in,FILE *out){
	fprintf(out, "<ifStatement>\n");
	printf("<ifStatement>\n");
	process("if",in,out);
	process("(",in,out);
	compileExpression(in,out);
	process(")",in,out);
	process("{",in,out);
	compileStatements(in,out);
	process("}",in,out);
	if(strcmp(token,"else")==0){
		process("else",in,out);
		process("{",in,out);
		compileStatements(in,out);
		process("}",in,out);
	}
	fprintf(out, "</ifStatement>\n");
	printf("</ifstatement>\n");
}

void compileWhile(FILE * in,FILE *out){
	fprintf(out, "<whileStatement>\n");
	printf("<whileStatement>\n");

	process("while",in,out);
	process("(",in,out);
	compileExpression(in,out);
	process(")",in,out);
	process("{",in,out);
	compileStatements(in,out);
	process("}",in,out);
	fprintf(out, "</whileStatement>\n");
	printf("</whileStatement>\n");
}

void compileDo(FILE *in,FILE *out){
	fprintf(out, "<doStatement>\n");
	printf("<doStatement>\n");
	process("do",in,out);
	fprintf(out, "<identifier> %s </identifier>\n",token);
	
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}

	if(strcmp(token,"(")==0){
		process("(", in,out);
		compileExpressionList(in, out);
		process(")", in, out);

	}else if(strcmp(token,".")==0){

		process(".", in, out);
		fprintf(out, "<identifier> %s </identifier>\n",token);
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
   			printf("token read: %s\n",token );
		}
		process("(", in,out);
		compileExpressionList(in,out);
		process(")", in,out);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			//fprintf(out,"token read after symbol: %s\n",token );
		//	}
	}
	process(";",in,out);
	fprintf(out, "</doStatement>\n");
	printf("</doStatement>\n");
}

void compileReturn(FILE * in, FILE *out){
	fprintf(out, "<returnStatement>\n");
	printf("<returnStatement>\n");
	process("return", in,out);
	
	int x = tokenType(token, stringFlag);

	if(x == INT_CONST||x == STRING_CONST|| x == IDENTIFIER ||
	strcmp(token, "true")==0||strcmp(token,"false")==0||
	strcmp(token,"null")==0||strcmp(token,"this")==0){
		
		compileExpression(in,out);
	}else if(strcmp(token,";")==0){
		process(";",in,out);
	}

	if(strcmp(token,";")==0){
		process(";",in,out);
	}
	fprintf(out, "</returnStatement>\n");
	printf("</returnStatement>\n");
}

void compileStatements(FILE *in,FILE *out){
	printf("<statements>\n");
	fprintf(out, "<statements>\n");

	//while (strcmp(token, "let") == 0 || strcmp(token, "if") == 0 ||
   // strcmp(token, "while") == 0 || strcmp(token, "do") == 0 ||
    //strcmp(token, "return") == 0) {
		
		while(1){
			if(strcmp(token, "let")==0){
				compileLet(in,out);
			}else if(strcmp(token, "if")==0){
				compileIf(in, out);
			}else if(strcmp(token, "while")==0){
				compileWhile(in, out);
			}else if(strcmp(token, "do")==0){
				compileDo(in, out);
			}else if(strcmp(token, "return")==0){
				compileReturn(in, out);
			}else{
				break;
			}
		}
	//}
	fprintf(out, "</statements>\n");
	printf("</statements\n");
}

void compileParamaterList(FILE *in, FILE *out){
	fprintf(out, "<parameterList>\n");
	printf("<parameterList>\n");
	int x = tokenType(token, stringFlag);
	//type
	if(strcmp(token, "int")==0 || strcmp(token, "char")==0
	|| strcmp(token, "boolean")==0){
		fprintf(out, "<keyword> %s </keyword>\n", token);
	}else if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
	}else if(strcmp(token,")")==0){
		fprintf(out, "</parameterList>\n");
		return;
	}	
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	//varName
	x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}
	while(1){
		if(strcmp(token,",")==0){
			process(",",in,out);
			x = tokenType(token, stringFlag);
			if(strcmp(token, "int")==0 || strcmp(token, "char")==0
			|| strcmp(token, "boolean")==0){
				fprintf(out, "<keyword> %s </keyword>\n", token);
			}else if(x == IDENTIFIER){
				fprintf(out, "<identifier> %s </identifier>\n", token);
			}
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			//varName
			x = tokenType(token, stringFlag);
			if(x == IDENTIFIER){
				fprintf(out, "<identifier> %s </identifier>\n", token);
			}
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
		}else if(strcmp(token,")")==0){
			fprintf(out, "</parameterList>\n");
			return;
		}
	}

	fprintf(out, "</parameterList>\n");
	printf("</parameterList>\n");
}
void compileVarDec(FILE *in, FILE *out){

	fprintf(out, "<varDec>\n");
	printf("<varDec>\n");
	//var
	process("var", in, out);
	
	//type

	int x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
		

	}else if(strcmp(token, "int")==0||strcmp(token,"char")==0||
		strcmp(token, "boolean")==0){
		fprintf(out, "<keyword> %s </keyword>\n", token);
	}
	//varName
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}
	x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    		printf("token read: %s\n",token );
		}
	}
	
	while(1){
		if(strcmp(token,",")==0){
			process(",", in, out);

			x = tokenType(token, stringFlag);
			if(x == IDENTIFIER){
				fprintf(out, "<identifier> %s </identifier>\n", token);
			}
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
			    printf("token read: %s\n",token );
			}
		}else if(strcmp(token,";")==0){
			process(";", in, out);
			break;
		}
	}
	fprintf(out, "</varDec>\n");
	printf("</varDec>\n");
}
//should I pass the symbolTable variables as arguments into compileClassVarDec?
//	char classSymbolTableName[250];
	//char classSymbolTableType[250];
	//char classSymbolTableKind[250];
	//int classSymbolTable#Static;
	//int classSymbolTable#Field;

	//char subroutineSymbolTableName[250];
	//char subroutineSymbolTableType[250];
	//char subroutineSymbolTableKind[250];
	//int subroutineSymbolTable#Static;
	//int subroutineSymbolTable#Field;

	//possibly have two flags to show if inside a class or inside a subroutine
	//int flagSymbolTableClass;
	//int flagSymbolTableSubroutine;

	//flag to show usage is declared or used
	//int symbolTableUsedorDeclared;

//classVarDec:('static'|'field') type varName (';'varName)*
//type: 'int'|'char'|'boolean'| className
//className:identifier
//varName:identifier


void compileClassVarDec(FILE *in, FILE *out, char *token){
	fprintf(out, "<classVarDec>\n"); //will remove for compiler to generate only VM code
	printf("<classVarDec>\n");		 //will remove for compiler to generate only VM code
	
	//('static'|'field')
	int x = tokenType(token, stringFlag);
	if(strcmp(token, "static")==0){
		process("static", in, out);
		//int classSymbolTable#Static;
	}else if(strcmp(token, "field")==0){
		process("field", in, out);
		//int classSymbolTable#Field;
	}
	
	//type:'int'|'char'|'boolean'
	x = tokenType(token, stringFlag);
	//break up keyword more specifically to  find the specific type
	if (x == KEYWORD) {
		fprintf(out, "<keyword> %s </keyword>\n", token);
	}//type:className(className:identifier)
	else if(x ==  IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		printf("token read: %s\n",token );//will remove for compiler to generate only VM code
	}
	
	//varName (';'varName)*
	while(1){
		x = tokenType(token, stringFlag);
		if(x == IDENTIFIER){
			fprintf(out, "<identifier> %s </identifier>\n", token);
		}
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
			printf("token read: %s\n",token );
		}
		if(token[0] == ';'){
			process(";",in, out);
			break;
		}
		else if(token[0] == ','){
			process(",",in, out);
			continue;
		}
	}	
	fprintf(out, "</classVarDec>\n");
	printf("</classVarDec>\n");
}

//subroutineBody: '{' varDec* statements '}'
void compileSubroutineBody(FILE *in,FILE *out){
	fprintf(out, "<subroutineBody>\n");
	printf("<subroutineBody>\n");
	process("{", in, out);

	while(1){
		if(strcmp(token, "var")==0){
			compileVarDec(in, out);
		}else if(strcmp(token, "let")==0){
			compileStatements(in,out);
		}else if(strcmp(token, "if")==0){
			compileStatements(in, out);
		}else if(strcmp(token, "while")==0){
			compileStatements(in, out);
		}else if(strcmp(token, "do")==0){
			compileStatements(in, out);
		}else if(strcmp(token, "return")==0){
			compileStatements(in, out);
		}else{
			break;
		}
	}
	process("}",in, out);
	fprintf(out, "</subroutineBody>\n");
	printf("</subroutineBody>\n");
}


void compileSubroutineDec(FILE *in, FILE *out){
	fprintf(out, "<subroutineDec>\n");
	printf("<subroutineDec>\n");
	//printf("Calling subroutineDec\n");
	fprintf(out, "<keyword> %s </keyword>\n", token);

	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	int x = tokenType(token, stringFlag);
	if(strcmp(token, "void")==0 || strcmp(token, "int")==0 ||
		strcmp(token, "char")==0 || strcmp(token, "boolean")==0){

		fprintf(out, "<keyword> %s </keyword>\n", token);
	}else if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
	}

	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	
	process( "(" , in, out);
	compileParamaterList(in, out);
	process( ")" , in, out);
	
	compileSubroutineBody(in, out);
		
	fprintf(out, "</subroutineDec>\n");
	printf("</subroutineDec>\n");
}
//<identifier> name:%s, type:%s, kind:%s, #:%d, category:%s, usage:%s </identifier>
//<identifier> name:%s, type:%s(class), kind(none):%s, #:%d(none), category:%s, usage:%s </identifier>

/*according to Compiling Classes(project 11, when starting to compile a class, compiler creates a class-level
symbol table and adds to it the field and static variables declared in the class declaration. Compiler also
creates an empty subroutine-level symbol table. No code is generated*/
void compileClass(FILE *in, FILE *out){
	fprintf(out, "<class>\n");
	//printf("<class>\n");

	

	while(hasMoreTokens(in)) {
    	advance(in, token, &stringFlag); // prime the first token
    	if (strlen(token) > 0) break;  	
	}
	
	process("class", in, out);
	//should I add the the two symbol tables as local variables to compileClass function?
	//char classSymbolTableName[250];
	//char classSymbolTableType[250];
	//char classSymbolTableKind[250];
	//int classSymbolTable#Static;
	//int classSymbolTable#Field;

	//char subroutineSymbolTableName[250];
	//char subroutineSymbolTableType[250];
	//char subroutineSymbolTableKind[250];
	//int subroutineSymbolTable#Static;
	//int subroutineSymbolTable#Field;

	//possibly have two flags to show if inside a class or inside a subroutine
	//int flagSymbolTableClass;
	//int flagSymbolTableSubroutine;

	//flag to show usage is declared or used
	//int symbolTableUsedorDeclared;


	int x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
		printf("token read: %s\n",token );
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	process("{", in, out);

	while(1){
		if(strcmp(token, "static")==0 || strcmp(token, "field")==0){
			compileClassVarDec(in,out,token);
		}else{
			break;
		}		
	}
	while(1){
		//printf("Calling subroutineDec\n");
		if(strcmp(token, "constructor") == 0 || 
    	strcmp(token,"function") == 0 || 
    	strcmp(token,"method") == 0) {
    		compileSubroutineDec(in, out);
		}else{
			break;
		}
	}	
	process("}", in, out);
	fprintf(out, "</class>\n");
	printf("</class>\n");
}
