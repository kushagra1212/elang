#pragma once
#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
class BoundVariableExpression : public BoundExpression {
private:
  std::shared_ptr<BoundExpression> identiferExpression;

public:
  BoundVariableExpression(std::shared_ptr<BoundExpression> identiferExpression);

public:
  BinderKindUtils::BoundNodeKind getKind();

public:
  const std::type_info &getType();

public:
  std::shared_ptr<BoundExpression> getIdentifierExpression();
};