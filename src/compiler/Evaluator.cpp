#include "Evaluator.h"

template class BoundLiteralExpression<std::any>;
template class BoundLiteralExpression<int>;
template class BoundLiteralExpression<double>;
template class BoundLiteralExpression<bool>;
template class BoundLiteralExpression<std::string>;
template class BoundLiteralExpression<char>;

Evaluator::Evaluator(std::shared_ptr<Evaluator> previous,
                     std::shared_ptr<CompilationUnitSyntax> compilation_unit) {
  this->compilation_unit = compilation_unit;

  if (previous != nullptr)
    this->previous = (previous);
}
std::shared_ptr<BoundScopeGlobal> Evaluator::getRoot() {
  if (root == nullptr) {
    if (previous != nullptr)
      root = Binder::bindGlobalScope(previous->root, compilation_unit);
    else
      root = Binder::bindGlobalScope(nullptr, compilation_unit);
  }
  return root;
}

void Evaluator::evaluateStatement(std::shared_ptr<BoundStatement> node) {
  switch (node->getKind()) {
  case BinderKindUtils::BoundNodeKind::ExpressionStatement: {
    std::shared_ptr<BoundExpressionStatement> expressionStatement =
        std::static_pointer_cast<BoundExpressionStatement>(node);
    last_value = this->evaluate<std::any>(expressionStatement->getExpression());
    break;
  }
  case BinderKindUtils::BoundNodeKind::BlockStatement: {

    std::shared_ptr<BoundBlockStatement> blockStatement =
        std::static_pointer_cast<BoundBlockStatement>(node);
    std::vector<std::shared_ptr<BoundStatement>> statements =
        blockStatement->getStatements();
    for (std::shared_ptr<BoundStatement> statement : statements) {
      this->evaluateStatement(statement);
    }
    break;
  }
  case BinderKindUtils::BoundNodeKind::VariableDeclaration: {
    std::shared_ptr<BoundVariableDeclaration> variableDeclaration =
        std::static_pointer_cast<BoundVariableDeclaration>(node);
    std::string variable_name = variableDeclaration->getVariable();

    this->root->variables[variable_name] = Utils::Variable(
        this->evaluate<std::any>(variableDeclaration->getInitializer()),
        variableDeclaration->isConst());

    last_value = this->root->variables[variable_name].value;
    break;
  }
  case BinderKindUtils::BoundNodeKind::IfStatement: {
    std::shared_ptr<BoundIfStatement> ifStatement =
        std::static_pointer_cast<BoundIfStatement>(node);
    std::any condition = this->evaluate<std::any>(ifStatement->getCondition());

    if (condition.type() == typeid(bool)) {
      if (std::any_cast<bool>(condition)) {
        this->evaluateStatement(ifStatement->getThenStatement());
      } else if (ifStatement->getElseStatement() != nullptr) {
        this->evaluateStatement(ifStatement->getElseStatement());
      }
    } else if (condition.type() == typeid(int)) {
      if (std::any_cast<int>(condition)) {
        this->evaluateStatement(ifStatement->getThenStatement());
      } else if (ifStatement->getElseStatement() != nullptr) {
        this->evaluateStatement(ifStatement->getElseStatement());
      }
    } else {
      this->root->logs.push_back("Error: Unexpected condition type");
      return;
    }
    break;
  }

  case BinderKindUtils::BoundNodeKind::WhileStatement: {
    std::shared_ptr<BoundWhileStatement> whileStatement =
        std::dynamic_pointer_cast<BoundWhileStatement>(node);
    std::any condition =
        this->evaluate<std::any>(whileStatement->getCondition());

    if (condition.type() == typeid(bool)) {
      while (std::any_cast<bool>(condition)) {
        this->evaluateStatement(whileStatement->getBody());
        condition = this->evaluate<std::any>(whileStatement->getCondition());
      }
    } else if (condition.type() == typeid(int)) {
      while (std::any_cast<int>(condition)) {
        this->evaluateStatement(whileStatement->getBody());
        condition = this->evaluate<std::any>(whileStatement->getCondition());
      }
    } else {
      this->root->logs.push_back("Error: Unexpected condition type");
      return;
    }
    break;
  }

  case BinderKindUtils::BoundNodeKind::ForStatement: {
    std::shared_ptr<BoundForStatement> forStatement =
        std::dynamic_pointer_cast<BoundForStatement>(node);
    std::string variable_name = "";

    std::any lowerBound = 0;
    if (forStatement->getInitialization()->getKind() ==
        BinderKindUtils::BoundNodeKind::VariableDeclaration) {
      std::shared_ptr<BoundVariableDeclaration> variableDeclaration =
          std::dynamic_pointer_cast<BoundVariableDeclaration>(
              forStatement->getInitialization());

      variable_name = variableDeclaration->getVariable();
      this->evaluateStatement(variableDeclaration);

      lowerBound = this->root->variables[variable_name].value;
    } else {
      this->evaluateStatement(forStatement->getInitialization());

      lowerBound = this->last_value;
    }

    std::any upperBound =
        this->evaluate<std::any>(forStatement->getUpperBound());

    if (lowerBound.type() != upperBound.type()) {
      this->root->logs.push_back("Error: Incompatible types");
      return;
    }

    if (lowerBound.type() == typeid(int)) {
      for (int i = std::any_cast<int>(lowerBound);
           i <= std::any_cast<int>(upperBound); i++) {
        if (variable_name != "") {
          this->root->variables[variable_name] = Utils::Variable(
              i, (this->root->variables[variable_name]).isConst);
        }
        this->evaluateStatement(forStatement->getStatement());
      }
    } else if (lowerBound.type() == typeid(double)) {
      for (double i = std::any_cast<double>(lowerBound);
           i <= std::any_cast<double>(upperBound); i++) {
        if (variable_name != "") {
          this->root->variables[variable_name] = Utils::Variable(
              i, (this->root->variables[variable_name]).isConst);
        }
        this->evaluateStatement(forStatement->getStatement());
      }
    } else {
      this->root->logs.push_back("Error: Unexpected condition type");
      return;
    }

    break;
  }
  default: {
    this->root->logs.push_back("Error: Unexpected node" + node->getKind());
  }
  }
}

