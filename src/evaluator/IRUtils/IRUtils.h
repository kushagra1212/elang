#ifndef IR_UTILS_H
#define IR_UTILS_H

// FLOWWING Files Import

#include "../../Common.h"
#include "../../IR/constants/FlowWingIRConstants.h"
#include "../../IR/context/CodeGenerationContext.h"
#include "../../IR/logger/LLVMLogger.h"
#include "../../bind/Binder/Binder.h"
#include "../../bind/Binder/BoundScopeGlobal/BoundScopeGlobal.h"
#include "../../bind/BinderKindUtils.h"
#include "../../bind/BoundAssignmentExpression/BoundAssignmentExpression.h"
#include "../../bind/BoundBinaryExpression/BoundBinaryExpression.h"
#include "../../bind/BoundBlockStatement/BoundBlockStatement.h"
#include "../../bind/BoundBringStatement/BoundBringStatement.h"
#include "../../bind/BoundExpression.h"
#include "../../bind/BoundExpressionStatement/BoundExpressionStatement.h"
#include "../../bind/BoundLiteralExpression/BoundLiteralExpression.h"
#include "../../bind/BoundParenthesizedExpression/BoundParenthesizedExpression.h"
#include "../../bind/BoundStatement/BoundStatement.h"
#include "../../bind/BoundUnaryExpression/BoundUnaryExpression.h"
#include "../../bind/BoundVariableExpression/BoundVariableExpression.h"
#include "../../syntax/CompilationUnitSyntax.h"
#include "../IRParser/IRParser.h"

using namespace FLOWWING::IR::CONSTANTS;

class IRUtils;
// LLVM Imports
#include "llvm/IR/Type.h"
#include "llvm/Pass.h"
#include <llvm/ADT/APInt.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/Support/Error.h>
#include <llvm/Support/SourceMgr.h>

#include <llvm/IRReader/IRReader.h>
// ExecutionEngine
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/SourceMgr.h>

#include "llvm//IR/Value.h"
#include "llvm/IR/Constants.h"

#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include <llvm/Linker/Linker.h>
#include <llvm/Support/FileSystem.h>
// JIT
#include <llvm/ExecutionEngine/Orc/LLJIT.h>

#include "../llvm/IRGenerator.h"

#include "../../utils/Utils.h"
#include <llvm/Support/raw_ostream.h>
#include <sstream>

class IRUtils {

public:
  IRUtils(CodeGenerationContext *codeGenerationContext);

  llvm::Value *getLLVMValue(std::any value, SyntaxKindUtils::SyntaxKind kind);

  llvm::Value *isCountZero(const std::string name, llvm::Type *ty);
  llvm::Value *getGlobalVarAndLoad(const std::string name, llvm::Type *Ty);

  void decrementCountIfNotZero(const std::string name);

  void incrementCount(const std::string name);

  std::string getString(BoundExpression *node);

  std::string valueToString(llvm::Value *val);

  llvm::Value *convertToString(llvm::Value *val);
  llvm::Value *concatenateStrings(llvm::Value *lhs, llvm::Value *rhs);
  llvm::Value *convertStringToi8Ptr(std::string stringValue);
  llvm::Type *getTypeFromAny(std::any value);

  bool isStringType(llvm::Type *type);

  llvm::Constant *createConstantFromValue(llvm::Value *myValue);
  Utils::type getReturnType(llvm::Type *type);

  bool isIntType(llvm::Type *type);

  bool isBoolType(llvm::Type *type);

  bool isDoubleType(llvm::Type *type);

  llvm::Value *getDefaultValue(Utils::type type);

  llvm::Value *explicitConvertToString(llvm::Value *val);

  llvm::Value *loadGlobalValue(llvm::Value *val);

  llvm::Type *getReturnType(Utils::type type);

  void errorGuard(std::function<void()> code);

  llvm::ConstantInt *getConstantIntFromValue(llvm::Value *value);
  llvm::ConstantFP *getConstantFPFromValue(llvm::Value *value);
  llvm::StringRef getConstantStringFromValue(llvm::Value *value);

  llvm::Value *createStringComparison(llvm::Value *lhsValue,
                                      llvm::Value *rhsValue,
                                      std::string functionName,
                                      std::string operand = "");

  llvm::Value *checkBitSet(llvm::Value *result, unsigned int bitPosition);

  llvm::PHINode *handleForLoopCondition(llvm::Value *stepValue,
                                        llvm::Value *value,
                                        llvm::Value *upperBound);
  bool saveLLVMModuleToFile(llvm::Module *module, const std::string &path);
  void handleConditionalBranch(
      llvm::Value *conditionValue, const std::string &trueBlockName,
      const std::string &falseBlockName,
      std::function<void(llvm::BasicBlock *, llvm::IRBuilder<> *Builder,
                         llvm::LLVMContext *TheContext, IRUtils *irutils)>
          trueBlockCode,
      std::function<void(llvm::BasicBlock *, llvm::IRBuilder<> *Builder,
                         llvm::LLVMContext *TheContext)>
          falseBlockCode);

  void setCurrentSourceLocation(DiagnosticUtils::SourceLocation sourceLocation);
  DiagnosticUtils::SourceLocation getCurrentSourceLocation();

  void logError(std::string errorMessgae);
  llvm::Constant *getNull();

  const int isInitializingGlobals() const;

  void setInitializingGlobals(int value);

  const int hasError() const;

  const int getIndexofMemberType(llvm::Type *type);

  const std::vector<llvm::Type *> getMemberTypesForDynamicTypes() const;

private:
  DiagnosticUtils::SourceLocation _currentSourceLocation;
  int _initializingGlobals = 0;
  int _hasError = 0;
  std::vector<llvm::Type *> _memberTypesForDynamicTypes;
  LLVMLogger *_llvmLogger;
  llvm::Module *_TheModule;
  llvm::IRBuilder<> *_Builder;
  llvm::LLVMContext *_TheContext;
  DiagnosticHandler *_diagnosticHandler;
  CodeGenerationContext *_codeGenerationContext;
};

#endif