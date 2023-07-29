#include "BoundExpressionStatement.h"

BoundExpressionStatement::BoundExpressionStatement(
    std::shared_ptr<BoundExpression> expression) {
  this->expression = expression;
}

BinderKindUtils::BoundNodeKind BoundExpressionStatement::getKind() {
  return BinderKindUtils::BoundNodeKind::ExpressionStatement;
}

std::shared_ptr<BoundExpression> BoundExpressionStatement::getExpression() {
  return expression;
}