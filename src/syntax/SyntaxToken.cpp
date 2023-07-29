#include "SyntaxToken.h"

template class SyntaxToken<std::any>;
template class SyntaxToken<int>;
template class SyntaxToken<double>;
template class SyntaxToken<bool>;
template class SyntaxToken<std::string>;
template class SyntaxToken<char>;

template <typename T>
SyntaxToken<T>::SyntaxToken(int lineNumber, SyntaxKindUtils::SyntaxKind kind,
                            int position, std::string text, T value) {
  this->kind = kind;
  this->position = position;
  this->text = text;
  this->value = value;
  this->lineNumber = lineNumber;
}

template <typename T> SyntaxKindUtils::SyntaxKind SyntaxToken<T>::getKind() {
  return this->kind;
}
template <typename T> int SyntaxToken<T>::getPosition() {
  return this->position;
}

template <typename T> int SyntaxToken<T>::getLineNumber() {
  return this->lineNumber;
}

template <typename T> std::string SyntaxToken<T>::getText() {
  return this->text;
}
template <typename T> T SyntaxToken<T>::getValue() { return (this->value); }
template <typename T> std::string SyntaxToken<T>::getKindText() {
  return SyntaxKindUtils::enum_to_string_map[this->kind];
}
template <typename T> int SyntaxToken<T>::getUnaryOperatorPrecedence() {
  switch (this->kind) {
  case SyntaxKindUtils::SyntaxKind::PlusToken:
  case SyntaxKindUtils::SyntaxKind::MinusToken:
  case SyntaxKindUtils::SyntaxKind::BangToken:
  case SyntaxKindUtils::SyntaxKind::TildeToken:
    return 10;
  default:
    return 0;
  }
}
template <typename T> int SyntaxToken<T>::getBinaryOperatorPrecedence() {
  switch (this->kind) {
  case SyntaxKindUtils::SyntaxKind::StarToken:
  case SyntaxKindUtils::SyntaxKind::SlashToken:
  case SyntaxKindUtils::SyntaxKind::PercentToken:
    return 9;
  case SyntaxKindUtils::SyntaxKind::PlusToken:
  case SyntaxKindUtils::SyntaxKind::MinusToken:
    return 8;

  case SyntaxKindUtils::SyntaxKind::LessToken:
  case SyntaxKindUtils::SyntaxKind::LessOrEqualsToken:
  case SyntaxKindUtils::SyntaxKind::GreaterToken:
  case SyntaxKindUtils::SyntaxKind::GreaterOrEqualsToken:
    return 7;
  case SyntaxKindUtils::SyntaxKind::EqualsEqualsToken:
  case SyntaxKindUtils::SyntaxKind::BangEqualsToken:
    return 6;
  case SyntaxKindUtils::SyntaxKind::AmpersandToken:
    return 5;
  case SyntaxKindUtils::SyntaxKind::CaretToken:
    return 4;
  case SyntaxKindUtils::SyntaxKind::PipeToken:
    return 3;

  case SyntaxKindUtils::SyntaxKind::AmpersandAmpersandToken:
    return 2;
  case SyntaxKindUtils::SyntaxKind::PipePipeToken:
    return 1;
  default:
    return 0;
  }
}
template <typename T>
std::vector<std::shared_ptr<SyntaxNode>> SyntaxToken<T>::getChildren() {
  return children;
}
