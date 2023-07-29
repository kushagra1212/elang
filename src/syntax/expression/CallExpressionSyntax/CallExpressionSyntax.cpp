#include "CallExpressionSyntax.h"

CallExpressionSyntax::CallExpressionSyntax(
    std::shared_ptr<ExpressionSyntax> identifier,
    std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken,
    std::vector<std::shared_ptr<ExpressionSyntax>> arguments,
    std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken) {
  this->identifier = identifier;
  this->openParenthesisToken = (openParenthesisToken);
  this->arguments = arguments;
  this->closeParenthesisToken = (closeParenthesisToken);
}

std::shared_ptr<ExpressionSyntax> CallExpressionSyntax::getIdentifier() const {
  return identifier;
}

std::shared_ptr<SyntaxToken<std::any>>
CallExpressionSyntax::getOpenParenthesisToken() {
  return (openParenthesisToken);
}

std::vector<std::shared_ptr<ExpressionSyntax>>
CallExpressionSyntax::getArguments() const {
  return arguments;
}

std::shared_ptr<SyntaxToken<std::any>>
CallExpressionSyntax::getCloseParenthesisToken() {
  return (closeParenthesisToken);
}

SyntaxKindUtils::SyntaxKind CallExpressionSyntax::getKind() {
  return SyntaxKindUtils::CallExpression;
}

std::vector<std::shared_ptr<SyntaxNode>> CallExpressionSyntax::getChildren() {
  std::vector<std::shared_ptr<SyntaxNode>> children;
  children.push_back(identifier);
  children.push_back(openParenthesisToken);
  for (auto &argument : arguments) {
    children.push_back(argument);
  }
  children.push_back(closeParenthesisToken);
  return children;
}
