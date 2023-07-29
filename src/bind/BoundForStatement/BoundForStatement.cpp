#include "BoundForStatement.h"

BoundForStatement::BoundForStatement(
    std::shared_ptr<BoundStatement> initialization,

    std::shared_ptr<BoundExpression> upperBound,
    std::shared_ptr<BoundStatement> statement)
    : initialization((initialization)), upperBound(upperBound),
      statement((statement)) {}

std::shared_ptr<BoundStatement> BoundForStatement::getInitialization() const {

  return this->initialization;
}

std::shared_ptr<BoundStatement> BoundForStatement::getStatement() const {

  return this->statement;
}

BinderKindUtils::BoundNodeKind BoundForStatement::getKind() {

  return BinderKindUtils::BoundNodeKind::ForStatement;
}

std::shared_ptr<BoundExpression> BoundForStatement::getUpperBound() const {

  return this->upperBound;
}
