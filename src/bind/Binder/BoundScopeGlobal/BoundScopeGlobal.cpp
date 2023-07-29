#include "BoundScopeGlobal.h"

BoundScopeGlobal::BoundScopeGlobal(
    std::shared_ptr<BoundScopeGlobal> previous,
    std::map<std::string, struct Utils::Variable> variables,
    std::vector<std::string> logs, std::shared_ptr<BoundStatement> statement)
    : previous(previous), variables(variables), logs(logs),
      statement(statement) {}