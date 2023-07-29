#include "ParenthesizedExpressionSyntax.h"

ParenthesizedExpressionSyntax::ParenthesizedExpressionSyntax(
    std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken,
    std::shared_ptr<ExpressionSyntax> expression,
    std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken) {
  this->openParenthesisToken = (openParenthesisToken);
  this->expression = expression;
  this->closeParenthesisToken = (closeParenthesisToken);
}

SyntaxKindUtils::SyntaxKind ParenthesizedExpressionSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::ParenthesizedExpression;
}

std::shared_ptr<SyntaxToken<std::any>>
ParenthesizedExpressionSyntax::getOpenParenthesisToken() {
  return (this->openParenthesisToken);
}

std::shared_ptr<ExpressionSyntax>
ParenthesizedExpressionSyntax::getExpression() {
  return this->expression;
}

std::shared_ptr<SyntaxToken<std::any>>
ParenthesizedExpressionSyntax::getCloseParenthesisToken() {
  return (this->closeParenthesisToken);
}

std::vector<std::shared_ptr<SyntaxNode>>
ParenthesizedExpressionSyntax::getChildren() {

  std::vector<std::shared_ptr<SyntaxNode>> children;
  children.push_back(this->openParenthesisToken);
  children.push_back(this->expression);
  children.push_back(this->closeParenthesisToken);
  return children;
}