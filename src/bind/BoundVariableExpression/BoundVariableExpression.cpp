#include "BoundVariableExpression.h"

BoundVariableExpression::BoundVariableExpression(
    std::shared_ptr<BoundExpression> identiferExpression) {
  this->identiferExpression = identiferExpression;
}

BinderKindUtils::BoundNodeKind BoundVariableExpression::getKind() {
  return BinderKindUtils::BoundNodeKind::VariableExpression;
}

const std::type_info &BoundVariableExpression::getType() {
  return identiferExpression->getType();
}

std::shared_ptr<BoundExpression>
BoundVariableExpression::getIdentifierExpression() {
  return this->identiferExpression;
}
