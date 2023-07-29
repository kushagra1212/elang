#include "BoundUnaryExpression.h"

BoundUnaryExpression::BoundUnaryExpression(
    BinderKindUtils::BoundUnaryOperatorKind op,
    std::shared_ptr<BoundExpression> operand) {
  this->op = op;
  this->operand = operand;
}

BinderKindUtils::BoundNodeKind BoundUnaryExpression::getKind() {
  return BinderKindUtils::BoundNodeKind::UnaryExpression;
}

const std::type_info &BoundUnaryExpression::getType() {
  return operand->getType();
}

BinderKindUtils::BoundUnaryOperatorKind BoundUnaryExpression::getOperator() {
  return op;
}

std::shared_ptr<BoundExpression> BoundUnaryExpression::getOperand() {
  return operand;
}
