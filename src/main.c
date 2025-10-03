#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"

int main() {

	//lol moment
	//lol moment 2

	char* contents = "";
	lexer_T* lexer = initLexer(
		"var name = \"Skibidi Toilet\";\n"
		"print(name);\n"
	);

	parser_T* parser = initParser(lexer);
	AST_T* source = parserParse(parser);

	return 0;
}