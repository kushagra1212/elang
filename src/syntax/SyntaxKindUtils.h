
#ifndef SYNTAXKINDUTILS_H
#define SYNTAXKINDUTILS_H
#include "../Common.h"
namespace SyntaxKindUtils {
enum SyntaxKind {

  // Tokens

  NumberToken,
  WhitespaceToken,
  PlusToken,
  MinusToken,
  StarToken,
  SlashToken,
  OpenParenthesisToken,
  CloseParenthesisToken,
  BadToken,
  EndOfFileToken,
  AmpersandAmpersandToken,
  PipePipeToken,
  EqualsEqualsToken,
  EqualsToken,
  BangToken,
  BangEqualsToken,
  LessOrEqualsToken,
  LessToken,
  GreaterOrEqualsToken,
  GreaterToken,
  SemiColonToken,
  ColonToken,
  CommaToken,
  EndOfLineToken,
  SlashHashToken,
  HashSlashToken,
  HashToken,
  NthgKeyword,
  Int32Keyword,
  DeciKeyword,
  StrKeyword,
  BoolKeyword,
  BringKeyword,
  SlashSlashToken,

  // Bitwise

  AmpersandToken,
  PipeToken,
  CaretToken,
  TildeToken,

  // percent
  PercentToken,

  // Braces

  OpenBraceToken,
  CloseBraceToken,
  OpenBracketToken,
  CloseBracketToken,
  // Strings

  StringToken,

  SemicolonToken,
  // Keywords
  TrueKeyword,
  FalseKeyword,
  VarKeyword,
  ConstKeyword,
  IfKeyword,
  ElseKeyword,
  WhileKeyword,
  ForKeyword,
  ToKeyword,
  FunctionKeyword,
  ContinueKeyword,
  BreakKeyword,
  ReturnKeyword,
  OrKeyword,
  ExposeKeyword,
  FromKeyword,
  ElseClause,
  FillKeyword,
  TypeKeyword,

  // Identifiers
  IdentifierToken,

  // Experssions

  LiteralExpression,
  UnaryExpression,
  BinaryExpression,
  ParenthesizedExpression,
  AssignmentExpression,
  CompilationUnit,
  CallExpression,
  IndexExpression,
  ContainerExpression,
  FillExpression,
  BracketedExpression,
  // Statements
  GlobalStatement,
  ExpressionStatement,
  CustomTypeStatement,
  BlockStatement,
  VariableDeclaration,
  IfStatement,
  OrIfStatement,
  WhileStatement,
  ForStatement,
  ReturnStatement,
  CommentStatement,
  FunctionDeclarationSyntax,
  ParameterSyntax,
  ArrayParameterSyntax,
  EmptyStatement,
  BringStatementSyntax,
  ContainerStatement,
  VariableExpressionSyntax,
  ArrayVariableExpressionSyntax,
  PrimitiveTypeExpression,
  ArrayTypeExpression,
  ObjectTypeExpression,
  ObjectExpression,

  // Types (Tokens) Which are not created by the User
  NBU_ARRAY_TYPE,
  NBU_OBJECT_TYPE,
  NBU_UNKNOWN_TYPE,
};
bool isInt32(const std::string &str);

bool isInt64(const std::string &str);

bool isDouble(const std::string &str);

std::string to_string(SyntaxKind kind);

bool isType(SyntaxKind kind);
}  // namespace SyntaxKindUtils

#endif  // SYNTAXKINDUTILS_H