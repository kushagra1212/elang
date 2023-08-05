#include "BoundScope.h"

BoundScope::BoundScope(BoundScope *parent) : parent(parent) {}

bool BoundScope::tryDeclareVariable(
    std::string name, const struct Utils::Variable &initialValue) {
  if (this->variables.find(name) == this->variables.end()) {

    this->variables[name] = initialValue;
    return true;
  }

  return false;
}

bool BoundScope::tryDeclareFunction(
    std::string name, const struct Utils::FunctionSymbol &function) {
  if (this->functions.find(name) != this->functions.end()) {
    return false;
  }
  this->functions[name] = function;
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
  std::vector<std::string> keys;
  if (this->parent != nullptr) {
    keys = this->parent->getVariablesKeys();
  }
  for (auto const &x : this->variables) {
    keys.push_back(x.first);
  }
  return keys;
}

std::map<std::string, Utils::Variable> BoundScope::getVariables() {
  std::map<std::string, Utils::Variable> variables;
  if (this->parent != nullptr) {
    variables = this->parent->getVariables();
  }
  for (auto const &x : this->variables) {
    variables[x.first] = x.second;
  }
  return variables;
}

BoundScope::~BoundScope() { delete this->parent; }