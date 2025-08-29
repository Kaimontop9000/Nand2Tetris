#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>

#include "GLOBALS.h"
#include "Token_Creation_Functions.h"
#include "Class_Compilation_Functions.h"
#include "Symbol_Table.h"
#include "VM_Writer.h"

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

void process(char *process,  FILE *in,  FILE *outXML) {
	
	if(strcmp(token,process)==0) {

		int x = tokenType(token, stringFlag);
		printXmlToken(x, outXML);//might not need this for compiler??

		printf("Before advance: token = '%s'\n", token);
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
			printf("After advance: token = '%s'\n", token);
		}
		else{
        // Debug info: print error message showing expected vs actual token
        printf("Syntax error: expected token '%s' but could not advance\n", process);
		}
	}else{
		fprintf(outXML,"Syntax error: expected token '%s' but found '%s'\n", process,token);
	}
}
int compileExpressionList(FILE *in,FILE *outXML,FILE *outVM, SymbolTable *subroutineTable, SymbolTable *classTable,
	const char *className){
	printf("<expressionList>\n");
	fprintf(outXML, "<expressionList>\n");
	
	int nArgs = 0;

	// case: empty expression list
	if(strcmp(token,")")==0){
		fprintf(outXML, "</expressionList>\n");
		printf("</expressionList>\n");
		return nArgs;
	}
	//first expression
	compileExpression(in,outXML,outVM, subroutineTable, classTable,className);
	nArgs++;

	// additional expressions
	while(1){
		if(strcmp(token,",")==0){
			process(",",in,outXML);
			//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		   //printf("token read: %s\n",token );
		//}
			compileExpression(in,outXML,outVM,subroutineTable, classTable,className);
			nArgs++;
		}else{
		break;
		}
	}
	fprintf(outXML, "</expressionList>\n");
	printf("</expressionList>\n");
	return nArgs;
}


