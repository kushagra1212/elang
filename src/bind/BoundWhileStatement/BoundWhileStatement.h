#pragma once
#include "../../Common.h"
#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
#include "../BoundStatement/BoundStatement.h"
class BoundWhileStatement : public BoundStatement {
private:
  std::shared_ptr<BoundExpression> _condition;
  std::shared_ptr<BoundStatement> _body;

public:
  BoundWhileStatement(std::shared_ptr<BoundExpression> condition,
                      std::shared_ptr<BoundStatement> body);
  BinderKindUtils::BoundNodeKind getKind() override;
  std::shared_ptr<BoundExpression> getCondition() const;
  std::shared_ptr<BoundStatement> getBody() const;
};