#ifndef __FLOWWING_EXPRESSION_GENERATION_STRATEGY_H__
#define __FLOWWING_EXPRESSION_GENERATION_STRATEGY_H__
#include "../../GenerationStrategy.h"
class ExpressionGenerationStrategy : public GenerationStrategy {
public:
  CodeGenerationContext *_codeGenerationContext;

  ExpressionGenerationStrategy(CodeGenerationContext *context)
      : GenerationStrategy(context) {}

  virtual llvm::Value *generateExpression(BoundExpression *expression) = 0;
  virtual llvm::Value *
  generateGlobalExpression(BoundExpression *expression) = 0;
};

#endif // __FLOWWING_EXPRESSION_GENERATION_STRATEGY_H__
