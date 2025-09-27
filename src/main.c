#include <stdio.h>
#include "include/lexer.h"

int main() {

	//lol moment
	//lol moment 2

	char* contents = "";
	lexer_T* lexer = initLexer(
		"var name = \"Skibidi Toilet\";\n"
		"print(name);\n"
	);

	token_T* token = (void*)0;

	while ((token = lexerGetNextToken(lexer)) != (void*)0) {
		printf("TOKEN(%d, %s)\n", token->type, token->value);
	}

	return 0;
}