#ifndef PARSERUTILS_H
#define PARSERUTILS_H

bool parserTypeIsTheWanted(parser_T* parser, int tokenType);
void parserGetNextToken(parser_T* parser);

#endif
