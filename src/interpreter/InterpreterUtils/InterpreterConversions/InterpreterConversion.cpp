#include "InterpreterConversion.h"

std::string
InterpreterConversion::explicitConvertAnyToString(const std::any value) {
  if (value.type() == typeid(int)) {
    return explicitConvertIntToString(std::any_cast<int>(value));
  } else if (value.type() == typeid(double)) {
    return explicitConvertDoubleToString(std::any_cast<double>(value));
  } else if (value.type() == typeid(bool)) {
    return explicitConvertBoolToString(std::any_cast<bool>(value));
  } else if (value.type() == typeid(std::string)) {
    return std::any_cast<std::string>(value);
  } else {
    return "";
  }
}

std::string InterpreterConversion::explicitConvertIntToString(const int value) {
  return std::to_string(value);
}
std::string
InterpreterConversion::explicitConvertBoolToString(const bool value) {
  return value ? "true" : "false";
}
std::string
InterpreterConversion::explicitConvertDoubleToString(const double value) {
  return std::to_string(value);
}

double
InterpreterConversion::explicitConvertToAnyToDouble(const std::any value) {
  if (value.type() == typeid(int)) {
    return implicitConvertIntToDouble(std::any_cast<int>(value));
  } else if (value.type() == typeid(double)) {
    return (double)(std::any_cast<double>(value));
  } else if (value.type() == typeid(bool)) {
    return implicitConvertBoolToDouble(std::any_cast<bool>(value));
  } else if (value.type() == typeid(std::string)) {
    return explicitConvertStringToDouble(std::any_cast<std::string>(value));
  } else {
    return 0.0;
  }
}

double InterpreterConversion::implicitConvertIntToDouble(const int value) {
  return (double)value;
}

double InterpreterConversion::implicitConvertBoolToDouble(const bool value) {
  return value ? 1.0 : 0.0;
}

double
InterpreterConversion::explicitConvertStringToDouble(const std::string value) {
  if (Utils::isDouble(value)) {
    return std::stod(value);
  }

  throw std::runtime_error("Cannot convert string to double"
                           " because string is not a double"
                           " value"
                           " or is not a valid double value"
                           " in the first place");
}

int InterpreterConversion::explicitConvertAnyToInt(const std::any value) {
  if (value.type() == typeid(int)) {
    return (int)std::any_cast<int>(value);
  } else if (value.type() == typeid(double)) {
    return explicitConvertDoubleToInt(std::any_cast<double>(value));
  } else if (value.type() == typeid(bool)) {
    return explicitConvertBoolToInt(std::any_cast<bool>(value));
  } else if (value.type() == typeid(std::string)) {
    return explicitConvertStringToInt(std::any_cast<std::string>(value));
  } else {
    return 0;
  }
}

int InterpreterConversion::explicitConvertDoubleToInt(const double value) {
  return (int)value;
}

int InterpreterConversion::explicitConvertBoolToInt(const bool value) {
  return value ? 1 : 0;
}

int InterpreterConversion::explicitConvertStringToInt(const std::string value) {
  if (Utils::isInteger(value)) {
    return std::stoi(value);
  }

  throw std::runtime_error("Cannot convert string to int"
                           " because string is not an int"
                           " value"
                           " or is not a valid int value"
                           " in the first place");
}

bool InterpreterConversion::explicitConvertAnyToBool(const std::any value) {
  if (value.type() == typeid(int)) {
    return explicitConvertIntToBool(std::any_cast<int>(value));
  } else if (value.type() == typeid(double)) {
    return explicitConvertDoubleToBool(std::any_cast<double>(value));
  } else if (value.type() == typeid(bool)) {
    return std::any_cast<bool>(value);
  } else if (value.type() == typeid(std::string)) {
    return explicitConvertStringToBool(std::any_cast<std::string>(value));
  } else {
    return false;
  }
}

bool InterpreterConversion::explicitConvertIntToBool(const int value) {
  return (bool)(value);
}

bool InterpreterConversion::explicitConvertDoubleToBool(const double value) {
  return (bool)(value);
}

bool InterpreterConversion::explicitConvertStringToBool(
    const std::string value) {
  return (bool)(value.length() > 0);
}