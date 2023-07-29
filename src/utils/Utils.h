#ifndef UTILS_H
#define UTILS_H
#include "../Common.h"
#include "../syntax/SyntaxNode.h"
#include "../syntax/expression/LiteralExpressionSyntax.h"
#include <typeinfo>
namespace Utils {
void prettyPrint(std::shared_ptr<SyntaxNode> node, std::string indent = "",
                 bool isLast = true);
std::string
getLineNumberAndPosition(std::shared_ptr<SyntaxToken<std::any>> token);

std::string convertAnyToString(const std::any value);
enum type { INT32, DOUBLE, STRING, BOOL, VOID, UNKNOWN };

enum class SymbolKind {
  Variable,
  Function,
  Parameter,
  Type,
  Namespace,
  Label,
  None
};

struct Variable {
  std::any value;
  bool isConst;
  SymbolKind kind;
  Variable() {}
  Variable(std::any value, bool isConst) {
    this->value = value;
    this->isConst = isConst;
    this->kind = SymbolKind::Variable;
  }
};

struct FunctionParameterSymbol {
  std::string name;
  bool isConst;
  SymbolKind kind;
  FunctionParameterSymbol() {}
  FunctionParameterSymbol(std::string name, bool isConst) {
    this->name = name;
    this->isConst = isConst;
    this->kind = SymbolKind::Parameter;
  }
};

struct FunctionSymbol {
  std::string name;
  std::vector<FunctionParameterSymbol> parameters;
  type return_type;
  SymbolKind kind;
  FunctionSymbol() { this->kind = SymbolKind::None; }
  FunctionSymbol(std::string name,
                 std::vector<FunctionParameterSymbol> parameters,
                 type return_type) {
    this->name = name;
    this->parameters = parameters;
    this->kind = SymbolKind::Function;
    this->return_type = return_type;
  }

  const std::type_info &getReturnType() {
    if (return_type == type::INT32) {
      return typeid(int);
    } else if (return_type == type::DOUBLE) {
      return typeid(double);
    } else if (return_type == type::STRING) {
      return typeid(std::string);
    } else if (return_type == type::BOOL) {
      return typeid(bool);
    } else if (return_type == type::VOID) {
      return typeid(void);
    } else {
      return typeid(void);
    }
  }

  int arity() { return parameters.size(); }
};

class BuiltInFunctions {
public:
  static FunctionSymbol print;
  static FunctionSymbol input;
  static FunctionSymbol random;

  // Conversations

  static FunctionSymbol String;
  static FunctionSymbol Int32;
  static FunctionSymbol Double;
  static FunctionSymbol Bool;
  static FunctionSymbol Void;

  static std::vector<FunctionSymbol> getAllFunctions();

  static FunctionSymbol getFunctionSymbol(std::string name);
};

} // namespace Utils

#endif // UTILS_H