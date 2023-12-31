#ifndef __FLOWWING_CONTAINER_STATEMENT_STRATEGY_H__
#define __FLOWWING_CONTAINER_STATEMENT_STRATEGY_H__

#include "../../../../bind/BoundBracketedExpression/BoundBracketedExpression.h"
#include "../../../../bind/BoundContainerStatement/BoundContainerStatement.h"
#include "../StatementGenerationStrategy/StatementGenerationStrategy.h"

class ContainerStatementGenerationStrategy
    : public StatementGenerationStrategy {

  void calcActualContainerSize(BoundContainerStatement *containerStatement);

public:
  ContainerStatementGenerationStrategy(CodeGenerationContext *context);

  llvm::Value *generateStatement(BoundStatement *statement) override;
  llvm::Value *generateGlobalStatement(BoundStatement *statement) override;

  llvm::Value *generateBracketGlobalExpression(
      BoundBracketedExpression *bracketedExpression);

  llvm::Value *
  generateBracketLocalExpression(BoundBracketedExpression *bracketedExpression);

  const bool canGenerateCallExpression(BoundExpression *callExp);

private:
  std::vector<uint64_t> _actualSizes;
  llvm::Type *_elementType;
  std::string _containerName;
  llvm::LoadInst *_loadedValue;
};

#endif //__FLOWWING_CONTAINER_STATEMENT_STRATEGY_H__
