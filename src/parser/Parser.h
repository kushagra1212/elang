#ifndef PARSER_H
#define PARSER_H
#include "../lexer/Lexer.h"
#include "../syntax/CompilationUnitSyntax.h"
#include "../syntax/expression/AssignmentExpressionSyntax.h"
#include "../syntax/expression/BinaryExpressionSyntax.h"
#include "../syntax/expression/CallExpressionSyntax/CallExpressionSyntax.h"
#include "../syntax/expression/LiteralExpressionSyntax.h"
#include "../syntax/expression/ParenthesizedExpressionSyntax.h"
#include "../syntax/expression/UnaryExpressionSyntax.h"
#include "../syntax/expression/VariableExpressionSyntax.h"
#include "../syntax/statements/BlockStatementSyntax/BlockStatementSyntax.h"
#include "../syntax/statements/ExpressionStatementSyntax/ExpressionStatementSyntax.h"
#include "../syntax/statements/ForStatementSyntax/ForStatementSyntax.h"
#include "../syntax/statements/IfStatementSyntax/IfStatementSyntax.h"
#include "../syntax/statements/StatementSyntax.h"
#include "../syntax/statements/VariableDeclarationSyntax/VariableDeclarationSyntax.h"
#include "../syntax/statements/WhileStatementSyntax/WhileStatementSyntax.h"
#include "../utils/Utils.h"
#include <typeindex>
class Parser {
private:
  std::vector<std::shared_ptr<SyntaxToken<std::any>>> tokens;
  int position;

public:
  std::vector<std::string> logs;

public:
  Parser(std::vector<std::string> text);

private:
  std::shared_ptr<SyntaxToken<std::any>> peek(int offset);

private:
  std::shared_ptr<SyntaxToken<std::any>> getCurrent();

private:
  std::shared_ptr<SyntaxToken<std::any>> nextToken();

private:
  std::shared_ptr<SyntaxToken<std::any>>
  match(SyntaxKindUtils::SyntaxKind kind);

public:
  std::shared_ptr<CompilationUnitSyntax> parseCompilationUnit();

private:
  StatementSyntax *parseStatement();
  BlockStatementSyntax *parseBlockStatement();

  ExpressionStatementSyntax *parseExpressionStatement();

  StatementSyntax *parseVariableDeclaration();

  IfStatementSyntax *parseIfStatement();

  WhileStatementSyntax *parseWhileStatement();

  ForStatementSyntax *parseForStatement();

  ExpressionSyntax *parseNameorCallExpression();

private:
  ExpressionSyntax *parseExpression(int parentPrecedence = 0);

private:
  ExpressionSyntax *parsePrimaryExpression();
};
#endif