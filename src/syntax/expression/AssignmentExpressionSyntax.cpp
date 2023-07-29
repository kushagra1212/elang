#include "AssignmentExpressionSyntax.h"

AssignmentExpressionSyntax::AssignmentExpressionSyntax(
    std::shared_ptr<ExpressionSyntax> left,
    std::shared_ptr<SyntaxToken<std::any>> operatorToken,
    std::shared_ptr<ExpressionSyntax> right)
    : left(left), operatorToken((operatorToken)), right(right) {}

SyntaxKindUtils::SyntaxKind AssignmentExpressionSyntax::getKind() {

  return SyntaxKindUtils::SyntaxKind::AssignmentExpression;
}

std::vector<std::shared_ptr<SyntaxNode>>
AssignmentExpressionSyntax::getChildren() {

  std::vector<std::shared_ptr<SyntaxNode>> children = {left, operatorToken,
                                                       (right)};

  return children;
}

std::shared_ptr<SyntaxToken<std::any>>
AssignmentExpressionSyntax::getOperatorToken() {
  return (operatorToken);
}

std::shared_ptr<ExpressionSyntax> AssignmentExpressionSyntax::getRight() {
  return right;
}

std::shared_ptr<ExpressionSyntax> AssignmentExpressionSyntax::getLeft() {
  return left;
}