#pragma once
#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
class BoundAssignmentExpression : public BoundExpression {
private:
  BinderKindUtils::BoundBinaryOperatorKind _op;
  std::unique_ptr<BoundExpression> _left;
  std::unique_ptr<BoundExpression> _right;

public:
  BoundAssignmentExpression(std::string lineAndColumn,
                            std::unique_ptr<BoundExpression> left,
                            BinderKindUtils::BoundBinaryOperatorKind op,
                            std::unique_ptr<BoundExpression> right);

  const std::type_info &getType();

  BinderKindUtils::BoundBinaryOperatorKind getOperator();

  std::unique_ptr<BoundExpression> getLeft();

  std::unique_ptr<BoundExpression> getRight();

  BinderKindUtils::BoundNodeKind getKind() const override;

  std::string getLineNumberAndColumn() override;

  std::vector<BoundNode *> getChildren() override;
};