
#include "SyntaxKindUtils.h"

std::map<SyntaxKindUtils::SyntaxKind, std::string>
    SyntaxKindUtils::enum_to_string_map;
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

void SyntaxKindUtils::init_enum_to_string_map() {
  enum_to_string_map[SyntaxKind::NumberToken] = "NumberToken";
  enum_to_string_map[SyntaxKind::TrueKeyword] = "TrueKeyword";
  enum_to_string_map[SyntaxKind::FalseKeyword] = "FalseKeyword";
  enum_to_string_map[SyntaxKind::WhitespaceToken] = "WhitespaceToken";
  enum_to_string_map[SyntaxKind::PlusToken] = "PlusToken";
  enum_to_string_map[SyntaxKind::MinusToken] = "MinusToken";
  enum_to_string_map[SyntaxKind::StarToken] = "StarToken";
  enum_to_string_map[SyntaxKind::SlashToken] = "SlashToken";
  enum_to_string_map[SyntaxKind::OpenParenthesisToken] = "OpenParenthesisToken";
  enum_to_string_map[SyntaxKind::CloseParenthesisToken] =
      "CloseParenthesisToken";
  enum_to_string_map[SyntaxKind::BadToken] = "BadToken";
  enum_to_string_map[SyntaxKind::EndOfFileToken] = "EndOfFileToken";
  enum_to_string_map[SyntaxKind::LiteralExpression] = "LiteralExpression";
  enum_to_string_map[SyntaxKind::UnaryExpression] = "UnaryExpression";
  enum_to_string_map[SyntaxKind::BinaryExpression] = "BinaryExpression";
  enum_to_string_map[SyntaxKind::ParenthesizedExpression] =
      "ParenthesizedExpression";
  enum_to_string_map[SyntaxKind::CompilationUnit] = "CompilationUnit";
  enum_to_string_map[SyntaxKind::AmpersandAmpersandToken] =
      "AmpersandAmpersandToken";
  enum_to_string_map[SyntaxKind::PipePipeToken] = "PipePipeToken";
  enum_to_string_map[SyntaxKind::EqualsEqualsToken] = "EqualsEqualsToken";
  enum_to_string_map[SyntaxKind::EqualsToken] = "EqualsToken";
  enum_to_string_map[SyntaxKind::BangToken] = "BangToken";
  enum_to_string_map[SyntaxKind::BangEqualsToken] = "BangEqualsToken";
  enum_to_string_map[SyntaxKind::LessOrEqualsToken] = "LessOrEqualsToken";
  enum_to_string_map[SyntaxKind::LessToken] = "LessToken";
  enum_to_string_map[SyntaxKind::GreaterOrEqualsToken] = "GreaterOrEqualsToken";
  enum_to_string_map[SyntaxKind::GreaterToken] = "GreaterToken";
  enum_to_string_map[SyntaxKind::SemiColonToken] = "SemiColonToken";
  enum_to_string_map[SyntaxKind::CommaToken] = "CommaToken";
  enum_to_string_map[SyntaxKind::OpenBraceToken] = "OpenBraceToken";
  enum_to_string_map[SyntaxKind::CloseBraceToken] = "CloseBraceToken";
  enum_to_string_map[SyntaxKind::StringToken] = "StringToken";
  enum_to_string_map[SyntaxKind::IdentifierToken] = "IdentifierToken";

  enum_to_string_map[SyntaxKind::SemicolonToken] = "SemicolonToken";

  enum_to_string_map[SyntaxKind::AmpersandToken] = "AmpersandToken";
  enum_to_string_map[SyntaxKind::PipeToken] = "PipeToken";
  enum_to_string_map[SyntaxKind::CaretToken] = "CaretToken";
  enum_to_string_map[SyntaxKind::TildeToken] = "TildeToken";
  enum_to_string_map[SyntaxKind::PercentToken] = "PercentToken";
  enum_to_string_map[SyntaxKind::VariableExpression] = "VariableExpression";
  enum_to_string_map[SyntaxKind::AssignmentExpression] = "AssignmentExpression";
  enum_to_string_map[SyntaxKind::PercentToken] = "PercentToken";

  enum_to_string_map[SyntaxKind::EndOfLineToken] = "EndOfLineToken";
  enum_to_string_map[SyntaxKind::VarKeyword] = "VarKeyword";
  enum_to_string_map[SyntaxKind::ConstKeyword] = "ConstKeyword";
  enum_to_string_map[SyntaxKind::VariableDeclaration] = "VariableDeclaration";
  enum_to_string_map[SyntaxKind::IfKeyword] = "IfKeyword";
  enum_to_string_map[SyntaxKind::ElseKeyword] = "ElseKeyword";
  enum_to_string_map[SyntaxKind::ElseClause] = "ElseClause";
  enum_to_string_map[SyntaxKind::IfStatement] = "IfStatement";
  enum_to_string_map[SyntaxKind::WhileKeyword] = "WhileKeyword";
  enum_to_string_map[SyntaxKind::WhileStatement] = "WhileStatement";
  enum_to_string_map[SyntaxKind::ForKeyword] = "ForKeyword";
  enum_to_string_map[SyntaxKind::ToKeyword] = "ToKeyword";
  enum_to_string_map[SyntaxKind::ForStatement] = "ForStatement";
  enum_to_string_map[SyntaxKind::BlockStatement] = "BlockStatement";
  enum_to_string_map[SyntaxKind::ExpressionStatement] = "ExpressionStatement";
}