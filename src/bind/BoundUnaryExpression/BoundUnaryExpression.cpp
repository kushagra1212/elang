#include "BoundUnaryExpression.h"

BoundUnaryExpression::BoundUnaryExpression(

    const std::string &lineAndColumn,
    BinderKindUtils::BoundUnaryOperatorKind op, BoundExpression *operand) {
  this->op = op;
  this->operand = operand;
  this->_lineAndColumn = lineAndColumn;
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

BoundExpression *BoundUnaryExpression::getOperand() { return operand; }

std::vector<BoundNode *> BoundUnaryExpression::getChildren() {
  return std::vector<BoundNode *>{operand};
}

std::string BoundUnaryExpression::getLineNumberAndColumn() const {
  return this->_lineAndColumn;
}

BoundUnaryExpression::~BoundUnaryExpression() {
  if (operand != nullptr) {
    delete operand;
    operand = nullptr;
  }
}
