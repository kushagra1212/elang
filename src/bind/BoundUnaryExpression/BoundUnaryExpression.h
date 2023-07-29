#pragma once
#include "../../Common.h"
#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
class BoundUnaryExpression : public BoundExpression {
private:
  BinderKindUtils::BoundUnaryOperatorKind op;
  std::shared_ptr<BoundExpression> operand;

public:
  BoundUnaryExpression(BinderKindUtils::BoundUnaryOperatorKind op,
                       std::shared_ptr<BoundExpression> operand);

public:
  BinderKindUtils::BoundNodeKind getKind();

public:
  const std::type_info &getType();

public:
  BinderKindUtils::BoundUnaryOperatorKind getOperator();

public:
  std::shared_ptr<BoundExpression> getOperand();
};