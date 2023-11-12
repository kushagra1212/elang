#ifndef __FLOWWING_FUNCTION_DECLARATION_STRATEGY_H__
#define __FLOWWING_FUNCTION_DECLARATION_STRATEGY_H__

#include "../../../../bind/BoundFunctionDeclaration/BoundFunctionDeclaration.h"
#include "../StatementGenerationStrategy/StatementGenerationStrategy.h"

class FunctionDeclarationGenerationStrategy
    : public StatementGenerationStrategy {
public:
  FunctionDeclarationGenerationStrategy(CodeGenerationContext *context);
  llvm::Value *generateStatement(BoundStatement *statement) override;
  llvm::Value *generateGlobalStatement(BoundStatement *statement) override;
};

#endif // __FLOWWING_FUNCTION_DECLARATION_STRATEGY_H__