//term: integerConstant |stringConstant |keywordConstant |varName |varName '[' expression ']'| 
void compileTerm(FILE *in, FILE *outXML,FILE *outVM,SymbolTable *subroutineTable, SymbolTable *classTable,
	 const char *className){
	fprintf(outXML, "<term>\n");
	printf("<term>\n");
	

	int x = tokenType(token, stringFlag);

	if(x == INT_CONST){
		fprintf(outXML, "<integerConstant> %s </integerConstant>\n",token);
		fprintf(outVM,"push constant %s\n", token);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//    printf("token read: %s\n",token );
		//}
	}else if(x == STRING_CONST){
		fprintf(outXML, "<stringConstant> %s </stringConstant>\n",token);
		int len = strlen(token);
		fprintf(outVM, "push constant %d\n", len);
		fprintf(outVM,"call String.new 1\n");

		for(int i = 0;i < len; i++){
			fprintf(outVM, "push constant %d\n",token[i]);
			fprintf(outVM, "call String.appendChar 2\n");
		}
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//    printf("token read: %s\n",token );
	}else if(strcmp(token, "true")==0){
		fprintf(outXML, "<keyword> %s </keyword>\n",token);
		fprintf(outVM, "push constant 0 \n");
		fprintf(outVM,"not\n");
	}else if(strcmp(token,"false")==0||strcmp(token,"null")==0){
		fprintf(outXML, "<keyword> %s </keyword>\n",token);
		fprintf(outVM, "push constant 0\n");
	}else if(strcmp(token,"this")==0){
		fprintf(outXML, "<keyword> %s </keyword>\n",token);
		fprintf(outVM, "push pointer 0\n");
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//    printf("token read: %s\n",token );
		//}

	// identifier (varName, subroutineCall, array access)
	}else if(x == IDENTIFIER){
		printf("token when we know its an identifier :%s\n\n",token);
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

		// Lookahead for array or subroutine call

		if(strcmp(tokenL1,"[")==0){
			// array access
			fprintf(outXML, "<identifier name=\"%s\" category=\"%s\" index=\"%d\" usage=\"used\"/>\n",
       		identifier,
	        (kindOf(subroutineTable, identifier) == KIND_STATIC ? "static" :
	        kindOf(subroutineTable, identifier) == KIND_FIELD  ? "field"  :
	        kindOf(subroutineTable, identifier) == KIND_ARG    ? "arg"    :
	        kindOf(subroutineTable, identifier) == KIND_VAR    ? "var"    :
	        kindOf(classTable, identifier) == KIND_STATIC ? "static" :
	        kindOf(classTable, identifier) == KIND_FIELD  ? "field"  :
	        kindOf(classTable, identifier) == KIND_ARG    ? "arg"    :
	        kindOf(classTable, identifier) == KIND_VAR    ? "var"    : "none"),
	        (kindOf(subroutineTable, identifier) != KIND_NONE ? indexOf(subroutineTable, identifier) :
	        kindOf(classTable, identifier) != KIND_NONE ? indexOf(classTable, identifier) : -1)
    		);		

    		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process("[", in, outXML);
			compileExpression(in,outXML,outVM, subroutineTable, classTable,className);
			process("]", in, outXML);
			fprintf(outXML, "</term>\n");
			return;

		}else if(strcmp(tokenL1,"(")==0){
			printf("tokenL1 when we check if tokenL1 is ( , tokenL1 ; %s, token ; %s\n\n", tokenL1, token);
			fprintf(outXML, "<identifier> %s </identifier>\n",identifier);

			// advance past '('
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process("(", in,outXML);

			int nArgs = compileExpressionList(in, outXML,outVM, subroutineTable, classTable, className);
			process(")", in, outXML);

			// Minimal change: generate VM call for this function
	    	char fullName[512];
    		snprintf(fullName, sizeof(fullName), "%s.%s", className, identifier);  // assume it's in current class
   		 	
   		 	writeCall(outVM, fullName, nArgs);


			fprintf(outXML, "</term>\n");
			return;

		}else if(strcmp(tokenL1,".")==0){
		    // --- OBJECT.METHOD(...) or CONSTRUCTOR HANDLING ---
		    // token = identifier before '.'
		    printf("tokenL1 is . \n tokenl1 ; %s token ; %s\n", tokenL1, token);
		    fprintf(outXML, "<identifier name=\"%s\" category=\"%s\" index=\"%d\" usage=\"used\"/>\n",
		        identifier,
		        (kindOf(subroutineTable, identifier) == KIND_STATIC ? "static" :
		        kindOf(subroutineTable, identifier) == KIND_FIELD  ? "field"  :
		        kindOf(subroutineTable, identifier) == KIND_ARG    ? "arg"    :
		        kindOf(subroutineTable, identifier) == KIND_VAR    ? "var"    :
		        kindOf(classTable, identifier) == KIND_STATIC ? "static" :
		        kindOf(classTable, identifier) == KIND_FIELD  ? "field"  :
		        kindOf(classTable, identifier) == KIND_ARG    ? "arg"    :
		        kindOf(classTable, identifier) == KIND_VAR    ? "var"    : "none"),
		        (kindOf(subroutineTable, identifier) != KIND_NONE ? indexOf(subroutineTable, identifier) :
		        kindOf(classTable, identifier) != KIND_NONE ? indexOf(classTable, identifier) : -1)
		    );

		    // Advance to the subroutine/method name
    		if(hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		        printf("DEBUG: token before process . is %s\n", token);
		    }

	
		    process(".", in, outXML);
		   		 
		    printf("DEBUG: token after process . is %s\n", token);

		    // subroutine name
		    char subroutineName[128];
		    strcpy(subroutineName, token);
		    fprintf(outXML, "<identifier> %s </identifier>\n", token);
		    if(hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		        printf("DEBUG: token before process ( is %s\n", token);
		    }
		   
		    process("(", in, outXML);

		    int nArgs = compileExpressionList(in, outXML, outVM, subroutineTable, classTable, className);
		    process(")", in, outXML);

		    // --- VM call generation ---
		    Kind k = kindOf(subroutineTable, identifier);
		    int idx = indexOf(subroutineTable, identifier);
		    char *type = typeOf(subroutineTable, identifier);

		    if(k == KIND_NONE){
		        k = kindOf(classTable, identifier);
		        idx = indexOf(classTable, identifier);
		        type = typeOf(classTable, identifier);
		        printf("DEBUG: identifier='%s' kind=%d idx=%d type='%s'\n",
       identifier, k, idx, typeOf(subroutineTable, identifier));
		    }

		    char fullName[512];

		    if(k != KIND_NONE){
		        // object method → push object reference first

		        fprintf(outVM, "push %s %d\n",
		            (k == KIND_VAR ? "local" :
		             k == KIND_ARG ? "argument" :
		             k == KIND_STATIC ? "static" :
		             k == KIND_FIELD ? "this" : "pointer"),
		            idx
		        );
		       

		        nArgs += 1; // account for 'this'
		         snprintf(fullName, sizeof(fullName), "%s.%s", type, subroutineName);
		    } else {
		        // Class method or constructor
		        snprintf(fullName, sizeof(fullName), "%s.%s", identifier, subroutineName);
		    }
		    printf("before writeCall within tokenl1 is . \n tokenl1 %s; token ; %s", tokenL1, token);
		    writeCall(outVM, fullName, nArgs);
		    fprintf(outXML, "</term>\n");
		   
		    return;

		}else {

			Kind k = kindOf(subroutineTable, identifier);
		    int idx = indexOf(subroutineTable, identifier);

		    if (k == KIND_NONE) {
		        k = kindOf(classTable, identifier);
		        idx = indexOf(classTable, identifier);
		    }


			fprintf(outXML, "<identifier name=\"%s\" category=\"%s\" index=\"%d\" usage=\"used\"/>\n",
            identifier,
            (kindOf(subroutineTable, identifier) == KIND_STATIC ? "static" :
            kindOf(subroutineTable, identifier) == KIND_FIELD  ? "field"  :
            kindOf(subroutineTable, identifier) == KIND_ARG    ? "arg"    :
            kindOf(subroutineTable, identifier) == KIND_VAR    ? "var"    :
            kindOf(classTable, identifier) == KIND_STATIC ? "static" :
            kindOf(classTable, identifier) == KIND_FIELD  ? "field"  :
            kindOf(classTable, identifier) == KIND_ARG    ? "arg"    :
            kindOf(classTable, identifier) == KIND_VAR    ? "var"    : "none"),
            (kindOf(subroutineTable, identifier) != KIND_NONE ? indexOf(subroutineTable, identifier) :
            kindOf(classTable, identifier) != KIND_NONE ? indexOf(classTable, identifier) : -1)
            );

             // *** FIX: push the variable’s value onto the stack ***
		    if (k != KIND_NONE) {
		        fprintf(outVM, "push %s %d\n",
		            (k == KIND_VAR ? "local" :
		             k == KIND_ARG ? "argument" :
		             k == KIND_STATIC ? "static" :
		             k == KIND_FIELD ? "this" : "pointer"),
		            idx
		        );
		    }

			strcpy(token, tokenL1);  // restore token for parent context
			stringFlag = flagL1;
		}
	}else if(strcmp(token,"~")==0 || strcmp(token,"-")==0){
		fprintf(outXML, "<symbol> %s </symbol>\n",token);
		
		// store which unary op we saw
    	char unaryOp[3];
    	strcpy(unaryOp, token);

		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    		printf("token read: %s\n",token );
		}
		// compile the term that comes after the unary op
		compileTerm(in,outXML,outVM, subroutineTable, classTable, className);

		 // now emit the VM command for the unary op
	    if (strcmp(unaryOp, "-") == 0) {
   	     	fprintf(outVM, "neg\n");
   		} else if (strcmp(unaryOp, "~") == 0) {
        	fprintf(outVM, "not\n");
    	}
		fprintf(outXML, "</term>\n");
		return;
	}
	else if(strcmp(token,"(")==0){
		process("(",in,outXML);
		compileExpression(in, outXML,outVM, subroutineTable, classTable,className);
		process(")", in, outXML);
		fprintf(outXML, "</term>\n");
		return;
	}else {
    	fprintf(stderr, "Syntax error in term: unexpected token '%s'\n", token);
	}
	if (hasMoreTokens(in)) {
		advance(in, token, &stringFlag);
	}
	fprintf(outXML, "</term>\n");
	printf("</term>\n");


}
//expression: term(op term)*
void compileExpression(FILE *in, FILE *outXML,FILE *outVM,SymbolTable *subroutineTable, SymbolTable *classTable,
	const char *className){
	fprintf(outXML, "<expression>\n");
	printf("<expression>\n");

	// First term (pushes left operand)
	compileTerm(in,outXML,outVM, subroutineTable, classTable,className);
	
	// Process (op term)* sequenc
	while(strcmp(token,"+")==0 || strcmp(token,"-")==0 || strcmp(token,"*")==0 ||
		  strcmp(token,"/")==0 || strcmp(token,"&")==0 || strcmp(token,"|")==0 ||
		  strcmp(token,"<")==0 || strcmp(token,">")==0 || strcmp(token,"=")==0) {
		
		// Save operator
        char op[5];
        strncpy(op, token, sizeof(op)-1);

        // XML printing (with escapes)
		if(strcmp(token,"<")==0){
			fprintf(outXML, "<symbol> &lt; </symbol>\n");
		}else if(strcmp(token,">")==0){
			fprintf(outXML, "<symbol> &gt; </symbol>\n");
		}else if(strcmp(token,"&")==0){
			fprintf(outXML, "<symbol> &amp; </symbol>\n");
		}else{
			fprintf(outXML, "<symbol> %s </symbol>\n", token);  
		}

		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			//fprintf(out,"token read after symbol: %s\n",token );
			}
		compileTerm(in, outXML,outVM,subroutineTable, classTable,className);

		// --- Emit VM code for the operator ---
        if(strcmp(op,"+")==0)        fprintf(outVM, "add\n");
        else if(strcmp(op,"-")==0)   fprintf(outVM, "sub\n");
        else if(strcmp(op,"*")==0)   fprintf(outVM, "call Math.multiply 2\n");
        else if(strcmp(op,"/")==0)   fprintf(outVM, "call Math.divide 2\n");
        else if(strcmp(op,"&")==0)   fprintf(outVM, "and\n");
        else if(strcmp(op,"|")==0)   fprintf(outVM, "or\n");
        else if(strcmp(op,"<")==0)   fprintf(outVM, "lt\n");
        else if(strcmp(op,">")==0)   fprintf(outVM, "gt\n");
        else if(strcmp(op,"=")==0)   fprintf(outVM, "eq\n");
	}

	fprintf(outXML, "</expression>\n");
	printf("</expression>\n");
}



