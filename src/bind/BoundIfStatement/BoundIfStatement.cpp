#include "BoundIfStatement.h"

BoundIfStatement::BoundIfStatement(
    std::shared_ptr<BoundExpression> condition,
    std::shared_ptr<BoundStatement> thenStatement,
    std::shared_ptr<BoundStatement> elseStatement)
    : _condition(condition), _thenStatement(thenStatement),
      _elseStatement(elseStatement) {}

BinderKindUtils::BoundNodeKind BoundIfStatement::getKind() {

  return BinderKindUtils::BoundNodeKind::IfStatement;
}

std::shared_ptr<BoundExpression> BoundIfStatement::getCondition() const {
  return _condition;
}

std::shared_ptr<BoundStatement> BoundIfStatement::getThenStatement() const {
  return _thenStatement;
}

std::shared_ptr<BoundStatement> BoundIfStatement::getElseStatement() const {
  return _elseStatement;
}
