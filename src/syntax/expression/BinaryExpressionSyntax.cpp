#include "BinaryExpressionSyntax.h"

BinaryExpressionSyntax::BinaryExpressionSyntax(
    ExpressionSyntax *left,
    std::shared_ptr<SyntaxToken<std::any>> operatorToken,
    ExpressionSyntax *right) {
  this->left = left;
  this->operatorToken = (operatorToken);
  this->right = right;
  children.push_back(this->left);
  children.push_back(new SyntaxToken<std::any>(*operatorToken));
  children.push_back(this->right);
}

SyntaxKindUtils::SyntaxKind BinaryExpressionSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::BinaryExpression;
}

ExpressionSyntax *BinaryExpressionSyntax::getLeft() { return this->left; }

std::shared_ptr<SyntaxToken<std::any>>
BinaryExpressionSyntax::getOperatorToken() {
  return (this->operatorToken);
}

ExpressionSyntax *BinaryExpressionSyntax::getRight() { return this->right; }

std::vector<SyntaxNode *> BinaryExpressionSyntax::getChildren() {
  return children;
}