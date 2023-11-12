#include "SyntaxToken.h"

template class SyntaxToken<std::any>;
template class SyntaxToken<int>;
template class SyntaxToken<double>;
template class SyntaxToken<bool>;
template class SyntaxToken<std::string>;
template class SyntaxToken<char>;

template <typename T>
SyntaxToken<T>::SyntaxToken(const std::string absoluteFilePath, int lineNumber,
                            SyntaxKindUtils::SyntaxKind kind, int columnNumber,
                            std::string text, T value) {
  this->kind = kind;
  this->text = text;
  this->value = value;
  this->lineNumber = lineNumber;
  this->columnNumber = columnNumber;
  this->absoluteFilePath = absoluteFilePath;
}

template <typename T>
SyntaxKindUtils::SyntaxKind SyntaxToken<T>::getKind() const {
  return this->kind;
}
template <typename T> int SyntaxToken<T>::getColumnNumber() {
  return this->columnNumber;
}

template <typename T> int SyntaxToken<T>::getLineNumber() {
  return this->lineNumber;
}

template <typename T> std::string SyntaxToken<T>::getText() {
  return this->text;
}

template <typename T>
const std::string &SyntaxToken<T>::getAbsoluteFilePath() const {
  return this->absoluteFilePath;
}

template <typename T> T SyntaxToken<T>::getValue() { return (this->value); }
template <typename T> std::string SyntaxToken<T>::getKindText() {
  return SyntaxKindUtils::to_string(this->kind);
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
  case SyntaxKindUtils::SyntaxKind::SlashSlashToken:
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
  case SyntaxKindUtils::SyntaxKind::EqualsToken:
    return 1;
  default:
    return 0;
  }
}
template <typename T> std::vector<SyntaxNode *> SyntaxToken<T>::getChildren() {
  return _children;
}

template <typename T>
DiagnosticUtils::SourceLocation SyntaxToken<T>::getSourceLocation() const {
  return DiagnosticUtils::SourceLocation(this->lineNumber, this->columnNumber,
                                         this->absoluteFilePath);
}