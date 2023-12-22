
#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "../bind/Binder/Binder.h"
#include "../bind/BoundIndexExpression/BoundIndexExpression.h"
#include "../syntax/SyntaxKindUtils.h"
#include "../syntax/expression/BinaryExpressionSyntax.h"
#include "../syntax/expression/ExpressionSyntax.h"
#include "../syntax/expression/LiteralExpressionSyntax.h"
#include "../syntax/expression/ParenthesizedExpressionSyntax.h"
#include "../syntax/expression/UnaryExpressionSyntax.h"
#include "InterpreterUtils/InterpreterConversions/InterpreterConversion.h"
#include "InterpreterUtils/InterpreterUtils.h"

#include "../bind/BoundAssignmentExpression/BoundAssignmentExpression.h"
#include "../bind/BoundBinaryExpression/BoundBinaryExpression.h"
#include "../bind/BoundExpression.h"
#include "../bind/BoundLiteralExpression/BoundLiteralExpression.h"
#include "../bind/BoundParenthesizedExpression/BoundParenthesizedExpression.h"
#include "../bind/BoundUnaryExpression/BoundUnaryExpression.h"
#include "../diagnostics/Diagnostic/Diagnostic.h"
#include "../diagnostics/DiagnosticHandler/DiagnosticHandler.h"
#include "../diagnostics/DiagnosticUtils/DiagnosticUtils.h"
#include "../utils/Utils.h"
#include <any>
#include <map>
#include <string>

class Interpreter {
public:
  Interpreter(BoundScopeGlobal *globalScope,
              DiagnosticHandler *diagnosticHandler);

  void execute(BoundBlockStatement *node);
  std::string getResult();

private:
  std::any last_value = nullptr;
  CompilationUnitSyntax *compilation_unit = nullptr;
  Interpreter *previous = nullptr;
  std::stack<std::map<std::string, Utils::Variable>> variable_stack;
  std::stack<std::map<std::string, BoundFunctionDeclaration *>> function_stack;
  std::stack<std::pair<Utils::type, int>> return_type_stack;
  BoundScopeGlobal *_globalScope = nullptr;
  std::unique_ptr<InterpreterUtils> _interpreterUtils;
  int break_count = 0, continue_count = 0, showResult = 1;

  void evaluateExpressionStatement(BoundExpressionStatement *node);
  void evaluateBlockStatement(BoundBlockStatement *node);
  void evaluateVariableDeclaration(BoundVariableDeclaration *node);
  void evaluateIfStatement(BoundIfStatement *node);
  void evaluateWhileStatement(BoundWhileStatement *node);
  void evaluateForStatement(BoundForStatement *node);
  void assignVariable(std::string name, Utils::Variable variable);
  void evaluateStatement(BoundStatement *node);
  void declareVariable(std::string name, Utils::Variable variable);
  void defineFunction(std::string name,
                      BoundFunctionDeclaration *functionDeclaration);

  BoundFunctionDeclaration *getFunction(std::string name);
  Utils::Variable getVariable(std::string name);
  template <typename T> T evaluateLiteralExpression(BoundExpression *node);
  template <typename T> T evaluateUnaryExpression(BoundExpression *node);
  template <typename T> T evaluateBinaryExpression(BoundExpression *node);
  template <typename T> T evaluateAssignmentExpression(BoundExpression *node);
  template <typename T> T evaluateVariableExpression(BoundExpression *node);
  template <typename T> T evaluateIndexExpression(BoundExpression *node);
  template <typename T> T evaluate(BoundExpression *node);

  std::any handleBuiltInFunction(BoundCallExpression *node);

  std::any unaryExpressionEvaluator(BoundUnaryExpression *unaryExpression,
                                    std::any operand);

  template <typename T>
  T binaryExpressionEvaluator(BoundBinaryExpression *binaryExpression, T left,
                              T right);
};

#endif // INTERPRETER_H