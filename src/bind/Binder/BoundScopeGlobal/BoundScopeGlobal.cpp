#include "BoundScopeGlobal.h"

BoundScopeGlobal::BoundScopeGlobal(
    std::unique_ptr<BoundScopeGlobal> previous,
    std::map<std::string, struct Utils::Variable> variables,
    std::map<std::string, BoundFunctionDeclaration *> functions,
    std::vector<std::string> logs, std::unique_ptr<BoundStatement> statement)
    : variables(variables), logs(logs), functions(functions) {

  this->statement = std::move(statement);

  this->previous = std::move(previous);
}

BoundScopeGlobal::~BoundScopeGlobal() {

  for (auto &function : this->functions) {
    if (function.second != nullptr) {
      delete function.second;
      function.second = nullptr;
    }
  }
  this->functions.clear();
}

bool BoundScopeGlobal::tryLookupVariable(std::string name) {
  if (this->variables.find(name) != this->variables.end()) {
    return true;
  }
  if (this->previous == nullptr) {
    return false;
  }
  return this->previous->tryLookupVariable(name);
}

bool BoundScopeGlobal::tryAssignVariable(std::string name,
                                         const struct Utils::Variable &value) {
  if (this->variables.find(name) != this->variables.end()) {
    this->variables[name] = value;
    return true;
  }
  if (this->previous == nullptr) {
    return false;
  }
  return this->previous->tryAssignVariable(name, value);
}

Utils::Variable BoundScopeGlobal::getVariable(std::string name) {
  if (this->variables.find(name) != this->variables.end()) {
    return this->variables[name];
  }
  return this->previous->getVariable(name);
}