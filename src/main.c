#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/fileRead.h"

void printHelp() {
	printf("Usage:\nskib.exe <file_name>\n");
	exit(1);
}

void codeExecutedWithNoErrors() {
	const char* GREEN = "\x1b[32m";
	const char* RESET = "\x1b[0m";
	printf("%sCode Executed With no Errors!%s\n", GREEN, RESET);
}

int main(int argc, char* argv[]) {
	if (argc < 2) { printHelp(); }

	lexer_T* lexer = initLexer(readFileLines(argv[1]));

	parser_T* parser = initParser(lexer);
	AST_T* source = parserParse(parser);
	visitor_T* visitor = initVisitor();
	visitorVisit(visitor, source);

	codeExecutedWithNoErrors();
	return 0;
}