#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"

int main() {

	//lol moment
	//lol moment 2

	char* contents = "";
	lexer_T* lexer = initLexer(
		"var name = \"Michele\";\n"
		"var namesecond = \"Skibidino\";\n"
		"print(name, names3cond);\n"
	);

	parser_T* parser = initParser(lexer);
	AST_T* source = parserParse(parser);
	visitor_T* visitor = initVisitor();
	visitorVisit(visitor, source);

	return 0;
}