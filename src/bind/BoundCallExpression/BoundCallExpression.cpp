#include "BoundCallExpression.h"

BoundCallExpression::BoundCallExpression(
    Utils::FunctionSymbol functionalSymbol,
    const std::vector<std::shared_ptr<BoundExpression>> &arguments) {
  this->functionalSymbol = functionalSymbol;
  this->arguments = arguments;
}

const std::string &BoundCallExpression::getName() const {
  return functionalSymbol.name;
}

Utils::FunctionSymbol BoundCallExpression::getFunctionSymbol() const {
  return functionalSymbol;
}

const std::vector<std::shared_ptr<BoundExpression>> &
BoundCallExpression::getArguments() const {
  return arguments;
}

BinderKindUtils::BoundNodeKind BoundCallExpression::getKind() {
  return BinderKindUtils::BoundNodeKind::CallExpression;
}

const std::type_info &BoundCallExpression::getType() {
  return functionalSymbol.getReturnType();
}