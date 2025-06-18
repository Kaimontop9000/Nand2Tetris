#include <stdio.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
	FILE *in = fopen("input.txt","r");
	FILE *out = fopen("output.txt","w");

	int x = 1;
	int c;
	int index=0;
	char string[100];
	while(x){
		c=fgetc(in);
		if (c == EOF) {
 		   if (index > 0) {
        	string[index] = '\0';  // return last token
        	x = 0;
    		} 
    		else {
        	x = 0; // stop loop
        	string[0] = '\0'; // no more tokens
    		}
		}

		else if(isspace(c)){
			string[index] = '\0';
			x = 0;

		}
		else if(c == '{' || c == '}' || c == '(' || c == ')'|| c == '[' || c == ']' || c == '.' || c == ',' || c == ';' || c == '+' 
			|| c == '-' || c == '*' || c == '/' || c == '&' || c == '|' || c == '<'|| c == '>' || c == '=' || c == '~'){
			//already building a token
			if(index>0){
				string[index] = '\0';
				x = 0;
			}
			//token is a symbol
			else{
				string[index++]=c;
				string[index] = '\0';
				x = 0;
			}
		}	//carry on building token
		else if (c == '"') {
    		if (index == 0) {
        	// start reading a string constant token
        	index = 0;
       		while ((c = fgetc(in)) != EOF && c != '"') {
            	if (index < sizeof(string) - 1) {
                string[index++] = c;
            	}
        	}
        	string[index] = '\0';
        	x = 0;  // done with this token
    		} else {
        		// already building a token, so this " is a delimiter
        		ungetc(c, in);  // put back the quote for next token read
        		string[index] = '\0';
       			x = 0;  // end current token here
    			}
			}			
		else{
		string[index++]=c;
		}
		
	}
	return 0;
}