template <typename T>
T Evaluator::evaluate(std::shared_ptr<BoundExpression> node) {

  switch (node->getKind()) {
  case BinderKindUtils::BoundNodeKind::LiteralExpression: {
    std::any value =
        (std::dynamic_pointer_cast<BoundLiteralExpression<std::any>>(node))
            ->getValue();
    if (value.type() == typeid(int)) {
      return std::any_cast<int>(value);
    } else if (value.type() == typeid(bool)) {
      return std::any_cast<bool>(value);
    } else if (value.type() == typeid(std::string)) {
      return std::any_cast<std::string>(value);
    } else if (value.type() == typeid(double)) {
      return std::any_cast<double>(value);
    } else {
      this->root->logs.push_back("Error: Unexpected literal expression");
    }
  }
  case BinderKindUtils::BoundNodeKind::UnaryExpression: {
    std::shared_ptr<BoundUnaryExpression> unaryExpression =
        std::dynamic_pointer_cast<BoundUnaryExpression>(node);
    std::any operand_any =
        (this->evaluate<std::any>(unaryExpression->getOperand()));
    return Evaluator::unaryExpressionEvaluator<std::any>(
        unaryExpression->getOperator(), operand_any);
  }
  case BinderKindUtils::BoundNodeKind::VariableExpression: {
    std::shared_ptr<BoundVariableExpression> variableExpression =
        std::dynamic_pointer_cast<BoundVariableExpression>(node);

    std::any variable =
        this->evaluate<std::any>(variableExpression->getIdentifierExpression());
    if (variable.type() != typeid(std::string)) {
      this->root->logs.push_back("Error: Unexpected variable name");
      return nullptr;
    }

    std::string variable_name = std::any_cast<std::string>(variable);
    switch (variableExpression->getKind()) {
    case BinderKindUtils::BoundNodeKind::VariableExpression: {
      if (this->root->variables.find(variable_name) !=
          this->root->variables.end()) {
        return this->root->variables[variable_name].value;
      } else {
        this->root->logs.push_back("Error: variable is undefined");
        return nullptr;
      }
    }
    default:
      this->root->logs.push_back("Error: Unexpected Variable found");
      return nullptr;
    }
  }

  case BinderKindUtils::BoundNodeKind::AssignmentExpression: {
    std::shared_ptr<BoundAssignmentExpression> assignmentExpression =
        std::dynamic_pointer_cast<BoundAssignmentExpression>(node);

    std::any variable =
        this->evaluate<std::any>(assignmentExpression->getLeft());
    if (variable.type() != typeid(std::string)) {
      this->root->logs.push_back("Error Unexpected variable name");
      return nullptr;
    }
    std::string variable_name = std::any_cast<std::string>(variable);

    switch (assignmentExpression->getOperator()) {
    case BinderKindUtils::BoundBinaryOperatorKind::Assignment: {

      this->root->variables[variable_name] = Utils::Variable(
          this->evaluate<std::any>(assignmentExpression->getRight()),
          this->root->variables[variable_name].isConst);

      this->last_value = this->root->variables[variable_name].value;
      break;
    }
    default:
      this->root->logs.push_back("Error: Unexpected assignment operator");
      return nullptr;
    }
    return this->root->variables[variable_name].value;
  }
  case BinderKindUtils::BoundNodeKind::BinaryExpression: {
    std::shared_ptr<BoundBinaryExpression> binaryExpression =
        std::dynamic_pointer_cast<BoundBinaryExpression>(node);
    std::any left_any = (this->evaluate<std::any>(binaryExpression->getLeft()));
    std::any right_any =
        (this->evaluate<std::any>(binaryExpression->getRight()));

    try {
      return Evaluator::binaryExpressionEvaluator<std::any>(
          binaryExpression->getOperator(), left_any, right_any);
    } catch (const std::exception &e) {
      this->root->logs.push_back(e.what());
      return nullptr;
    }
  }
  case BinderKindUtils::BoundNodeKind::ParenthesizedExpression: {
    std::shared_ptr<BoundParenthesizedExpression> parenthesizedExpression =
        std::dynamic_pointer_cast<BoundParenthesizedExpression>(node);
    return this->evaluate<T>(parenthesizedExpression->getExpression());
  }
  case BinderKindUtils::BoundNodeKind::CallExpression: {
    std::shared_ptr<BoundCallExpression> callExpression =
        std::dynamic_pointer_cast<BoundCallExpression>(node);

    Utils::FunctionSymbol function = callExpression->getFunctionSymbol();

    std::size_t arguments_size = callExpression->getArguments().size();

    if (function.name == Utils::BuiltInFunctions::input.name) {

      if (arguments_size == 0) {
        std::string input;
        std::getline(std::cin, input);
        return input;
      } else if (arguments_size == 1) {
        std::cout << std::any_cast<std::string>(
            this->evaluate<std::any>(callExpression->getArguments()[0]));
        std::string input;
        std::getline(std::cin, input);
        return input;
      } else {
        this->root->logs.push_back(
            "Error: Unexpected function cal: arguments does  not match");
        return nullptr;
      }

    } else if (function.name == Utils::BuiltInFunctions::print.name) {

      if (arguments_size == 0) {

        this->root->logs.push_back(
            "Error: Unexpected function cal: arguments does  not match");
        return nullptr;
      } else if (arguments_size == 1) {

        std::any value =
            (this->evaluate<std::any>((std::shared_ptr<BoundExpression>)
                                          callExpression->getArguments()[0]));

        try {
          std::cout << Utils::convertAnyToString(value);
        } catch (const std::exception &e) {
          this->root->logs.push_back(e.what());
        }
        return nullptr;
      } else {
        this->root->logs.push_back(
            "Error: Unexpected function cal: arguments does  not match");
        return nullptr;
      }
      return nullptr;
    } else if (function.name == Utils::BuiltInFunctions::String.name) {
      if (arguments_size == 1) {
        std::any value =
            (this->evaluate<std::any>((std::shared_ptr<BoundExpression>)
                                          callExpression->getArguments()[0]));
        if (value.type() == typeid(std::string)) {
          return std::any_cast<std::string>(value);
        } else if (value.type() == typeid(int)) {
          return std::to_string(std::any_cast<int>(value));
        } else if (value.type() == typeid(double)) {
          return std::to_string(std::any_cast<double>(value));
        } else if (value.type() == typeid(bool)) {
          return std::to_string(std::any_cast<bool>(value));
        } else {
          this->root->logs.push_back(
              "Error: Unexpected function call argument: required string");
          return nullptr;
        }
      } else {
        this->root->logs.push_back(
            "Error: Unexpected function cal: arguments does  not match");
        return nullptr;
      }
    } else if (function.name == Utils::BuiltInFunctions::Int32.name) {
      if (arguments_size == 1) {
        std::any value =
            (this->evaluate<std::any>((std::shared_ptr<BoundExpression>)
                                          callExpression->getArguments()[0]));
        if (value.type() == typeid(std::string)) {
          return std::stoi(std::any_cast<std::string>(value));
        } else if (value.type() == typeid(int)) {
          return std::any_cast<int>(value);
        } else if (value.type() == typeid(double)) {
          return int(std::any_cast<double>(value));
        } else if (value.type() == typeid(bool)) {
          return int(std::any_cast<bool>(value));
        } else {
          this->root->logs.push_back(
              "Error: Unexpected function call argument: required int");
          return nullptr;
        }
      } else {
        this->root->logs.push_back(
            "Error: Unexpected function cal: arguments does  not match");
        return nullptr;
      }
    } else if (function.name == Utils::BuiltInFunctions::Double.name) {
      if (arguments_size == 1) {
        std::any value =
            (this->evaluate<std::any>((std::shared_ptr<BoundExpression>)
                                          callExpression->getArguments()[0]));
        if (value.type() == typeid(std::string)) {
          return std::stod(std::any_cast<std::string>(value));
        } else if (value.type() == typeid(int)) {
          return double(std::any_cast<int>(value));
        } else if (value.type() == typeid(double)) {
          return std::any_cast<double>(value);
        } else if (value.type() == typeid(bool)) {
          return double(std::any_cast<bool>(value));
        } else {
          this->root->logs.push_back(
              "Error: Unexpected function call argument: required double");
          return nullptr;
        }
      } else {
        this->root->logs.push_back(
            "Error: Unexpected function cal: arguments does  not match");
        return nullptr;
      }
    } else if (function.name == Utils::BuiltInFunctions::Bool.name) {
      if (arguments_size == 1) {
        std::any value =
            (this->evaluate<std::any>((std::shared_ptr<BoundExpression>)
                                          callExpression->getArguments()[0]));
        if (value.type() == typeid(std::string)) {
          bool is = std::any_cast<std::string>(value) == "false" ||
                    std::any_cast<std::string>(value) == "0";
          return !is;
        } else if (value.type() == typeid(int)) {
          return std::any_cast<int>(value) != 0;
        } else if (value.type() == typeid(double)) {
          return std::any_cast<double>(value) != 0;
        } else if (value.type() == typeid(bool)) {
          return std::any_cast<bool>(value);
        } else {
          this->root->logs.push_back(
              "Error: Unexpected function call argument: required bool");
          return nullptr;
        }
      } else {
        this->root->logs.push_back(
            "Error: Unexpected function cal: arguments does  not match");
        return nullptr;
      }
    } else {
      // std::vector<std::any> arguments;
      // for (std::shared_ptr< BoundExpression >argument :
      // callExpression->getArguments()) {
      //   arguments.push_back(this->evaluate<std::any>(argument));
      // }
      // return function.call(arguments);

      this->root->logs.push_back("Error: Unexpected function call");
      return nullptr;
    }
  }
  default: {
    this->root->logs.push_back("Error: Unexpected node");
    return nullptr;
  }
  }
}
template <typename T>
T Evaluator::unaryExpressionEvaluator(
    BinderKindUtils::BoundUnaryOperatorKind op, T operand) {
  if (operand.type() == typeid(int)) {
    return Evaluator::unaryExpressionEvaluatorHandler<int, std::any>(
        op, std::any_cast<int>(operand));
  } else if (operand.type() == typeid(double)) {
    return Evaluator::unaryExpressionEvaluatorHandler<double, std::any>(
        op, std::any_cast<double>(operand));
  } else if (operand.type() == typeid(bool)) {
    return Evaluator::unaryExpressionEvaluatorHandler<bool, std::any>(
        op, std::any_cast<bool>(operand));
  } else {
    throw std::runtime_error("Error: Unexpected operand type");
    return operand;
  }
}

