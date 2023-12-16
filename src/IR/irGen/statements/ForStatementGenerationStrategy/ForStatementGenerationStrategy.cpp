#include "ForStatementGenerationStrategy.h"
#include "../../expressions/ExpressionGenerationStrategy/ExpressionGenerationStrategy.h"

ForStatementGenerationStrategy::ForStatementGenerationStrategy(
    CodeGenerationContext *context)
    : StatementGenerationStrategy(context) {}

llvm::Value *
ForStatementGenerationStrategy::generateStatement(BoundStatement *statement) {
  BoundForStatement *forStatement = static_cast<BoundForStatement *>(statement);

  _codeGenerationContext->getLogger()->setCurrentSourceLocation(
      forStatement->getLocation());

  _codeGenerationContext->getNamedValueChain()->addHandler(
      new NamedValueTable());

  _codeGenerationContext->getAllocaChain()->addHandler(new AllocaTable());

  std::string variableName = "";

  // Step Value

  llvm::Value *stepValue =
      llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 1, true)); // default

  BoundExpression *forStepExp = forStatement->getStepExpressionPtr().get();
  if (forStepExp) {

    stepValue = _int32TypeConverter->convertImplicit(
        _expressionGenerationFactory->createStrategy(forStepExp->getKind())
            ->generateExpression(forStepExp));
  }

  // Upper Bound
  BoundExpression *upperBoundExp = forStatement->getUpperBoundPtr().get();

  llvm::Value *upperBound =
      _expressionGenerationFactory->createStrategy(upperBoundExp->getKind())
          ->generateExpression(upperBoundExp);

  // Declare Loop Variable

  if (forStatement->getInitializationPtr()->getKind() ==
      BinderKindUtils::BoundNodeKind::VariableDeclaration) {
    BoundVariableDeclaration *variableDeclaration =
        (BoundVariableDeclaration *)forStatement->getInitializationPtr().get();

    _codeGenerationContext->getLogger()->setCurrentSourceLocation(
        variableDeclaration->getLocation());

    variableName = variableDeclaration->getVariable();

    _statementGenerationFactory->createStrategy(variableDeclaration->getKind())
        ->generateStatement(variableDeclaration);

  } else {

    // Loop Variable

    variableName = "loopVariable";

    llvm::AllocaInst *variable = Builder->CreateAlloca(
        llvm::Type::getInt32Ty(*TheContext), nullptr, variableName.c_str());

    _codeGenerationContext->getAllocaChain()->setAllocaInst(variableName,
                                                            variable);
    BoundStatement *initializationStat =
        forStatement->getInitializationPtr().get();

    llvm::Value *result = _statementGenerationFactory
                              ->createStrategy(initializationStat->getKind())
                              ->generateStatement(initializationStat);

    Builder->CreateStore(result, variable);

    _codeGenerationContext->getNamedValueChain()->setNamedValue(variableName,
                                                                result);
  }

  _codeGenerationContext->getLogger()->setCurrentSourceLocation(
      forStatement->getLocation());

  if (variableName == "") {

    _codeGenerationContext->getLogger()->LogError(
        "Variable name is not found in for statement");

    return nullptr;
  }

  llvm::BasicBlock *currentBlock = Builder->GetInsertBlock();
  llvm::Function *function = currentBlock->getParent();
  llvm::Value *exitValue = nullptr;
  llvm::BasicBlock *loopCondition =
      llvm::BasicBlock::Create(*TheContext, "loopCondition", function);
  llvm::BasicBlock *loopBody =
      llvm::BasicBlock::Create(*TheContext, "loopBody", function);
  llvm::BasicBlock *afterLoop =
      llvm::BasicBlock::Create(*TheContext, "afterLoop", function);

  llvm::BasicBlock *breakLoop =
      llvm::BasicBlock::Create(*TheContext, "breakLoop", function);

  Builder->CreateBr(loopCondition);

  // Loop Condition

  Builder->SetInsertPoint(loopCondition);

  _codeGenerationContext->decrementCountIfNotZero(
      _codeGenerationContext->getPrefixedName(FLOWWING_CONTINUE_COUNT));

  llvm::Value *variableValue =
      _codeGenerationContext->getNamedValueChain()->getNamedValue(variableName);

  llvm::AllocaInst *v =
      _codeGenerationContext->getAllocaChain()->getAllocaInst(variableName);

  llvm::Value *value = Builder->CreateLoad(variableValue->getType(), v);

  llvm::PHINode *conditionPHI =
      generateLoopCondition(stepValue, value, upperBound);

  if (conditionPHI == nullptr) {

    _codeGenerationContext->getLogger()->LogError(
        "Condition value is not found in for statement");

    return nullptr;
  }

  Builder->CreateCondBr(conditionPHI, breakLoop, afterLoop);

  Builder->SetInsertPoint(breakLoop);

  Builder->CreateCondBr(
      _codeGenerationContext->isCountZero(
          _codeGenerationContext->getPrefixedName(FLOWWING_BREAK_COUNT),
          llvm::Type::getInt32Ty(*TheContext)),
      loopBody, afterLoop);

  // Loop Body

  Builder->SetInsertPoint(loopBody);

  BoundStatement *bodyStat = forStatement->getStatementPtr().get();

  llvm::Value *result =
      _statementGenerationFactory->createStrategy(bodyStat->getKind())
          ->generateStatement(bodyStat);

  // Value incremented by Step

  llvm::Value *incrementedValue = Builder->CreateAdd(value, stepValue);

  _codeGenerationContext->getNamedValueChain()->updateNamedValue(
      variableName, incrementedValue);

  Builder->CreateStore(incrementedValue, v);

  Builder->CreateBr(loopCondition);

  Builder->SetInsertPoint(afterLoop);

  _codeGenerationContext->decrementCountIfNotZero(
      _codeGenerationContext->getPrefixedName(FLOWWING_BREAK_COUNT));

  _codeGenerationContext->getAllocaChain()->removeHandler();

  _codeGenerationContext->getNamedValueChain()->removeHandler();
  return exitValue;
}

llvm::Value *ForStatementGenerationStrategy::generateGlobalStatement(
    BoundStatement *statement) {
  return this->generateStatement(statement);
}

llvm::PHINode *ForStatementGenerationStrategy::generateLoopCondition(
    llvm::Value *stepValue, llvm::Value *value, llvm::Value *upperBound) {
  llvm::Value *isStepNegative =
      Builder->CreateICmpSLT(stepValue, Builder->getInt32(0));

  llvm::BasicBlock *trueBlock = llvm::BasicBlock::Create(
      value->getContext(), "trueBlock", Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *falseBlock =
      llvm::BasicBlock::Create(value->getContext(), "falseBlock",
                               Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *mergeBlock =
      llvm::BasicBlock::Create(value->getContext(), "mergeBlock",
                               Builder->GetInsertBlock()->getParent());

  Builder->CreateCondBr(isStepNegative, trueBlock, falseBlock);

  Builder->SetInsertPoint(trueBlock);

  llvm::Value *oppositeCondition = Builder->CreateICmpSGE(value, upperBound);

  Builder->CreateBr(mergeBlock);

  Builder->SetInsertPoint(falseBlock);

  llvm::Value *normalCondition = Builder->CreateICmpSLE(value, upperBound);

  Builder->CreateBr(mergeBlock);

  Builder->SetInsertPoint(mergeBlock);

  llvm::PHINode *conditionPHI = Builder->CreatePHI(Builder->getInt1Ty(), 2);
  conditionPHI->addIncoming(oppositeCondition, trueBlock);
  conditionPHI->addIncoming(normalCondition, falseBlock);

  return conditionPHI;
}
