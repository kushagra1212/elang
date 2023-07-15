#include "Evaluator.h"

template <typename T> T Evaluator::evaluate(BoundExpression *node) {

  switch (node->getKind()) {
  case BinderKindUtils::BoundNodeKind::LiteralExpression: {

    if (auto literalExpression =
            dynamic_cast<BoundLiteralExpression<int> *>(node))
      return (literalExpression->getValue());
    else if (auto literalExpression =
                 dynamic_cast<BoundLiteralExpression<bool> *>(node)) {

      return (literalExpression->getValue());
    } else if (auto literalExpression =
                   dynamic_cast<BoundLiteralExpression<std::string> *>(node)) {

      return (literalExpression->getValue());
    } else {
      throw "Unexpected literal expression";
    }

    return 0;
  }
  case BinderKindUtils::BoundNodeKind::UnaryExpression: {
    BoundUnaryExpression *unaryExpression = (BoundUnaryExpression *)node;
    std::any operand_any =
        (Evaluator::evaluate<std::any>(unaryExpression->getOperand()));
    int operand = 1;

    if (operand_any.type() == typeid(std::string)) {

      throw "Unexpected string in unary expression";
      return 0;
    }

    if (operand_any.type() == typeid(bool)) {
      bool operand_bool = std::any_cast<bool>(operand_any);
      operand = operand_bool ? 1 : 0;
    } else {
      operand = std::any_cast<int>(operand_any);
    }

    switch (unaryExpression->getOperator()) {
    case BinderKindUtils::BoundUnaryOperatorKind::Identity:
      return operand;
    case BinderKindUtils::BoundUnaryOperatorKind::Negation:
      return -operand;
    default:
      throw "Unexpected unary operator";
    }
  }
  case BinderKindUtils::BoundNodeKind::BinaryExpression: {
    BoundBinaryExpression *binaryExpression = (BoundBinaryExpression *)node;
    std::any left_any =
        (Evaluator::evaluate<std::any>(binaryExpression->getLeft()));
    std::any right_any =
        (Evaluator::evaluate<std::any>(binaryExpression->getRight()));

    int left = 1, right = 1;
    if (left_any.type() == typeid(std::string) ||
        right_any.type() == typeid(std::string)) {

      throw "Unexpected string in binary expression";
      return 0;
    }

    if (left_any.type() == typeid(bool)) {
      bool left_bool = std::any_cast<bool>(left_any);
      left = left_bool ? 1 : 0;
    } else {
      left = std::any_cast<int>(left_any);
    }

    if (right_any.type() == typeid(bool)) {
      bool right_bool = std::any_cast<bool>(right_any);
      right = right_bool ? 1 : 0;
    } else {
      right = std::any_cast<int>(right_any);
    }

    return Evaluator::binaryExpressionEvaluator<T>(
        binaryExpression->getOperator(), left, right);
  }
  case BinderKindUtils::BoundNodeKind::ParenthesizedExpression: {
    BoundParenthesizedExpression *parenthesizedExpression =
        (BoundParenthesizedExpression *)node;
    return Evaluator::evaluate<T>(parenthesizedExpression->getExpression());
  }
  default:
    throw "Unexpected node";
  }
}

template <typename T>
T Evaluator::binaryExpressionEvaluator(
    BinderKindUtils::BoundBinaryOperatorKind op, int left, int right) {
  switch (op) {
  case BinderKindUtils::BoundBinaryOperatorKind::Addition:
    return left + right;
  case BinderKindUtils::BoundBinaryOperatorKind::Subtraction:
    return left - right;
  case BinderKindUtils::BoundBinaryOperatorKind::Multiplication:
    return left * right;
  case BinderKindUtils::BoundBinaryOperatorKind::Division:
    return left / right;
  default:
    throw "Unexpected binary operator";
  }
}
template std::any Evaluator::evaluate<std::any>(BoundExpression *node);