// switch (unaryExpression->getOperator()) {
// case BinderKindUtils::BoundUnaryOperatorKind::Identity:
//   return operand;
// case BinderKindUtils::BoundUnaryOperatorKind::Negation:
//   return -operand;
// case BinderKindUtils::BoundUnaryOperatorKind::LogicalNegation:
//   return !operand;
// default:
//   this->root->logs.push_back "Error: Unexpected unary operator";
// }

template <typename V, typename U> // Int
U Evaluator::unaryExpressionEvaluatorHandler(
    BinderKindUtils::BoundUnaryOperatorKind op, V operand) {
  switch (op) {
  case BinderKindUtils::BoundUnaryOperatorKind::Identity:
    return operand;
  case BinderKindUtils::BoundUnaryOperatorKind::Negation:
    return -operand;
  case BinderKindUtils::BoundUnaryOperatorKind::LogicalNegation:
    return !operand;
  case BinderKindUtils::BoundUnaryOperatorKind::BitwiseNegation:
    return ~int(operand);
  default:
    throw std::runtime_error("Error: Unexpected unary operator");
    return operand;
  }
}

template <typename T>
T Evaluator::binaryExpressionEvaluator(
    BinderKindUtils::BoundBinaryOperatorKind op, T left, T right) {
  if (left.type() == typeid(int) && right.type() == typeid(int)) {
    return binaryExpressionEvaluatorIntIntHandler<int, int, int>(
        op, std::any_cast<int>(left), std::any_cast<int>(right));
  } else if (left.type() == typeid(double) && right.type() == typeid(double)) {

    return binaryExpressionEvaluatorDoubleDoubleHandler<double, double, double>(
        op, std::any_cast<double>(left), std::any_cast<double>(right));
  } else if (left.type() == typeid(double) && right.type() == typeid(int)) {

    return binaryExpressionEvaluatorDoubleIntHandler<double, int, double>(
        op, std::any_cast<double>(left), std::any_cast<int>(right));
  } else if (left.type() == typeid(int) && right.type() == typeid(double)) {

    return binaryExpressionEvaluatorIntDoubleHandler<int, double, double>(
        op, std::any_cast<int>(left), std::any_cast<double>(right));
  } else if (left.type() == typeid(bool) && right.type() == typeid(bool)) {

    return binaryExpressionEvaluatorBoolBoolHandler<bool, bool, std::any>(
        op, std::any_cast<bool>(left), std::any_cast<bool>(right));

  } else if (left.type() == typeid(std::string) &&
             right.type() == typeid(std::string)) {
    return binaryExpressionEvaluatorStringStringHandler<std::string,
                                                        std::string, std::any>(
        op, std::any_cast<std::string>(left),
        std::any_cast<std::string>(right));
  } else if (left.type() == typeid(std::string) &&
             right.type() == typeid(int)) {
    return binaryExpressionEvaluatorStringIntHandler<std::string, int,
                                                     std::any>(
        op, std::any_cast<std::string>(left), std::any_cast<int>(right));
  } else if (left.type() == typeid(int) &&
             right.type() == typeid(std::string)) {
    return binaryExpressionEvaluatorIntStringHandler<int, std::string,
                                                     std::any>(
        op, std::any_cast<int>(left), std::any_cast<std::string>(right));
  } else if (left.type() == typeid(std::string) &&
             right.type() == typeid(double)) {
    return binaryExpressionEvaluatorStringDoubleHandler<std::string, double,
                                                        std::any>(
        op, std::any_cast<std::string>(left), std::any_cast<double>(right));
  } else if (left.type() == typeid(double) &&
             right.type() == typeid(std::string)) {
    return binaryExpressionEvaluatorDoubleStringHandler<double, std::string,
                                                        std::any>(
        op, std::any_cast<double>(left), std::any_cast<std::string>(right));
  } else if (left.type() == typeid(std::string) &&
             right.type() == typeid(bool)) {
    return binaryExpressionEvaluatorStringBoolHandler<std::string, bool,
                                                      std::any>(
        op, std::any_cast<std::string>(left), std::any_cast<bool>(right));
  } else if (left.type() == typeid(bool) &&
             right.type() == typeid(std::string)) {
    return binaryExpressionEvaluatorBoolStringHandler<bool, std::string,
                                                      std::any>(
        op, std::any_cast<bool>(left), std::any_cast<std::string>(right));
  }
  // intbool
  else if (left.type() == typeid(int) && right.type() == typeid(bool)) {
    return binaryExpressionEvaluatorIntBoolHandler<int, bool, int>(
        op, std::any_cast<int>(left), std::any_cast<bool>(right));
  }
  // boolint
  else if (left.type() == typeid(bool) && right.type() == typeid(int)) {
    return binaryExpressionEvaluatorBoolIntHandler<bool, int, int>(
        op, std::any_cast<bool>(left), std::any_cast<int>(right));
  }

  // doublebool
  else if (left.type() == typeid(double) && right.type() == typeid(bool)) {
    return binaryExpressionEvaluatorDoubleBoolHandler<double, bool, double>(
        op, std::any_cast<double>(left), std::any_cast<bool>(right));
  }
  // booldouble
  else if (left.type() == typeid(bool) && right.type() == typeid(double)) {
    return binaryExpressionEvaluatorBoolDoubleHandler<bool, double, double>(
        op, std::any_cast<bool>(left), std::any_cast<double>(right));
  } else {
    throw std::runtime_error("Error: Incompatible operand types");
    return left;
  }
}

