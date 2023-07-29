#include "VariableDeclarationSyntax.h"

VariableDeclarationSyntax::VariableDeclarationSyntax(
    std::shared_ptr<SyntaxToken<std::any>> keyword,
    std::shared_ptr<SyntaxToken<std::any>> identifier,
    std::shared_ptr<SyntaxToken<std::any>> equalsToken,
    ExpressionSyntax *initializer)
    : _keyword((keyword)), _identifier((identifier)),
      _equalsToken((equalsToken)), _initializer(initializer) {}

SyntaxKindUtils::SyntaxKind VariableDeclarationSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::VariableDeclaration;
}

std::vector<SyntaxNode *> VariableDeclarationSyntax::getChildren() {
  return std::vector<SyntaxNode *>{new SyntaxToken<std::any>(*_keyword),
                                   new SyntaxToken<std::any>(*_identifier),
                                   new SyntaxToken<std::any>(*_equalsToken),
                                   _initializer};
}

std::shared_ptr<SyntaxToken<std::any>>
VariableDeclarationSyntax::getIdentifier() {
  return (_identifier);
}

std::shared_ptr<SyntaxToken<std::any>>
VariableDeclarationSyntax::getEqualsToken() {
  return (_equalsToken);
}

ExpressionSyntax *VariableDeclarationSyntax::getInitializer() const {
  return _initializer;
}

std::shared_ptr<SyntaxToken<std::any>> VariableDeclarationSyntax::getKeyword() {
  return (_keyword);
}

// Path:
// src/syntax/statements/VariableDeclarationSyntax/VariableDeclarationSyntax.h