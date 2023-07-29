#include "CallExpressionSyntax.h"

CallExpressionSyntax::CallExpressionSyntax(
    ExpressionSyntax *identifier,
    std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken,
    std::vector<ExpressionSyntax *> arguments,
    std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken) {
  this->identifier = identifier;
  this->openParenthesisToken = (openParenthesisToken);
  this->arguments = arguments;
  this->closeParenthesisToken = (closeParenthesisToken);
}

ExpressionSyntax *CallExpressionSyntax::getIdentifier() const {
  return identifier;
}

std::shared_ptr<SyntaxToken<std::any>>
CallExpressionSyntax::getOpenParenthesisToken() {
  return (openParenthesisToken);
}

std::vector<ExpressionSyntax *> CallExpressionSyntax::getArguments() const {
  return arguments;
}

std::shared_ptr<SyntaxToken<std::any>>
CallExpressionSyntax::getCloseParenthesisToken() {
  return (closeParenthesisToken);
}

SyntaxKindUtils::SyntaxKind CallExpressionSyntax::getKind() {
  return SyntaxKindUtils::CallExpression;
}

std::vector<SyntaxNode *> CallExpressionSyntax::getChildren() {
  std::vector<SyntaxNode *> children;
  children.push_back(identifier);
  children.push_back(new SyntaxToken<std::any>(*openParenthesisToken));
  for (auto &argument : arguments) {
    children.push_back(argument);
  }
  children.push_back(new SyntaxToken<std::any>(*closeParenthesisToken));
  return children;
}
