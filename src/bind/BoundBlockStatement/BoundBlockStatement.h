#pragma once
#include "../../Common.h"
#include "../BinderKindUtils.h"
#include "../BoundStatement/BoundStatement.h"
class BoundBlockStatement : public BoundStatement {
private:
  std::vector<std::shared_ptr<BoundStatement>> statements;

public:
  BoundBlockStatement(std::vector<std::shared_ptr<BoundStatement>> statements);

public:
  BinderKindUtils::BoundNodeKind getKind();

public:
  std::vector<std::shared_ptr<BoundStatement>> getStatements();
};