/*  letStatement: 'let' varName ('[' expression ']')? '=' expression ';'  */
void compileLet(FILE *in,FILE *outXML,FILE *outVM, SymbolTable *subroutineTable, SymbolTable *classTable,
	const char *className){

	fprintf(outXML, "<letStatement>\n");
	printf("<letStatement>\n");

	//let keyword
	process("let",in,outXML);

	//varName
	int x = tokenType(token, stringFlag);
	Kind k = KIND_NONE;
	int idx = -1;

	if(x == IDENTIFIER){
		k = kindOf(subroutineTable, token);
    	idx = indexOf(subroutineTable, token);

    	// If not in subroutine table, check class table
    	if(k == KIND_NONE){
        k = kindOf(classTable, token);
        idx = indexOf(classTable, token);
    	}

		fprintf(outXML,
        "<identifier name=\"%s\" category=\"%s\" index=\"%d\" usage=\"used\"/>\n",
        token,
        (k == KIND_STATIC ? "static" :
         k == KIND_FIELD  ? "field"  :
         k == KIND_ARG    ? "arg"    :
         k == KIND_VAR    ? "var"    : "none"),
        idx
    	);
	}else{
		printf("ERROR\n");
	}

	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}

	// Handle array assignment (varName[expression] = ...)
	if(strcmp(token,"[")==0){
		process("[",in,outXML);
		compileExpression(in, outXML,outVM, subroutineTable, classTable,className);
		process("]",in,outXML);
	}

			
	//"=" expression
	if(strcmp(token,"=")==0){
		process("=", in, outXML);
	
		compileExpression(in,outXML,outVM,subroutineTable, classTable,className);
		
		// Emit VM pop into variable (only for simple let right now)
		if (k == KIND_VAR) {
			fprintf(outVM, "pop local %d\n", idx);
		} else if (k == KIND_ARG) {
			fprintf(outVM, "pop argument %d\n", idx);
		} else if (k == KIND_STATIC) {
			fprintf(outVM, "pop static %d\n", idx);
		} else if (k == KIND_FIELD) {
			fprintf(outVM, "pop this %d\n", idx);
		}

		//;
		if(strcmp(token,";")==0){
		process(";",in,outXML);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//	printf("After advance: token = '%s'\n", token);
		//}
		//fprintf(out,"process ; complete. token now =%s\n",token );
		}
	}
	else if(strcmp(token,";")==0){
		process(";", in, outXML);
	}

	fprintf(outXML, "</letStatement>\n");
	printf("</letStatement>\n");
}

