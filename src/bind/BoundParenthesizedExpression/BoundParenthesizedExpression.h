#pragma once
#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
#include <typeinfo>

class BoundParenthesizedExpression : public BoundExpression {
private:
  std::shared_ptr<BoundExpression> expression;

public:
  BoundParenthesizedExpression(std::shared_ptr<BoundExpression> expression);

public:
public:
  BinderKindUtils::BoundNodeKind getKind();

public:
  const std::type_info &getType();

public:
  std::shared_ptr<BoundExpression> getExpression();
};
