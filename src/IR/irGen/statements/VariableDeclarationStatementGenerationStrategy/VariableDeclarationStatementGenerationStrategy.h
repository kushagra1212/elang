#ifndef __FLOWWING_VARIABLE_DECLARATION_STATEMENT_STRATEGY_H__
#define __FLOWWING_VARIABLE_DECLARATION_STATEMENT_STRATEGY_H__

#include "../../../../bind/BoundVariableDeclaration/BoundVariableDeclaration.h"
#include "../StatementGenerationStrategy/StatementGenerationStrategy.h"

class VariableDeclarationStatementGenerationStrategy
    : public StatementGenerationStrategy {
public:
  VariableDeclarationStatementGenerationStrategy(
      CodeGenerationContext *context);

  llvm::Value *generateStatement(BoundStatement *statement) override;
  llvm::Value *generateGlobalStatement(BoundStatement *statement) override;

  // Specialized for BoundVariableDeclaration

  bool canGenerateStatement(BoundStatement *statement);

  // Local Primitive Variable Declaration

  llvm::Value *
  handleTypedPrimitiveLocalVariableDeclr(const std::string &variableName,
                                         const Utils::type &variableType,
                                         llvm::Value *rhsValue);

  llvm::Value *
  handleUnTypedPrimitiveLocalVariableDeclr(const std::string &variableName,
                                           llvm::Value *rhsValue);

  llvm::Value *
  handlePrimitiveLocalVariableDeclr(const std::string &variableName,
                                    const Utils::type &variableType,
                                    llvm::Value *rhsValue);

private:
  std::string _variableName;
  llvm::Value *_rhsValue;
  Utils::type _variableType;
};

#endif // __FLOWWING_VARIABLE_DECLARATION_STATEMENT_STRATEGY_H__