void compileIf(FILE *in,FILE *outXML,FILE *outVM, SymbolTable *subroutineTable, SymbolTable *classTable,
	const char *className,const char *returnType){
	
	static int ifCounter = 0;  // persists across calls
    int myIf = ifCounter++;    // unique index for this if statement

	fprintf(outXML, "<ifStatement>\n");
	printf("<ifStatement>\n");

	process("if",in,outXML);
	process("(",in,outXML);

	compileExpression(in,outXML,outVM,subroutineTable, classTable,className);
	
	process(")",in,outXML);
	process("{",in,outXML);
	
	// label for IF_TRUE and IF_FALSE
    fprintf(outVM, "if-goto IF_TRUE%d\n", myIf);
    fprintf(outVM, "goto IF_FALSE%d\n", myIf);
    fprintf(outVM, "label IF_TRUE%d\n", myIf);

	compileStatements(in,outXML,outVM,subroutineTable, classTable, className, returnType);
	process("}",in,outXML);

	if(strcmp(token,"else")==0){
		process("else",in,outXML);
		process("{",in,outXML);

		fprintf(outVM, "goto IF_END%d\n", myIf);
        fprintf(outVM, "label IF_FALSE%d\n", myIf);
		
		compileStatements(in,outXML, outVM, subroutineTable, classTable, className,returnType);

		process("}",in,outXML);
		fprintf(outVM, "label IF_END%d\n", myIf);
	}else {
        fprintf(outVM, "label IF_FALSE%d\n", myIf);
    }
    
	fprintf(outXML, "</ifStatement>\n");
	printf("</ifstatement>\n");
}

void compileWhile(FILE * in,FILE *outXML,FILE *outVM, SymbolTable *subroutineTable, SymbolTable *classTable,
	const char *className,const char *returnType){
	fprintf(outXML, "<whileStatement>\n");
	printf("<whileStatement>\n");

	static int whileCounter = 0;
    int id = whileCounter++;

    // 'while'
	process("while",in,outXML);
	
    // --- VM: label for start of while condition ---
    fprintf(outVM, "label WHILE_EXP%d\n", id);


    // '(' expression ')'
	process("(",in,outXML);
	compileExpression(in,outXML, outVM,subroutineTable, classTable,className);
	process(")",in,outXML);

	// --- VM: condition check ---
    // Jack expects: while (expr) { ... }
    // VM needs: if expr == false -> exit
    fprintf(outVM, "not\n");
    fprintf(outVM, "if-goto WHILE_END%d\n", id);

    // '{' statements '}'
	process("{",in,outXML);
	compileStatements(in,outXML,outVM, subroutineTable, classTable,className, returnType);
	process("}",in,outXML);

	// --- VM: jump back to start ---
    fprintf(outVM, "goto WHILE_EXP%d\n", id);
    fprintf(outVM, "label WHILE_END%d\n", id);

	fprintf(outXML, "</whileStatement>\n");
	printf("</whileStatement>\n");
}

