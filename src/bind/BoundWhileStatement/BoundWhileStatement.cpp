#include "BoundWhileStatement.h"

BoundWhileStatement::BoundWhileStatement(
    std::shared_ptr<BoundExpression> condition,
    std::shared_ptr<BoundStatement> body)
    : _condition(condition), _body(body) {}

BinderKindUtils::BoundNodeKind BoundWhileStatement::getKind() {

  return BinderKindUtils::BoundNodeKind::WhileStatement;
}

std::shared_ptr<BoundExpression> BoundWhileStatement::getCondition() const {
  return _condition;
}

std::shared_ptr<BoundStatement> BoundWhileStatement::getBody() const {
  return _body;
}
