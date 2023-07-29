#pragma once
#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
class BoundAssignmentExpression : public BoundExpression {
private:
  BinderKindUtils::BoundBinaryOperatorKind op;
  std::shared_ptr<BoundExpression> left;
  std::shared_ptr<BoundExpression> right;

public:
  BoundAssignmentExpression(std::shared_ptr<BoundExpression> left,
                            BinderKindUtils::BoundBinaryOperatorKind op,
                            std::shared_ptr<BoundExpression> right);

public:
  BinderKindUtils::BoundNodeKind getKind();

public:
  const std::type_info &getType();

public:
  BinderKindUtils::BoundBinaryOperatorKind getOperator();

public:
  std::shared_ptr<BoundExpression> getLeft();

public:
  std::shared_ptr<BoundExpression> getRight();
};