/*void compileDo(FILE *in,FILE *outXML,FILE *outVM, SymbolTable *subroutineTable, SymbolTable *classTable,
	const char *className){
	fprintf(outXML, "<doStatement>\n");
	printf("<doStatement>\n");
	process("do",in,outXML);

    // first identifier (could be className, varName, or subroutineName)
    char firstIdentifier[128];
    strcpy(firstIdentifier, token); // could be className, varName, or subroutineName
	fprintf(outXML, "<identifier> %s </identifier>\n",token);
	
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}

	// --- Handle subroutine call ---
    char subroutineName[128];
    char fullName[128];
    int nArgs = 0;

	if(strcmp(token,"(")==0){
		// case: do foo(...)
		strcpy(subroutineName, firstIdentifier);

		process("(", in,outXML);
		nArgs = compileExpressionList(in, outXML,outVM,subroutineTable, classTable,className);
		process(")", in, outXML);


        // VM call: currentClass.subroutineName
        sprintf(fullName, "%s.%s",className, subroutineName); 
        writeCall(outVM, fullName, nArgs); //check this

	}else if(strcmp(token,".")==0){

		// case: do ClassName.method(...) or varName.method(...)

		process(".", in, outXML);
		char methodName[128];
        strcpy(methodName, token);
		fprintf(outXML, "<identifier> %s </identifier>\n",token);

		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
   			printf("token read: %s\n",token );
		}
		process("(", in,outXML);
		// Handle argument list
		nArgs = compileExpressionList(in,outXML,outVM,subroutineTable, classTable,className);
		process(")", in,outXML);


   		 // --- Build fullName ---
   		char fullName[256];
   		sprintf(fullName, "%s.%s", classOrVar, methodName);

    	// --- Write VM call ---
    	writeCall(outVM, fullName, nArgs);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			//fprintf(out,"token read after symbol: %s\n",token );
		//	}
	}
	process(";",in,outXML);

    // do statements always discard return value
    writePop(outVM, "temp", 0);
	
	fprintf(outXML, "</doStatement>\n");
	printf("</doStatement>\n");
}*/

void compileDo(FILE *in, FILE *outXML, FILE *outVM,
               SymbolTable *subroutineTable, SymbolTable *classTable,
               const char *className) {

    fprintf(outXML, "<doStatement>\n");
    printf("<doStatement>\n");

    process("do", in, outXML);

    char firstIdentifier[128];
    strcpy(firstIdentifier, token); // could be className, varName, or subroutineName
    fprintf(outXML, "<identifier> %s </identifier>\n", token);

    if(hasMoreTokens(in) && advance(in, token, &stringFlag)){
        printf("token read: %s\n", token);
    }

    char subroutineName[128];
    int nArgs = 0;
    char fullName[256];

    if(strcmp(token,"(")==0){
        // simple subroutine call in current class
        strcpy(subroutineName, firstIdentifier);
        process("(", in, outXML);
        nArgs = compileExpressionList(in, outXML, outVM, subroutineTable, classTable, className);
        process(")", in, outXML);
        sprintf(fullName, "%s.%s", className, subroutineName);
    }
    else if(strcmp(token,".")==0){
        process(".", in, outXML);

        strcpy(subroutineName, token);
        fprintf(outXML, "<identifier> %s </identifier>\n", token);

        if(hasMoreTokens(in) && advance(in, token, &stringFlag)){
            printf("token read: %s\n", token);
        }
        process("(", in, outXML);
        nArgs = compileExpressionList(in, outXML, outVM, subroutineTable, classTable, className);
        process(")", in, outXML);

        // --- check if firstIdentifier is a variable ---
        Kind k = kindOf(subroutineTable, firstIdentifier);
        int idx = indexOf(subroutineTable, firstIdentifier);
        if(k == KIND_NONE){
            k = kindOf(classTable, firstIdentifier);
            idx = indexOf(classTable, firstIdentifier);
        }

        if(k != KIND_NONE){
            // push object reference
            fprintf(outVM, "push %s %d\n",
                (k == KIND_VAR ? "local" :
                 k == KIND_ARG ? "argument" :
                 k == KIND_FIELD ? "this" :
                 k == KIND_STATIC ? "static" : "pointer"),
                idx
            );
            nArgs += 1; // account for 'this'

            // build fullName using variable type
            sprintf(fullName, "%s.%s",
                typeOf(subroutineTable, firstIdentifier) != NULL ? typeOf(subroutineTable, firstIdentifier) :
                typeOf(classTable, firstIdentifier),
                subroutineName
            );
        } else {
            // class method / constructor
            sprintf(fullName, "%s.%s", firstIdentifier, subroutineName);
        }
    }
    printf("\n\nDEBUG: compileDo issuing call to %s with %d args\n\n", fullName, nArgs);
    writeCall(outVM, fullName, nArgs);
    process(";", in, outXML);

    // discard return value
    writePop(outVM, "temp", 0);

    fprintf(outXML, "</doStatement>\n");
    printf("</doStatement>\n");
}

