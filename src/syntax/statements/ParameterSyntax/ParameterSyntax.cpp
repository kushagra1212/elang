

#include "ParameterSyntax.h"

ParameterSyntax::ParameterSyntax(
    std::unique_ptr<SyntaxToken<std::any>> identifierToken) {
  this->_identifierToken = std::move(identifierToken);

  // Add children

  _children.push_back(_identifierToken.get());
}

std::unique_ptr<SyntaxToken<std::any>> ParameterSyntax::getIdentifierToken() {
  return std::move(_identifierToken);
}

SyntaxKindUtils::SyntaxKind ParameterSyntax::getKind() const {
  return SyntaxKindUtils::SyntaxKind::ParameterSyntax;
}

std::string ParameterSyntax::getLineNumberAndColumn() {
  return _identifierToken->getLineNumberAndColumn();
}

std::vector<SyntaxNode *> ParameterSyntax::getChildren() {
  return this->_children;
}