template <typename V, typename U, typename Z> // BoolDouble
Z Evaluator::binaryExpressionEvaluatorBoolDoubleHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    return left - right;
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    return left * right;
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    if (!right) {
      throw std::runtime_error("Error: Bool Division by zero");
      return left;
    }
    return left / right;
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    if (!right) {
      throw std::runtime_error("Error: Bool Modulus by zero");
      return left;
    }
    return static_cast<int>(left) % static_cast<int>(right);
  default:
    throw std::runtime_error("Error: Incompatible operand types");
    return left;
  }
}

template <typename V, typename U, typename Z> // IntInt
Z Evaluator::binaryExpressionEvaluatorIntIntHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    return left - right;
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    return left * right;
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    if (!right) {
      throw std::runtime_error("Error: Integer Division by zero");
      return left;
    }
    return left / right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    return left && right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    return left || right;
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    return left & right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    return left | right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    return left ^ right;
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    if (!right) {
      throw std::runtime_error("Error: Integer Modulus by zero");
      return left;
    }
    return static_cast<int>(left) % static_cast<int>(right);
  default:
    throw std::runtime_error("Error: Unexpected binary operator");
    return left;
  }
}

template <typename V, typename U, typename Z> // DoubleDouble
Z Evaluator::binaryExpressionEvaluatorDoubleDoubleHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    return left - right;
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    return left * right;
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    if (!right) {
      throw std::runtime_error("Error: Double Division by zero");
      return left;
    }
    return left / right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    return left && right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    return left || right;
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    if (!right) {
      throw std::runtime_error("Error: Double Modulus by zero");
      return left;
    }
    return static_cast<int>(left) % static_cast<int>(right);
  default:
    throw std::runtime_error("Error: Unexpected binary operator");
    return left;
  }
}

