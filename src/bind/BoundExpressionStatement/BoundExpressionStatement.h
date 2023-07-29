#pragma once
#include "../../Common.h"
#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
#include "../BoundStatement/BoundStatement.h"

class BoundExpressionStatement : public BoundStatement {

private:
  std::shared_ptr<BoundExpression> expression;

public:
  BoundExpressionStatement(std::shared_ptr<BoundExpression> expression);

public:
  BinderKindUtils::BoundNodeKind getKind();

public:
  std::shared_ptr<BoundExpression> getExpression();
};
