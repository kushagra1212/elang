#include "BinaryExpressionSyntax.h"

BinaryExpressionSyntax::BinaryExpressionSyntax(
    std::shared_ptr<ExpressionSyntax> left,
    std::shared_ptr<SyntaxToken<std::any>> operatorToken,
    std::shared_ptr<ExpressionSyntax> right) {
  this->left = left;
  this->operatorToken = (operatorToken);
  this->right = right;
}

SyntaxKindUtils::SyntaxKind BinaryExpressionSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::BinaryExpression;
}

std::shared_ptr<ExpressionSyntax> BinaryExpressionSyntax::getLeft() {
  return this->left;
}

std::shared_ptr<SyntaxToken<std::any>>
BinaryExpressionSyntax::getOperatorToken() {
  return (this->operatorToken);
}

std::shared_ptr<ExpressionSyntax> BinaryExpressionSyntax::getRight() {
  return this->right;
}

std::vector<std::shared_ptr<SyntaxNode>> BinaryExpressionSyntax::getChildren() {
  std::vector<std::shared_ptr<SyntaxNode>> children;
  children.push_back(this->left);
  children.push_back(operatorToken);
  children.push_back(this->right);
  return children;
}