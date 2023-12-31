#include "FunctionDeclarationGenerationStrategy.h"
#include "../../expressions/LiteralExpressionGenerationStrategy/LiteralExpressionGenerationStrategy.h"

FunctionDeclarationGenerationStrategy::FunctionDeclarationGenerationStrategy(
    CodeGenerationContext *context)
    : StatementGenerationStrategy(context) {}

llvm::Value *FunctionDeclarationGenerationStrategy::generateStatement(
    BoundStatement *statement) {
  BoundFunctionDeclaration *node =
      static_cast<BoundFunctionDeclaration *>(statement);

  _codeGenerationContext->getLogger()->setCurrentSourceLocation(
      node->getLocation());

  _codeGenerationContext->getLogger()->LogError(
      "Function Declaration is not allowed in global scope ");

  return nullptr;
}

llvm::Value *FunctionDeclarationGenerationStrategy::generateGlobalStatement(
    BoundStatement *statement) {
  BoundFunctionDeclaration *fd =
      static_cast<BoundFunctionDeclaration *>(statement);

  _codeGenerationContext->getLogger()->setCurrentSourceLocation(
      fd->getLocation());
  std::unique_ptr<LiteralExpressionGenerationStrategy>
      literalExpressionGenerationStrategy =
          std::make_unique<LiteralExpressionGenerationStrategy>(
              _codeGenerationContext);
  std::vector<llvm::Type *> argTypes;
  for (int i = 0; i < fd->getParametersRef().size(); i++) {

    llvm::Type *parmType = nullptr;

    if (fd->getParametersRef()[i]->getVariableTypeRef() ==
        Utils::type::UNKNOWN) {

      parmType = (_codeGenerationContext->getDynamicType()->get());
      argTypes.push_back(parmType);

      _codeGenerationContext->getArgsTypeHandler()->addArgsType(
          fd->getFunctionNameRef(), std::make_unique<LLVMType>(parmType));

    } else if (Utils::isContainerType(
                   fd->getParametersRef()[i]->getVariableTypeRef())) {

      Utils::type containerElementType = Utils::toContainerElementType(
          fd->getParametersRef()[i]->getVariableTypeRef());

      BoundArrayVariableExpression *boundArrayVariable =
          static_cast<BoundArrayVariableExpression *>(
              fd->getParametersRef()[i].get());

      _codeGenerationContext->getLogger()->setCurrentSourceLocation(
          boundArrayVariable->getLocation());

      size_t multiDimSize = boundArrayVariable->getSizeExpressionsRef().size();

      parmType = _codeGenerationContext->getMapper()->mapCustomTypeToLLVMType(
          containerElementType);

      std::vector<uint64_t> dimensions(multiDimSize, 0);
      for (int64_t k = multiDimSize - 1; k >= 0; k--) {

        llvm::Value *arraySize =
            literalExpressionGenerationStrategy->generateExpression(
                boundArrayVariable->getSizeExpressionsRef()[k].get());

        if (!llvm::isa<llvm::ConstantInt>(arraySize)) {
          _codeGenerationContext->getLogger()->LogError(
              "Array size must be a constant integer");
        }

        dimensions[k] =
            llvm::cast<llvm::ConstantInt>(arraySize)->getSExtValue();

        parmType = llvm::ArrayType::get(parmType, dimensions[k]);
      }
      llvm::Type *parmTypePointer = llvm::PointerType::get(parmType, 0);

      argTypes.push_back(parmTypePointer);
      _codeGenerationContext->getArgsTypeHandler()->addArgsType(
          fd->getFunctionNameRef(),
          std::make_unique<LLVMArrayType>(
              parmTypePointer, parmType,
              _codeGenerationContext->getMapper()->mapCustomTypeToLLVMType(
                  containerElementType),
              dimensions));

    } else {

      parmType = _codeGenerationContext->getMapper()->mapCustomTypeToLLVMType(
          fd->getParametersRef()[i]->getVariableTypeRef());
      argTypes.push_back(parmType);
      _codeGenerationContext->getArgsTypeHandler()->addArgsType(
          fd->getFunctionNameRef(), std::make_unique<LLVMType>(parmType));
    }
  }
  llvm::FunctionType *FT = nullptr;
  llvm::Function *F = nullptr;
  std::string returnInfo = "";
  {
    llvm::Type *returnType = nullptr;
    BoundTypeExpression *bTE =
        static_cast<BoundTypeExpression *>(fd->getReturnType().get());

    switch (bTE->getKind()) {

    case BinderKindUtils::BoundTypeExpression: {
      llvm::Type *elementType =
          _codeGenerationContext->getMapper()->mapCustomTypeToLLVMType(
              bTE->getUtilsType());
      returnType = elementType;
      FT = llvm::FunctionType::get(returnType, argTypes, false);
      F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                 fd->getFunctionNameRef(), *TheModule);

      _codeGenerationContext->getReturnTypeHandler()->addReturnType(
          fd->getFunctionNameRef(), std::make_unique<LLVMType>(returnType));
      returnInfo =
          fd->getFunctionNameRef() + ":rt:pr:" +
          std::to_string(
              _codeGenerationContext->getMapper()->mapLLVMTypeToCustomType(
                  elementType));
      break;
    }
    case BinderKindUtils::BoundArrayTypeExpression: {

      llvm::Type *elementType =
          _codeGenerationContext->getMapper()->mapCustomTypeToLLVMType(
              bTE->getUtilsType());
      BoundArrayTypeExpression *boundArrayTypeExpression =
          static_cast<BoundArrayTypeExpression *>(bTE);

      llvm::Type *arrayType = elementType;
      llvm::Constant *_defaultVal = llvm::cast<llvm::Constant>(
          _codeGenerationContext->getMapper()->getDefaultValue(elementType));
      std::vector<uint64_t> returnDimentions(
          boundArrayTypeExpression->getDimensions().size(), 0);
      for (int64_t k = boundArrayTypeExpression->getDimensions().size() - 1;
           k >= 0; k--) {

        llvm::Value *arraySize =
            literalExpressionGenerationStrategy->generateExpression(
                boundArrayTypeExpression->getDimensions()[k].get());

        if (!llvm::isa<llvm::ConstantInt>(arraySize)) {
          _codeGenerationContext->getLogger()->LogError(
              "Array size must be a constant integer");
        }

        returnDimentions[k] =
            llvm::cast<llvm::ConstantInt>(arraySize)->getSExtValue();

        arrayType = llvm::ArrayType::get(arrayType, returnDimentions[k]);
      }
      llvm::Type *returnType = llvm::PointerType::get(arrayType, 0);

      FT = llvm::FunctionType::get(returnType, argTypes, false);
      F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                                 fd->getFunctionNameRef(), *TheModule);

      _codeGenerationContext->getReturnTypeHandler()->addReturnType(
          fd->getFunctionNameRef(),
          std::make_unique<LLVMArrayType>(returnType, arrayType, elementType,
                                          returnDimentions));

      returnInfo =
          fd->getFunctionNameRef() + ":rt:ay:" +
          std::to_string(
              _codeGenerationContext->getMapper()->mapLLVMTypeToCustomType(
                  elementType)) +
          ":sz:";

      for (int64_t k = 0; k < returnDimentions.size(); k++) {
        returnInfo += std::to_string(returnDimentions[k]) + ":";
      }
      break;
    }
    default: {
      _codeGenerationContext->getLogger()->LogError(
          "Invalid return type for function");
      return nullptr;
    }
    }
  }
  // Return type metadata
  F->setMetadata(
      "rt", llvm::MDNode::get(*TheContext,
                              llvm::MDString::get(*TheContext, returnInfo)));

  const std::vector<std::unique_ptr<LLVMType>> &llvmArrayArgs =
      _codeGenerationContext->getArgsTypeHandler()->getArgsType(
          fd->getFunctionNameRef());

  for (unsigned i = 0; i < F->arg_size(); ++i) {
    std::string argInfo = "";
    llvm::Argument *arg = F->getArg(i);

    if (llvmArrayArgs[i]->isPointerToArray()) {
      LLVMArrayType *llvmArrayType =
          static_cast<LLVMArrayType *>(llvmArrayArgs[i].get());
      llvm::ArrayType *arrayType =
          static_cast<llvm::ArrayType *>(llvmArrayType->getElementType());

      arg->setName("arg" + std::to_string(i) + "_ptr");

      uint64_t arraySize = arrayType->getNumElements();

      // Create a metadata string that describes the argument

      argInfo = fd->getParametersRef()[i]->getVariableNameRef() + ":" +
                std::to_string(i) + ":ay:" +
                _codeGenerationContext->getMapper()->getLLVMTypeName(
                    llvmArrayType->getArrayElementType()) +
                ":sz:";
      for (int64_t k = 0; k < llvmArrayType->getDimensions().size(); k++) {
        argInfo += std::to_string(llvmArrayType->getDimensions()[k]) + ":";
      }
    } else {

      arg->setName("arg" + std::to_string(i));

      // Create a metadata string that describes the argument
      argInfo = fd->getParametersRef()[i]->getVariableNameRef() + ":" +
                std::to_string(i) + ":" +
                _codeGenerationContext->getMapper()->getLLVMTypeName(
                    llvmArrayArgs[i]->getType());
    }

    llvm::MDString *argInfoMD = llvm::MDString::get(*TheContext, argInfo);
    llvm::StringRef Kind = "argInfo" + std::to_string(i);
    // Attach the metadata to the argument
    F->setMetadata(Kind, llvm::MDNode::get(*TheContext, argInfoMD));
  }

  _codeGenerationContext->addBoundedUserFunction(fd->getFunctionNameRef(), fd);

  return nullptr;
}