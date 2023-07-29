#include "UnaryExpressionSyntax.h"

UnaryExpressionSyntax::UnaryExpressionSyntax(
    std::shared_ptr<SyntaxToken<std::any>> operatorToken,
    std::shared_ptr<ExpressionSyntax> operand) {

  this->operatorToken = (operatorToken);
  this->operand = operand;
}

SyntaxKindUtils::SyntaxKind UnaryExpressionSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::UnaryExpression;
}

std::shared_ptr<SyntaxToken<std::any>>
UnaryExpressionSyntax::getOperatorToken() {

  return (this->operatorToken);
}

std::shared_ptr<ExpressionSyntax> UnaryExpressionSyntax::getOperand() {
  return this->operand;
}

std::vector<std::shared_ptr<SyntaxNode>> UnaryExpressionSyntax::getChildren() {
  std::vector<std::shared_ptr<SyntaxNode>> children;
  children.push_back(
      std::dynamic_pointer_cast<SyntaxNode>(this->operatorToken));
  children.push_back(std::dynamic_pointer_cast<SyntaxNode>(this->operand));
  return children;
}