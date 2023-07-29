#include "ParenthesizedExpressionSyntax.h"

ParenthesizedExpressionSyntax::ParenthesizedExpressionSyntax(
    std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken,
    ExpressionSyntax *expression,
    std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken) {
  this->openParenthesisToken = (openParenthesisToken);
  this->expression = expression;
  this->closeParenthesisToken = (closeParenthesisToken);
  children.push_back(new SyntaxToken<std::any>(*this->openParenthesisToken));
  children.push_back(this->expression);
  children.push_back(new SyntaxToken<std::any>(*this->closeParenthesisToken));
}

SyntaxKindUtils::SyntaxKind ParenthesizedExpressionSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::ParenthesizedExpression;
}

std::shared_ptr<SyntaxToken<std::any>>
ParenthesizedExpressionSyntax::getOpenParenthesisToken() {
  return (this->openParenthesisToken);
}

ExpressionSyntax *ParenthesizedExpressionSyntax::getExpression() {
  return this->expression;
}

std::shared_ptr<SyntaxToken<std::any>>
ParenthesizedExpressionSyntax::getCloseParenthesisToken() {
  return (this->closeParenthesisToken);
}

std::vector<SyntaxNode *> ParenthesizedExpressionSyntax::getChildren() {
  return children;
}