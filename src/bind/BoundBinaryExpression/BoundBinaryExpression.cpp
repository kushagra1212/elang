#include "BoundBinaryExpression.h"

BoundBinaryExpression::BoundBinaryExpression(
    std::shared_ptr<BoundExpression> left,
    BinderKindUtils::BoundBinaryOperatorKind op,
    std::shared_ptr<BoundExpression> right)
    : op(op), left(left), right(right) {}

BinderKindUtils::BoundNodeKind BoundBinaryExpression::getKind() {
  return BinderKindUtils::BoundNodeKind::BinaryExpression;
}

const std::type_info &BoundBinaryExpression::getType() {
  return left->getType();
}

BinderKindUtils::BoundBinaryOperatorKind BoundBinaryExpression::getOperator() {
  return op;
}

std::shared_ptr<BoundExpression> BoundBinaryExpression::getLeft() {
  return left;
}

std::shared_ptr<BoundExpression> BoundBinaryExpression::getRight() {
  return right;
}
