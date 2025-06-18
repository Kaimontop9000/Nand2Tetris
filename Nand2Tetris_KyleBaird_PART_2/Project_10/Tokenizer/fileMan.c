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
		if(c == '{' || c == '}' || c == '(' || c == ')'|| c == '[' || c == ']' || c == '.' || c == ',' || c == ';' || c == '+' 
			|| c == '-' || c == '*' || c == '/' || c == '&' || c == '|' || c == '<'|| c == '>' || c == '=' || c == '~')
		{
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
			//carry on building token
		}else{
			string[index++]=c;
		}
		
	}
	return 0;
}