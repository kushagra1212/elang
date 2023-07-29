#include "BoundBlockStatement.h"
BoundBlockStatement::BoundBlockStatement(
    std::vector<std::shared_ptr<BoundStatement>> statements) {
  this->statements = statements;
}

BinderKindUtils::BoundNodeKind BoundBlockStatement::getKind() {
  return BinderKindUtils::BoundNodeKind::BlockStatement;
}

std::vector<std::shared_ptr<BoundStatement>>
BoundBlockStatement::getStatements() {
  return statements;
}