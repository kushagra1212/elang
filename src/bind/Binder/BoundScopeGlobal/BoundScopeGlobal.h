#pragma once
#include "../../../Common.h"
#include "../../../diagnostics/DiagnosticHandler/DiagnosticHandler.h"
#include "../../../utils/Utils.h"
#include "../../BoundExpression.h"
#include "../../BoundFunctionDeclaration/BoundFunctionDeclaration.h"
#include "../../BoundStatement/BoundStatement.h"
class BoundScopeGlobal {
public:
  BoundScopeGlobal(std::unique_ptr<BoundScopeGlobal> previous,
                   std::map<std::string, Utils::Variable> variables,
                   std::map<std::string, BoundFunctionDeclaration *> functions,
                   DiagnosticHandler *diagnosticHandler,
                   std::unique_ptr<BoundBlockStatement> statement);

  bool tryLookupVariable(std::string name);
  bool tryAssignVariable(std::string name, const struct Utils::Variable &value);
  Utils::Variable getVariable(std::string name);

public:
  std::unique_ptr<BoundScopeGlobal> previous;
  std::map<std::string, Utils::Variable> variables;
  std::map<std::string, BoundFunctionDeclaration *> functions;
  DiagnosticHandler *_diagnosticHandler;
  std::unique_ptr<BoundBlockStatement> globalStatement;
};