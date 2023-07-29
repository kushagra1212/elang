#include "BoundScope.h"

BoundScope::BoundScope(std::shared_ptr<BoundScope> parent) : parent(parent) {}

bool BoundScope::tryDeclareVariable(
    std::string name, const struct Utils::Variable &initialValue) {
  if (this->variables.find(name) != this->variables.end()) {
    return false;
  }
  this->variables[name] = initialValue;
  return true;
}

bool BoundScope::tryLookupVariable(std::string name) {
  if (this->variables.find(name) != this->variables.end()) {
    return true;
  }
  if (this->parent == nullptr) {
    return false;
  }
  return this->parent->tryLookupVariable(name);
}

bool BoundScope::tryAssignVariable(std::string name,
                                   const struct Utils::Variable &value) {
  if (this->variables.find(name) != this->variables.end()) {
    this->variables[name] = value;
    return true;
  }
  if (this->parent == nullptr) {
    return false;
  }
  return this->parent->tryAssignVariable(name, value);
}

std::vector<std::string> BoundScope::getVariablesKeys() {
  std::vector<std::string> variables;
  for (auto &variable : this->variables) {
    variables.push_back(variable.first);
  }
  return variables;
}