#include "ExpressionStatementSyntax.h"

ExpressionStatementSyntax::ExpressionStatementSyntax(
    std::shared_ptr<ExpressionSyntax> expression) {
  this->expression = expression;
}

SyntaxKindUtils::SyntaxKind ExpressionStatementSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::ExpressionStatement;
}

std::vector<std::shared_ptr<SyntaxNode>>
ExpressionStatementSyntax::getChildren() {
  return std::vector<std::shared_ptr<SyntaxNode>>{this->expression};
}

std::shared_ptr<ExpressionSyntax> ExpressionStatementSyntax::getExpression() {
  return this->expression;
}