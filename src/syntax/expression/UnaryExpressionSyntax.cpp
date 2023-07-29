#include "UnaryExpressionSyntax.h"

UnaryExpressionSyntax::UnaryExpressionSyntax(
    std::shared_ptr<SyntaxToken<std::any>> operatorToken,
    ExpressionSyntax *operand) {

  this->operatorToken = (operatorToken);
  this->operand = operand;
  children.push_back(new SyntaxToken<std::any>(*this->operatorToken));
  children.push_back(this->operand);
}

SyntaxKindUtils::SyntaxKind UnaryExpressionSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::UnaryExpression;
}

std::shared_ptr<SyntaxToken<std::any>>
UnaryExpressionSyntax::getOperatorToken() {
  return (this->operatorToken);
}

ExpressionSyntax *UnaryExpressionSyntax::getOperand() { return this->operand; }

std::vector<SyntaxNode *> UnaryExpressionSyntax::getChildren() {
  return children;
}