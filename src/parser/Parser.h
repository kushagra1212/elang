#ifndef PARSER_H
#define PARSER_H
#include "../lexer/Lexer.h"
#include "../syntax/CompilationUnitSyntax.h"
#include "../syntax/expression/AssignmentExpressionSyntax.h"
#include "../syntax/expression/BinaryExpressionSyntax.h"
#include "../syntax/expression/LiteralExpressionSyntax.h"
#include "../syntax/expression/ParenthesizedExpressionSyntax.h"
#include "../syntax/expression/UnaryExpressionSyntax.h"
#include "../syntax/expression/VariableExpressionSyntax.h"

#include <typeindex>
class Parser {
private:
  std::vector<SyntaxToken<std::any> *> tokens;
  int position;

public:
  std::vector<std::string> logs;

public:
  Parser(std::vector<std::string> text);

private:
  SyntaxToken<std::any> *peek(int offset);

private:
  SyntaxToken<std::any> *getCurrent();

private:
  SyntaxToken<std::any> *nextToken();

private:
  SyntaxToken<std::any> *match(SyntaxKindUtils::SyntaxKind kind);

public:
  CompilationUnitSyntax *parseCompilationUnit();

private:
  ExpressionSyntax *parseExpression(int parentPrecedence = 0);

private:
  ExpressionSyntax *parsePrimaryExpression();
};
#endif