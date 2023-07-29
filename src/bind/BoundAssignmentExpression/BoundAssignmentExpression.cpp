#include "BoundAssignmentExpression.h"

BoundAssignmentExpression::BoundAssignmentExpression(
    std::shared_ptr<BoundExpression> left,
    BinderKindUtils::BoundBinaryOperatorKind op,
    std::shared_ptr<BoundExpression> right) {
  this->op = op;
  this->left = left;
  this->right = right;
}

BinderKindUtils::BoundNodeKind BoundAssignmentExpression::getKind() {
  return BinderKindUtils::BoundNodeKind::AssignmentExpression;
}

const std::type_info &BoundAssignmentExpression::getType() {
  return right->getType();
}

BinderKindUtils::BoundBinaryOperatorKind
BoundAssignmentExpression::getOperator() {
  return op;
}

std::shared_ptr<BoundExpression> BoundAssignmentExpression::getLeft() {
  return left;
}

std::shared_ptr<BoundExpression> BoundAssignmentExpression::getRight() {
  return right;
}