void compileReturn(FILE * in, FILE *outXML,FILE *outVM, SymbolTable *subroutineTable, SymbolTable *classTable,
	const char *className, const char *returnType){
	fprintf(outXML, "<returnStatement>\n");
	printf("<returnStatement>\n");
	process("return", in,outXML);

	/*
	int x = tokenType(token, stringFlag);

	if(x == INT_CONST||x == STRING_CONST|| x == IDENTIFIER ||
	strcmp(token, "true")==0||strcmp(token,"false")==0||
	strcmp(token,"null")==0||strcmp(token,"this")==0){
		
		compileExpression(in,outXML,outVM, subroutineTable, classTable,className);
	}

	if(strcmp(token,";")==0){
		// return;  → push constant 0
        writePush(outVM, "constant", 0);
		process(";",in,outXML);
	}*/

	if (strcmp(returnType, "void") == 0) {
        // Expect only ";", push dummy 0
        process(";", in, outXML);
        writePush(outVM, "constant", 0);
    } else {
        // Must compile expression (something to return)
        compileExpression(in, outXML, outVM, subroutineTable, classTable, className);
        process(";", in, outXML);
    }

	   
	// Every return must end with VM return
    writeReturn(outVM);

	fprintf(outXML, "</returnStatement>\n");
	printf("</returnStatement>\n");
}

void compileStatements(FILE *in,FILE *outXML,FILE *outVM,SymbolTable *subroutineTable, SymbolTable *classTable,
	const char *className, const char *returnType){
	printf("<statements>\n");
	fprintf(outXML, "<statements>\n");
	

	//while (strcmp(token, "let") == 0 || strcmp(token, "if") == 0 ||
   // strcmp(token, "while") == 0 || strcmp(token, "do") == 0 ||
    //strcmp(token, "return") == 0) {
		
		while(1){
			if(strcmp(token, "let")==0){
				compileLet(in,outXML,outVM,subroutineTable,classTable,className);
			}else if(strcmp(token, "if")==0){
				compileIf(in, outXML,outVM,subroutineTable,classTable,className,returnType);
			}else if(strcmp(token, "while")==0){
				compileWhile(in, outXML,outVM,subroutineTable,classTable,className,returnType);
			}else if(strcmp(token, "do")==0){
				compileDo(in, outXML,outVM,subroutineTable,classTable,className);
			}else if(strcmp(token, "return")==0){
				compileReturn(in, outXML,outVM,subroutineTable,classTable,className,returnType);
			}else{
				break;
			}
		}
	//}

	fprintf(outXML, "</statements>\n");
	printf("</statements>\n");
}

