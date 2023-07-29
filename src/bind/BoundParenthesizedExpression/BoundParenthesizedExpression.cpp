#include "BoundParenthesizedExpression.h"

BoundParenthesizedExpression::BoundParenthesizedExpression(
    std::shared_ptr<BoundExpression> expression) {
  this->expression = expression;
}

BinderKindUtils::BoundNodeKind BoundParenthesizedExpression::getKind() {
  return BinderKindUtils::BoundNodeKind::ParenthesizedExpression;
}

const std::type_info &BoundParenthesizedExpression::getType() {
  return expression->getType();
}

std::shared_ptr<BoundExpression> BoundParenthesizedExpression::getExpression() {
  return expression;
}
