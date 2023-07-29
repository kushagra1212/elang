
#include "VariableExpressionSyntax.h"

VariableExpressionSyntax::VariableExpressionSyntax(
    std::shared_ptr<ExpressionSyntax> identifierExpression) {
  this->identifierExpression = identifierExpression;
}

SyntaxKindUtils::SyntaxKind VariableExpressionSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::VariableExpression;
}

std::shared_ptr<ExpressionSyntax> VariableExpressionSyntax::getIdentifier() {
  return this->identifierExpression;
}

std::vector<std::shared_ptr<SyntaxNode>>
VariableExpressionSyntax::getChildren() {

  std::vector<std::shared_ptr<SyntaxNode>> children = {identifierExpression};
  return children;
}