
#include "SyntaxKindUtils.h"

bool SyntaxKindUtils::isInt32(const std::string &str) {
  try {
    size_t pos;
    int32_t number = std::stoi(str, &pos);
    return pos == str.size() && number >= std::numeric_limits<int32_t>::min() &&
           number <= std::numeric_limits<int32_t>::max();
  } catch (const std::exception &) {
    return false;
  }
}

bool SyntaxKindUtils::isInt64(const std::string &str) {
  try {
    size_t pos;
    int64_t number = std::stoll(str, &pos);
    return pos == str.size() && number >= std::numeric_limits<int64_t>::min() &&
           number <= std::numeric_limits<int64_t>::max();
  } catch (const std::exception &) {
    return false;
  }
}

bool SyntaxKindUtils::isDouble(const std::string &str) {
  try {
    size_t pos;
    double number = std::stod(str, &pos);
    return pos == str.size() && number >= std::numeric_limits<double>::min() &&
           number <= std::numeric_limits<double>::max();
  } catch (const std::exception &) {
    return false;
  }
}

std::string SyntaxKindUtils::to_string(SyntaxKind kind) {
  switch (kind) {
  case SyntaxKind::NumberToken:
    return "NumberToken";
  case SyntaxKind::TrueKeyword:
    return "TrueKeyword";
  case SyntaxKind::FalseKeyword:
    return "FalseKeyword";
  case SyntaxKind::WhitespaceToken:
    return "WhitespaceToken";
  case SyntaxKind::PlusToken:
    return "PlusToken";
  case SyntaxKind::MinusToken:
    return "MinusToken";
  case SyntaxKind::StarToken:
    return "StarToken";
  case SyntaxKind::SlashToken:
    return "SlashToken";
  case SyntaxKind::OpenParenthesisToken:
    return "OpenParenthesisToken";
  case SyntaxKind::CloseParenthesisToken:
    return "CloseParenthesisToken";
  case SyntaxKind::BadToken:
    return "BadToken";
  case SyntaxKind::EndOfFileToken:
    return "EndOfFileToken";
  case SyntaxKind::LiteralExpression:
    return "LiteralExpression";
  case SyntaxKind::UnaryExpression:
    return "UnaryExpression";
  case SyntaxKind::BinaryExpression:
    return "BinaryExpression";
  case SyntaxKind::ParenthesizedExpression:
    return "ParenthesizedExpression";
  case SyntaxKind::CompilationUnit:
    return "CompilationUnit";
  case SyntaxKind::AmpersandAmpersandToken:
    return "AmpersandAmpersandToken";
  case SyntaxKind::PipePipeToken:
    return "PipePipeToken";
  case SyntaxKind::EqualsEqualsToken:
    return "EqualsEqualsToken";
  case SyntaxKind::EqualsToken:
    return "EqualsToken";
  case SyntaxKind::BangToken:
    return "BangToken";
  case SyntaxKind::BangEqualsToken:
    return "BangEqualsToken";
  case SyntaxKind::LessOrEqualsToken:
    return "LessOrEqualsToken";
  case SyntaxKind::LessToken:
    return "LessToken";
  case SyntaxKind::GreaterOrEqualsToken:
    return "GreaterOrEqualsToken";
  case SyntaxKind::GreaterToken:
    return "GreaterToken";
  case SyntaxKind::SemiColonToken:
    return "SemiColonToken";
  case SyntaxKind::CommaToken:
    return "CommaToken";
  case SyntaxKind::OpenBraceToken:
    return "OpenBraceToken";
  case SyntaxKind::CloseBraceToken:
    return "CloseBraceToken";
  case SyntaxKind::StringToken:
    return "StringToken";
  case SyntaxKind::IdentifierToken:
    return "IdentifierToken";
  case SyntaxKind::SemicolonToken:
    return "SemicolonToken";
  case SyntaxKind::AmpersandToken:
    return "AmpersandToken";
  case SyntaxKind::PipeToken:
    return "PipeToken";
  case SyntaxKind::CaretToken:
    return "CaretToken";
  case SyntaxKind::TildeToken:
    return "TildeToken";
  case SyntaxKind::PercentToken:
    return "PercentToken";
  case SyntaxKind::VariableExpression:
    return "VariableExpression";
  case SyntaxKind::AssignmentExpression:
    return "AssignmentExpression";
  case SyntaxKind::EndOfLineToken:
    return "EndOfLineToken";
  case SyntaxKind::VarKeyword:
    return "VarKeyword";
  case SyntaxKind::ConstKeyword:
    return "ConstKeyword";
  case SyntaxKind::VariableDeclaration:
    return "VariableDeclaration";
  case SyntaxKind::IfKeyword:
    return "IfKeyword";
  case SyntaxKind::ElseKeyword:
    return "ElseKeyword";
  case SyntaxKind::ElseClause:
    return "ElseClause";
  case SyntaxKind::IfStatement:
    return "IfStatement";
  case SyntaxKind::WhileKeyword:
    return "WhileKeyword";
  case SyntaxKind::WhileStatement:
    return "WhileStatement";
  case SyntaxKind::ForKeyword:
    return "ForKeyword";
  case SyntaxKind::ToKeyword:
    return "ToKeyword";
  case SyntaxKind::ForStatement:
    return "ForStatement";
  case SyntaxKind::BlockStatement:
    return "BlockStatement";
  case SyntaxKind::ExpressionStatement:
    return "ExpressionStatement";
  case SyntaxKind::CallExpression:
    return "CallExpression";
  case SyntaxKind::GlobalStatement:
    return "GlobalStatement";
  case SyntaxKind::ParameterSyntax:
    return "ParameterSyntax";
  case SyntaxKind::FunctionDeclarationSyntax:
    return "FunctionDeclarationSyntax";
  case SyntaxKind::FunctionKeyword:
    return "FunctionKeyword";
  case SyntaxKind::ContinueKeyword:
    return "ContinueKeyword";
  case SyntaxKind::BreakKeyword:
    return "BreakKeyword";

  case SyntaxKind::ReturnKeyword:
    return "ReturnKeyword";

  default:
    return "NotDefined";
  }
}
