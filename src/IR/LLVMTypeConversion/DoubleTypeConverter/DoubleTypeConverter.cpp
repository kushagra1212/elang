#include "DoubleTypeConverter.h"

DoubleTypeConverter::DoubleTypeConverter(CodeGenerationContext *context)
    : TypeConverterBase(context){};

llvm::Value *DoubleTypeConverter::convertExplicit(llvm::Value *value) {

  llvm::Value *res = nullptr;

  Utils::type type = this->_mapper->mapLLVMTypeToCustomType(value->getType());

  switch (type) {

  case Utils::type::INT16:
  case Utils::type::INT32:
  case Utils::type::INT64: {
    return _builder->CreateSIToFP(
        value, llvm::Type::getDoubleTy(_builder->getContext()));
  }
  case Utils::type::DECIMAL: {
    return value;
  }
  case Utils::type::BOOL: {
    return _builder->CreateUIToFP(
        value, llvm::Type::getDoubleTy(_builder->getContext()));
  }
  case Utils::type::STRING: {

    return _builder->CreateCall(
        _module->getFunction(INNERS::FUNCTIONS::STRING_TO_DOUBLE), {value});
  }
  default:
    break;
  }

  _logger->logLLVMError(
      llvm::createStringError(llvm::inconvertibleErrorCode(),
                              "Unsupported type for conversion to double"));

  return nullptr;
}

llvm::Value *DoubleTypeConverter::convertImplicit(llvm::Value *value) {
  llvm::Value *res = nullptr;

  Utils::type type = this->_mapper->mapLLVMTypeToCustomType(value->getType());

  switch (type) {

  case Utils::type::INT16:
  case Utils::type::INT32:
  case Utils::type::INT64: {
    return _builder->CreateSIToFP(
        value, llvm::Type::getDoubleTy(_builder->getContext()));
  }
  case Utils::type::DECIMAL: {
    return value;
  }
  case Utils::type::BOOL: {
    _logger->logLLVMError(llvm::createStringError(
        llvm::inconvertibleErrorCode(),
        "Implicit conversion from bool to double is not "
        "supported for variable with predefined type"));
    return nullptr;
  }
  case Utils::type::STRING: {
    _logger->logLLVMError(llvm::createStringError(
        llvm::inconvertibleErrorCode(),
        "Implicit conversion from string to double is not "
        "supported for variable with predefined type"));

    return nullptr;
  }
  default:
    break;
  }

  _logger->logLLVMError(
      llvm::createStringError(llvm::inconvertibleErrorCode(),
                              "Unsupported type for conversion to double"));

  return nullptr;
}