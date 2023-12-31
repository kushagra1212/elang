#ifndef PARSER_H
#define PARSER_H
#include "../diagnostics/DiagnosticHandler/DiagnosticHandler.h"
#include "../lexer/Lexer.h"
#include "../syntax/CompilationUnitSyntax.h"
#include "../syntax/MemberSyntax.h"
#include "../syntax/expression/AssignmentExpressionSyntax.h"
#include "../syntax/expression/BinaryExpressionSyntax.h"
#include "../syntax/expression/BracketedExpressionSyntax/BracketedExpressionSyntax.h"
#include "../syntax/expression/CallExpressionSyntax/CallExpressionSyntax.h"
#include "../syntax/expression/ContainerExpressionSyntax/ContainerExpressionSyntax.h"
#include "../syntax/expression/FillExpressionSyntax/FillExpressionSyntax.h"
#include "../syntax/expression/IndexExpressionSyntax/IndexExpressionSyntax.h"
#include "../syntax/expression/LiteralExpressionSyntax.h"
#include "../syntax/expression/ParenthesizedExpressionSyntax.h"
#include "../syntax/expression/TypeExpressionSyntax/ArrayTypeExpressionSyntax/ArrayTypeExpressionSyntax.h"
#include "../syntax/expression/TypeExpressionSyntax/TypeExpressionSyntax.h"
#include "../syntax/expression/UnaryExpressionSyntax.h"
#include "../syntax/expression/VariableExpressionSyntax/ArrayVariableExpressionSyntax/ArrayVariableExpressionSyntax.h"
#include "../syntax/expression/VariableExpressionSyntax/VariableExpressionSyntax.h"
#include "../syntax/statements/BlockStatementSyntax/BlockStatementSyntax.h"
#include "../syntax/statements/BreakStatementSyntax/BreakStatementSyntax.h"
#include "../syntax/statements/BringStatementSyntax/BringStatementSyntax.h"
#include "../syntax/statements/ContainerStatementSyntax/ContainerStatementSyntax.h"
#include "../syntax/statements/ContinueStatementSyntax/ContinueStatementSyntax.h"
#include "../syntax/statements/EmptyStatementSyntax/EmptyStatementSyntax.h"
#include "../syntax/statements/ExpressionStatementSyntax/ExpressionStatementSyntax.h"
#include "../syntax/statements/ForStatementSyntax/ForStatementSyntax.h"
#include "../syntax/statements/FunctionDeclarationSyntax/FunctionDeclarationSyntax.h"
#include "../syntax/statements/GlobalStatementSyntax/GlobalStatementSyntax.h"
#include "../syntax/statements/IfStatementSyntax/IfStatementSyntax.h"
#include "../syntax/statements/OrIfStatementSyntax/OrIfStatementSyntax.h"
#include "../syntax/statements/ParameterSyntax/ParameterSyntax.h"
#include "../syntax/statements/ReturnStatementSyntax/ReturnStatementSyntax.h"
#include "../syntax/statements/StatementSyntax.h"
#include "../syntax/statements/VariableDeclarationSyntax/VariableDeclarationSyntax.h"
#include "../syntax/statements/WhileStatementSyntax/WhileStatementSyntax.h"
#include "../utils/Utils.h"

#include <typeindex>
class Parser {

public:
  std::vector<std::unique_ptr<SyntaxToken<std::any>>> tokens;

  std::unique_ptr<Lexer> lexer;

  Parser(std::vector<std::string> souceCode,
         DiagnosticHandler *diagnosticHandler);
  ~Parser();

  std::unique_ptr<CompilationUnitSyntax> parseCompilationUnit();

private:
  DiagnosticHandler *_diagnosticHandler;
  std::unique_ptr<CompilationUnitSyntax> compilationUnit;
  int position = 0;

  bool matchKind(SyntaxKindUtils::SyntaxKind kind);
  void parseMemberList(std::vector<std::unique_ptr<MemberSyntax>> members);

  std::unique_ptr<SyntaxToken<std::any>>
  match(SyntaxKindUtils::SyntaxKind kind);
  SyntaxToken<std::any> *peek(int offset);
  SyntaxToken<std::any> *getCurrent();
  std::unique_ptr<SyntaxToken<std::any>> nextToken();

  /*
    STATEMENTS
  */
  std::unique_ptr<StatementSyntax> parseStatement();
  std::unique_ptr<BlockStatementSyntax> parseBlockStatement();
  std::unique_ptr<BreakStatementSyntax> parseBreakStatement();
  std::unique_ptr<ReturnStatementSyntax> parseReturnStatement();
  std::unique_ptr<ContinueStatementSyntax> parseContinueStatement();
  std::unique_ptr<ExpressionStatementSyntax> parseExpressionStatement();
  std::unique_ptr<StatementSyntax> parseVariableDeclaration();
  std::unique_ptr<IfStatementSyntax> parseIfStatement();
  std::unique_ptr<ElseClauseSyntax> parseElseStatement();
  std::unique_ptr<WhileStatementSyntax> parseWhileStatement();
  std::unique_ptr<ForStatementSyntax> parseForStatement();
  std::unique_ptr<StatementSyntax> parseBringStatement();
  std::unique_ptr<ContainerStatementSyntax> parseContainerStatement();
  std::unique_ptr<GlobalStatementSyntax>
  parseGlobalStatement(const bool &isExposed);

  /*
    EXPRESSIONS
  */
  std::unique_ptr<ExpressionSyntax> parseIndexExpression();
  std::unique_ptr<ExpressionSyntax> parseNameorCallExpression();
  std::unique_ptr<FunctionDeclarationSyntax>
  parseFunctionDeclaration(const bool &isExposed);
  std::unique_ptr<FunctionDeclarationSyntax> handleOptionalType(
      std::unique_ptr<FunctionDeclarationSyntax> &functionDeclaration);
  std::unique_ptr<ExpressionSyntax> parseExpression(int parentPrecedence = 0);
  std::unique_ptr<ExpressionSyntax> parsePrimaryExpression();
  std::unique_ptr<ContainerExpressionSyntax> parseContainerExpression();
  std::unique_ptr<VariableExpressionSyntax> parseVariableExpression();
  std::unique_ptr<ExpressionSyntax> parseBracketedExpression();
  std::unique_ptr<FillExpressionSyntax> parseFillExpression();
  std::unique_ptr<TypeExpressionSyntax> parseTypeExpression();

  Utils::type parseType();

  std::unique_ptr<MemberSyntax> parseMember();
};
#endif