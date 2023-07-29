#pragma once

#include "../../Common.h"
#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
class BoundBinaryExpression : public BoundExpression {
private:
  BinderKindUtils::BoundBinaryOperatorKind op;
  std::shared_ptr<BoundExpression> left;
  std::shared_ptr<BoundExpression> right;

public:
  BoundBinaryExpression(std::shared_ptr<BoundExpression> left,
                        BinderKindUtils::BoundBinaryOperatorKind op,
                        std::shared_ptr<BoundExpression> right);

public:
  BinderKindUtils::BoundNodeKind getKind() override;

public:
  const std::type_info &getType() override;

public:
  BinderKindUtils::BoundBinaryOperatorKind getOperator();

public:
  std::shared_ptr<BoundExpression> getLeft();

public:
  std::shared_ptr<BoundExpression> getRight();
};