void compileParamaterList(FILE *in, FILE *outXML,SymbolTable *subroutineTable){
	fprintf(outXML, "<parameterList>\n");
	printf("<parameterList>\n");
	int x = tokenType(token, stringFlag);
	char type[MAX_NAME_LEN];  // store type for symbol table

	//type
	if(strcmp(token, "int")==0 || strcmp(token, "char")==0
	|| strcmp(token, "boolean")==0){
		strcpy(type, token);  // save type for symbol table
		fprintf(outXML, "<keyword> %s </keyword>\n", token);
	}else if(x == IDENTIFIER){
		strcpy(type, token);
		fprintf(outXML, "<identifier> %s </identifier>\n", token);
	}else if(strcmp(token,")")==0){
		// empty paramater list
		fprintf(outXML, "</parameterList>\n");
		return;
	}	
	//advance to varName
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	//first variable name
	x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		define(subroutineTable, token, type, KIND_ARG);
		 // Enhanced XML output
        fprintf(outXML,
        "<identifier name=\"%s\" category=\"arg\" index=\"%d\" usage=\"declared\"/>\n",
        token,
        indexOf(subroutineTable, token)
        );
	}
	// advance to next token (',' or ')')
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}
	// --- Handle subsequent parameters ---
	while(1){
		if(strcmp(token,",")==0){
			process(",",in,outXML);

			x = tokenType(token, stringFlag);
			if(strcmp(token, "int")==0 || strcmp(token, "char")==0
			|| strcmp(token, "boolean")==0){
				strcpy(type, token);  // save type
				fprintf(outXML, "<keyword> %s </keyword>\n", token);
			}else if(x == IDENTIFIER){
				strcpy(type, token);
				fprintf(outXML, "<identifier> %s </identifier>\n", token);
			}
			//advance to varName
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			x = tokenType(token, stringFlag);
			if(x == IDENTIFIER){
                define(subroutineTable, token, type, KIND_ARG);

				fprintf(outXML,
                "<identifier name=\"%s\" category=\"arg\" index=\"%d\" usage=\"declared\"/>\n",
                token,
                indexOf(subroutineTable, token)
                );
			}
			// advance to next token
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
		}else if(strcmp(token,")")==0){
			fprintf(outXML, "</parameterList>\n");
			return;
		}
	}

	fprintf(outXML, "</parameterList>\n");
	printf("</parameterList>\n");
}
void compileVarDec(FILE *in, FILE *outXML, SymbolTable *subroutineTable){

	fprintf(outXML, "<varDec>\n");
	printf("<varDec>\n");
	//var
	process("var", in, outXML);
	
	//type
	int x = tokenType(token, stringFlag);

	// type: 'int' | 'char' | 'boolean' | className
    char type[MAX_NAME_LEN];
    strcpy(type, token); // store type for symbol table

	if(x == IDENTIFIER){
		fprintf(outXML, "<identifier> %s </identifier>\n", token);
		

	}else if(strcmp(token, "int")==0||strcmp(token,"char")==0||
		strcmp(token, "boolean")==0){
		fprintf(outXML, "<keyword> %s </keyword>\n", token);
	}
	//varName
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}
	x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		define(subroutineTable, token, type, KIND_VAR);
		int idx = indexOf(subroutineTable, token);
        fprintf(outXML,
        "<identifier name=\"%s\" category=\"var\" index=\"%d\" usage=\"declared\"/>\n",
        token, idx
        );
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    		printf("token read: %s\n",token );
		}
	}
	
	while(1){
		if(strcmp(token,",")==0){
			process(",", in, outXML);

			x = tokenType(token, stringFlag);
			if(x == IDENTIFIER){
				// Add variable to subroutine symbol table
            	define(subroutineTable, token, type, KIND_VAR);
				int idx = indexOf(subroutineTable, token);
				// Enhanced XML output
            	fprintf(outXML,
                "<identifier name=\"%s\" category=\"var\" index=\"%d\" usage=\"declared\"/>\n",
                token,
                idx
            	);
			}
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
			    printf("token read: %s\n",token );
			}
		}else if(strcmp(token,";")==0){
			process(";", in, outXML);
			break;
		}
	}
	fprintf(outXML, "</varDec>\n");
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


void compileClassVarDec(FILE *in, FILE *outXML, char *token, SymbolTable *classTable){
	fprintf(outXML, "<classVarDec>\n"); //will remove for compiler to generate only VM code
	printf("<classVarDec>\n");		 //will remove for compiler to generate only VM code
	
	//('static'|'field')
	int x = tokenType(token, stringFlag);
	Kind kind;
	if(strcmp(token, "static")==0){
		kind = KIND_STATIC;
		process("static", in, outXML);
		//int classSymbolTable#Static;
	}else if(strcmp(token, "field")==0){
		kind = KIND_FIELD;
		process("field", in, outXML);
		//int classSymbolTable#Field;
	}
	
	//type:'int'|'char'|'boolean'
	char type[MAX_NAME_LEN];
	strcpy(type,token);

	x = tokenType(token, stringFlag);
	//break up keyword more specifically to  find the specific type
	if (x == KEYWORD) {
		fprintf(outXML, "<keyword> %s </keyword>\n", token);
	}//type:className(className:identifier)
	else if(x ==  IDENTIFIER){
		fprintf(outXML, "<identifier> %s </identifier>\n", token);
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		printf("token read: %s\n",token );//will remove for compiler to generate only VM code
	}
	
	//varName (';'varName)*
	while(1){
		x = tokenType(token, stringFlag);
		if(x == IDENTIFIER){
			// Define in symbol table
            define(classTable, token, type, kind);

			// Lookup the kind from the table (instead of relying on local 'kind')
			Kind k = kindOf(classTable, token);
			int idx = indexOf(classTable, token);

			fprintf(outXML,
    		"<identifier name=\"%s\" kind=\"%s\" index=\"%d\" usage=\"declared\"/>\n",
  			token,
    		(k == KIND_STATIC ? "static" :
    		k == KIND_FIELD  ? "field"  :
		    k == KIND_ARG    ? "arg"    :
		    k == KIND_VAR    ? "var"    : "none"),
		    idx
			);
		}
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
			printf("token read: %s\n",token );
		}
		if(token[0] == ';'){
			process(";",in, outXML);
			break;
		}
		else if(token[0] == ','){
			process(",",in, outXML);
			continue;
		}
	}	
	fprintf(outXML, "</classVarDec>\n");
	printf("</classVarDec>\n");
}