template <typename V, typename U, typename Z> // BoolBool
Z Evaluator::binaryExpressionEvaluatorBoolBoolHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    return left - right;
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    return left * right;
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    if (!right) {
      throw std::runtime_error("Error: Bool Division by zero");
      return left;
    }
    return left / right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    return left && right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    return left || right;
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    throw std::runtime_error("Error: Less not supported for bool");
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    throw std::runtime_error("Error: Less or equals not supported for bool");
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    throw std::runtime_error("Error: Greater not supported for bool");
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    throw std::runtime_error("Error: Greater or equals not supported for bool");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    return left & right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    return left | right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    return left ^ right;
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    if (!right) {
      throw std::runtime_error("Error: Bool Modulus by zero");
      return left;
    }
    return static_cast<int>(left) % static_cast<int>(right);
  default:
    throw std::runtime_error("Error: Unexpected binary operator");
    return left;
  }
}

template <typename V, typename U, typename Z> // StringString
Z Evaluator::binaryExpressionEvaluatorStringStringHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    throw std::runtime_error("Error: Subtraction not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    throw std::runtime_error("Error: Multiplication not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    throw std::runtime_error("Error: Division not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    throw std::runtime_error("Error: Logical and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    throw std::runtime_error("Error: Logical or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    throw std::runtime_error("Error: Modulus not supported for string");
  default:
    throw std::runtime_error("Error: Unexpected binary operator");
    return left;
  }
}

template <typename V, typename U, typename Z> // IntString
Z Evaluator::binaryExpressionEvaluatorIntStringHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return std::to_string(left) + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    throw std::runtime_error("Error: Subtraction not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    throw std::runtime_error("Error: Multiplication not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    throw std::runtime_error("Error: Division not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    throw std::runtime_error("Error: Logical and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    throw std::runtime_error("Error: Logical or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == std::stoi(right);
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != std::stoi(right);
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < std::stoi(right);
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= std::stoi(right);
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > std::stoi(right);
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= std::stoi(right);
  case BinderKindUtils::BoundBinaryOperatorKind::Assignment:
    throw std::runtime_error("Error: Assignment not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    throw std::runtime_error("Error: Modulus not supported for string");
  default:
    throw std::runtime_error("Error: Unexpected binary operator");
    return left;
  }
}

template <typename V, typename U, typename Z> // StringInt
Z Evaluator::binaryExpressionEvaluatorStringIntHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + std::to_string(right);
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    throw std::runtime_error("Error: Subtraction not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    throw std::runtime_error("Error: Multiplication not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    throw std::runtime_error("Error: Division not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    throw std::runtime_error("Error: Logical and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    throw std::runtime_error("Error: Logical or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return std::stoi(left) == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return std::stoi(left) != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return std::stoi(left) < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return std::stoi(left) <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return std::stoi(left) > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return std::stoi(left) >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    throw std::runtime_error("Error: Modulus not supported for string");
  default:
    throw std::runtime_error("Error: Unexpected binary operator");
    return left;
  }
}

template <typename V, typename U, typename Z> // DoubleInt
Z Evaluator::binaryExpressionEvaluatorDoubleIntHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    return left - right;
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    return left * right;
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    if (!right) {
      throw std::runtime_error("Error: Double Division by zero");
      return left;
    }
    return left / right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    throw std::runtime_error("Error: Logical and not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    throw std::runtime_error("Error: Logical or not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    if (!right) {
      throw std::runtime_error("Error: Double Modulus by zero");
      return left;
    }
    return static_cast<int>(left) % static_cast<int>(right);
  default:
    throw std::runtime_error("Error: Unexpected binary operator");
    return left;
  }
}

template <typename V, typename U, typename Z> // IntDouble
Z Evaluator::binaryExpressionEvaluatorIntDoubleHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    return left - right;
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    return left * right;
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    if (!right) {
      throw std::runtime_error("Error: Double Division by zero");
      return left;
    };
    return left / right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    throw std::runtime_error("Error: Logical and not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    throw std::runtime_error("Error: Logical or not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    throw std::runtime_error("Error: Modulus not supported for double");
  default:
    throw std::runtime_error("Error: Unexpected binary operator");
    return left;
  }
}

template <typename V, typename U, typename Z>
Z // StringBool
Evaluator::binaryExpressionEvaluatorStringDoubleHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + std::to_string(right);
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    throw std::runtime_error("Error: Subtraction not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    throw std::runtime_error("Error: Multiplication not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    throw std::runtime_error("Error: Division not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    throw std::runtime_error("Error: Logical and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    throw std::runtime_error("Error: Logical or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return std::stod(left) == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return std::stod(left) != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return std::stod(left) < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return std::stod(left) <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return std::stod(left) > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return std::stod(left) >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    throw std::runtime_error("Error: Modulus not supported for string");
  default:
    throw std::runtime_error(
        "Error: Unexpected binary operator"); // TODO: add error message
    return left;
  }
}

template <typename V, typename U, typename Z> // DoubleString
Z Evaluator::binaryExpressionEvaluatorDoubleStringHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return std::to_string(left) + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    throw std::runtime_error("Error: Subtraction not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    throw std::runtime_error("Error: Multiplication not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    throw std::runtime_error("Error: Division not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    throw std::runtime_error("Error: Logical and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    throw std::runtime_error("Error: Logical or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == std::stod(right);
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != std::stod(right);
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < std::stod(right);
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= std::stod(right);
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > std::stod(right);
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= std::stod(right);
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    throw std::runtime_error("Error: Modulus not supported for string");
  default:
    throw std::runtime_error(
        "Error:  Unexpected binary operator"); // TODO: add error message
    return left;
  }
}

template <typename V, typename U, typename Z> // StringBool
Z Evaluator::binaryExpressionEvaluatorStringBoolHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    throw std::runtime_error("Error: Addition not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    throw std::runtime_error("Error: Subtraction not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    throw std::runtime_error("Error: Multiplication not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    throw std::runtime_error("Error: Division not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    throw std::runtime_error("Error: Logical and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    throw std::runtime_error("Error: Logical or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == std::to_string(right);
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != std::to_string(right);
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    throw std::runtime_error("Error: Less not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    throw std::runtime_error("Error: Less or equals not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    throw std::runtime_error("Error: Greater not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    throw std::runtime_error(
        "Error: Greater or equals not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    throw std::runtime_error("Error: Mod not supported for string");
  default:
    throw std::runtime_error(
        "Error: Unexpected binary operator"); // TODO: add error message
    return left;
  }
}

template <typename V, typename U, typename Z> // BoolString
Z Evaluator::binaryExpressionEvaluatorBoolStringHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    throw std::runtime_error("Error: Addition not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    throw std::runtime_error("Error: Subtraction not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    throw std::runtime_error("Error: Multiplication not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    throw std::runtime_error("Error: Division not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    throw std::runtime_error("Error: Logical and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    throw std::runtime_error("Error: Logical or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return std::to_string(left) == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return std::to_string(left) != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    throw std::runtime_error("Error: Less not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    throw std::runtime_error("Error: Less or equals not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    throw std::runtime_error("Error: Greater not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    throw std::runtime_error(
        "Error: Greater or equals not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Assignment:
    throw std::runtime_error("Error: Assignment not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for string");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    throw std::runtime_error("Error: Mod not supported for string");
  default:
    throw std::runtime_error(
        "Error: Unexpected binary operator"); // TODO: add error message
    return left;
  }
}

template <typename V, typename U, typename Z> // IntBool
Z Evaluator::binaryExpressionEvaluatorIntBoolHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    return left - right;
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    return left * right;
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    if (!right) {
      throw std::runtime_error("Error: Integer Division by zero");
      return left;
    }
    return left / right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    return left && right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    return left || right;
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    return left & right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    return left | right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    return left ^ right;
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    if (!right) {
      throw std::runtime_error("Error: Integer Modulus by zero");
      return left;
    }
    return static_cast<int>(left) % static_cast<int>(right);
  default:
    throw std::runtime_error(
        "Error: Unexpected binary operator"); // TODO: add error message
    return left;
  }
}

template <typename V, typename U, typename Z> // BoolInt
Z Evaluator::binaryExpressionEvaluatorBoolIntHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    throw std::runtime_error("Error: Addition not supported for int");
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    throw std::runtime_error("Error: Subtraction not supported for int");
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    throw std::runtime_error("Error: Multiplication not supported for int");
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    throw std::runtime_error("Error: Division not supported for int");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    return left && right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    return left || right;
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    return left & right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    return left | right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    return left ^ right;
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    return static_cast<int>(left) % static_cast<int>(right);
  default:
    throw std::runtime_error(
        "Error: Unexpected binary operator"); // TODO: add error message
    return left;
  }
}

template <typename V, typename U, typename Z> // DoubleBool
Z Evaluator::binaryExpressionEvaluatorDoubleBoolHandler(
    BinderKindUtils::BoundBinaryOperatorKind op, const V &left,
    const U &right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    throw std::runtime_error("Error: Addition not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    throw std::runtime_error("Error: Subtraction not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    throw std::runtime_error("Error: Multiplication not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    throw std::runtime_error("Error: Division not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd:
    return left && right;
  case BinderKindUtils::BoundBinaryOperatorKind::LogicalOr:
    return left || right;
  case BinderKindUtils::BoundBinaryOperatorKind::Equals:
    return left == right;
  case BinderKindUtils::BoundBinaryOperatorKind::NotEquals:
    return left != right;
  case BinderKindUtils::BoundBinaryOperatorKind::Less:
    return left < right;
  case BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals:
    return left <= right;
  case BinderKindUtils::BoundBinaryOperatorKind::Greater:
    return left > right;
  case BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals:
    return left >= right;
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd:
    throw std::runtime_error("Error: Bitwise and not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr:
    throw std::runtime_error("Error: Bitwise or not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor:
    throw std::runtime_error("Error: Bitwise xor not supported for double");
  case BinderKindUtils::BoundBinaryOperatorKind::Modulus:
    throw std::runtime_error("Error: Mod not supported for double");
  default:
    throw std::runtime_error(
        "Error: Unexpected binary operator"); // TODO: add error message
    return left;
  }
}