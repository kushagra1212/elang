#pragma once
#include "../../../Common.h"
#include "../../../utils/Utils.h"
#include "../../BoundExpression.h"
#include "../../BoundStatement/BoundStatement.h"
class BoundScopeGlobal {
public:
  BoundScopeGlobal(std::shared_ptr<BoundScopeGlobal> previous,
                   std::map<std::string, Utils::Variable> variables,
                   std::vector<std::string> logs,
                   std::shared_ptr<BoundStatement> statement);

public:
  std::shared_ptr<BoundScopeGlobal> previous;
  std::map<std::string, Utils::Variable> variables;
  std::vector<std::string> logs;
  std::shared_ptr<BoundStatement> statement;
};