//subroutineBody: '{' varDec* statements '}'
void compileSubroutineBody(FILE *in, FILE *outXML, FILE *outVM,SymbolTable *subroutineTable, 
SymbolTable *classTable,const char *className,const char *subroutineKind, const char *subroutineName,
const char *returnType){
	fprintf(outXML, "<subroutineBody>\n");
	printf("<subroutineBody>\n");
	process("{", in, outXML);

    FILE *inCopy = in;  // assume advance/hasMoreTokens moves the global token
    char tokenBackup[MAX_NAME_LEN];
    strcpy(tokenBackup, token); // save current token

    // simple scan: count 'var' statements
    while (strcmp(token, "}") != 0) {
        if (strcmp(token, "var") == 0) {
            //nLocals++;
            compileVarDec(in, outXML, subroutineTable);  // this advances token
        } else {
            break;
        }
    }

    // --- Count locals using the symbol table (KIND_VAR only) ---
    Kind kind = KIND_VAR;
    int nLocals = varCount(subroutineTable,kind);

    // --- Emit function command ---
    writeFunction(outVM, className, subroutineName, nLocals);

    // --- Initialize locals to 0 (VM spec §8.5) ---
    for (int i = 0; i < nLocals; i++) {           
        fprintf(outVM, "push constant 0\n");      
        fprintf(outVM, "pop local %d\n", i);      
    }          

    // --- Compile remaining statements ---
    while (strcmp(token, "}") != 0) {
        compileStatements(in, outXML, outVM, subroutineTable, classTable, className, returnType);
    }

	process("}",in, outXML);
	fprintf(outXML, "</subroutineBody>\n");
	printf("</subroutineBody>\n");
}


void compileSubroutineDec(FILE *in, FILE *outXML, FILE *outVM, SymbolTable *classTable, const char *className,
	 char *returnType){
	SymbolTable subroutineTable;
    construct_Symbol_Table(&subroutineTable);  // reset for this subroutine

    char subroutineKind[32];   // constructor, function, method
    
    char subroutineName[128];  // function name (e.g. "main")

	fprintf(outXML, "<subroutineDec>\n");
	printf("<subroutineDec>\n");

	// First token: constructor | function | method
    strcpy(subroutineKind, token);
	fprintf(outXML, "<keyword> %s </keyword>\n", token);

	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}

	// Return type (void | int | char | boolean | className)
	int x = tokenType(token, stringFlag);

	if(strcmp(token, "void")==0 || strcmp(token, "int")==0 ||
		strcmp(token, "char")==0 || strcmp(token, "boolean")==0){
		strcpy(returnType, token);
		fprintf(outXML, "<keyword> %s </keyword>\n", token);
	}else if(x == IDENTIFIER){
		strcpy(returnType, token); 
		fprintf(outXML, "<identifier> %s </identifier>\n", token);
	}

	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	// Subroutine name
	x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		strcpy(subroutineName, token);
		fprintf(outXML, "<identifier> %s </identifier>\n", token);
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}

	// Parameter list
	process( "(" , in, outXML);
	compileParamaterList(in, outXML,&subroutineTable);
	process( ")" , in, outXML);
	
	 // Subroutine body
    compileSubroutineBody(in, outXML, outVM, &subroutineTable, classTable, className, subroutineKind, subroutineName,
    	returnType);
		
	fprintf(outXML, "</subroutineDec>\n");
	printf("</subroutineDec>\n");
}
//<identifier> name:%s, type:%s, kind:%s, #:%d, category:%s, usage:%s </identifier>
//<identifier> name:%s, type:%s(class), kind(none):%s, #:%d(none), category:%s, usage:%s </identifier>

/*according to Compiling Classes(project 11, when starting to compile a class, compiler creates a class-level
symbol table and adds to it the field and static variables declared in the class declaration. Compiler also
creates an empty subroutine-level symbol table. No code is generated*/

void compileClass(FILE *in, FILE *outXML, FILE *outVM){
	fprintf(outXML, "<class>\n");
	printf("<class>\n");

	SymbolTable classTable;            // local class-level symbol table
    construct_Symbol_Table(&classTable);  // initialize/reset table

    char subroutineReturnType[64];       // return type

	while(hasMoreTokens(in)) {
    	advance(in, token, &stringFlag); // prime the first token
    	if (strlen(token) > 0) break;  	
	}
	
	process("class", in, outXML);

	 // ---- Get class name ----
    char className[MAX_NAME_LEN]; // store class name
	
	int x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(outXML, "<identifier> %s </identifier>\n", token);
		printf("token read: %s\n",token );
		strncpy(className, token, sizeof(className)-1);
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	process("{", in, outXML);

	while(1){
		if(strcmp(token, "static")==0 || strcmp(token, "field")==0){
			compileClassVarDec(in,outXML,token,&classTable);
		}else{
			break;
		}		
	}
	while(1){
		//printf("Calling subroutineDec\n");
		if(strcmp(token, "constructor") == 0 || 
    	strcmp(token,"function") == 0 || 
    	strcmp(token,"method") == 0) {
    		compileSubroutineDec(in, outXML,outVM,&classTable, className, subroutineReturnType);
		}else{
			break;
		}
	}	
	process("}", in, outXML);
	fprintf(outXML, "</class>\n");
	printf("</class>\n");
}
