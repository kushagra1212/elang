
#include "Binder.h"

std::shared_ptr<BoundStatement>
Binder::bindStatement(std::shared_ptr<StatementSyntax> syntax) {
  switch (syntax->getKind()) {
  case SyntaxKindUtils::SyntaxKind::ExpressionStatement: {
    std::shared_ptr<ExpressionStatementSyntax> expressionStatement =
        std::dynamic_pointer_cast<ExpressionStatementSyntax>(syntax);
    std::shared_ptr<BoundExpression> expression =
        bindExpression(expressionStatement->getExpression());
    return std::make_shared<BoundExpressionStatement>(std::move(expression));
  }
  case SyntaxKindUtils::SyntaxKind::BlockStatement: {
    std::shared_ptr<BlockStatementSyntax> blockStatement =
        std::dynamic_pointer_cast<BlockStatementSyntax>(syntax);

    this->root = std::shared_ptr<BoundScope>(this->root);

    std::vector<std::shared_ptr<BoundStatement>> statements;
    for (int i = 0; i < blockStatement->getStatements().size(); i++) {
      statements.push_back(bindStatement(blockStatement->getStatements()[i]));
    }

    this->root = this->root->parent;
    return std::make_shared<BoundBlockStatement>(std::move(statements));
  }
  case SyntaxKindUtils::SyntaxKind::VariableDeclaration: {
    std::shared_ptr<VariableDeclarationSyntax> variableDeclaration =
        std::dynamic_pointer_cast<VariableDeclarationSyntax>(syntax);
    std::shared_ptr<BoundExpression> initializer =
        bindExpression(variableDeclaration->getInitializer());

    std::string variable_str = std::any_cast<std::string>(
        variableDeclaration->getIdentifier()->getValue());
    bool isConst = variableDeclaration->getKeyword()->getKind() ==
                   SyntaxKindUtils::SyntaxKind::ConstKeyword;

    if (!root->tryDeclareVariable(variable_str,
                                  Utils::Variable(nullptr, isConst))) {
      logs.push_back("Error: Variable " + variable_str + " already exists");
    }

    return std::make_shared<BoundVariableDeclaration>(variable_str, isConst,
                                                      std::move(initializer));
  }
  case SyntaxKindUtils::SyntaxKind::IfStatement: {
    std::shared_ptr<IfStatementSyntax> ifStatement =
        std::dynamic_pointer_cast<IfStatementSyntax>(syntax);
    std::shared_ptr<BoundExpression> condition =
        bindExpression(ifStatement->getCondition());
    std::shared_ptr<BoundStatement> thenStatement =
        bindStatement(std::dynamic_pointer_cast<StatementSyntax>(
            ifStatement->getStatement()));
    std::shared_ptr<BoundStatement> elseStatement = nullptr;
    if (ifStatement->getElseClause() != nullptr) {
      elseStatement = bindStatement(std::dynamic_pointer_cast<StatementSyntax>(
          ifStatement->getElseClause()->getStatement()));
    }
    return std::make_shared<BoundIfStatement>(std::move(condition),
                                              std::move(thenStatement),
                                              std::move(elseStatement));
  }
  case SyntaxKindUtils::SyntaxKind::WhileStatement: {
    std::shared_ptr<WhileStatementSyntax> whileStatement =
        std::dynamic_pointer_cast<WhileStatementSyntax>(syntax);
    std::shared_ptr<BoundExpression> condition =
        bindExpression(whileStatement->getCondition());
    std::shared_ptr<BoundStatement> body = bindStatement(
        std::dynamic_pointer_cast<StatementSyntax>(whileStatement->getBody()));
    return std::make_shared<BoundWhileStatement>(std::move(condition),
                                                 std::move(body));
  }

  case SyntaxKindUtils::SyntaxKind::ForStatement: {
    std::shared_ptr<ForStatementSyntax> forStatement =
        std::dynamic_pointer_cast<ForStatementSyntax>(syntax);
    std::shared_ptr<BoundStatement> intializer =
        (std::shared_ptr<BoundStatement>)bindStatement(
            (std::shared_ptr<StatementSyntax>)
                forStatement->getInitialization());

    std::shared_ptr<BoundExpression> upperBound =
        bindExpression(forStatement->getUpperBound());

    std::shared_ptr<BoundStatement> body =
        bindStatement(std::dynamic_pointer_cast<StatementSyntax>(
            forStatement->getStatement()));

    return std::make_shared<BoundForStatement>(
        std::move(intializer), std::move(upperBound), std::move(body));
  }
  default:
    throw "Unexpected syntax";
  }
}

std::shared_ptr<BoundExpression>
Binder::bindExpression(std::shared_ptr<ExpressionSyntax> syntax) {
  switch (syntax->getKind()) {
  case SyntaxKindUtils::SyntaxKind::LiteralExpression: {

    std::any value =
        (std::dynamic_pointer_cast<LiteralExpressionSyntax<std::any>>(syntax))
            ->getValue();
    return std::make_shared<BoundLiteralExpression<std::any>>(value);
  }
  case SyntaxKindUtils::SyntaxKind::UnaryExpression: {
    std::shared_ptr<UnaryExpressionSyntax> unaryExpression =
        std::dynamic_pointer_cast<UnaryExpressionSyntax>(syntax);
    std::shared_ptr<BoundExpression> operand =
        bindExpression(unaryExpression->getOperand());
    BinderKindUtils::BoundUnaryOperatorKind op;

    switch (unaryExpression->getOperatorToken()->getKind()) {
    case SyntaxKindUtils::SyntaxKind::PlusToken:
      op = BinderKindUtils::BoundUnaryOperatorKind::Identity;
      break;
    case SyntaxKindUtils::SyntaxKind::MinusToken:
      op = BinderKindUtils::BoundUnaryOperatorKind::Negation;
      break;
    case SyntaxKindUtils::SyntaxKind::BangToken:
      op = BinderKindUtils::BoundUnaryOperatorKind::LogicalNegation;
      break;
    case SyntaxKindUtils::SyntaxKind::TildeToken:
      op = BinderKindUtils::BoundUnaryOperatorKind::BitwiseNegation;
      break;
    default:
      throw "Unexpected unary operator";
    }
    return std::make_shared<BoundUnaryExpression>(std::move(op),
                                                  std::move(operand));
  }
  case SyntaxKindUtils::SyntaxKind::BinaryExpression: {
    std::shared_ptr<BinaryExpressionSyntax> binaryExpression =
        std::dynamic_pointer_cast<BinaryExpressionSyntax>(syntax);
    std::shared_ptr<BoundExpression> left =
        bindExpression(binaryExpression->getLeft());
    std::shared_ptr<BoundExpression> right =
        bindExpression(binaryExpression->getRight());
    BinderKindUtils::BoundBinaryOperatorKind op;

    // if (left->getType() != typeid(int) || right->getType() != typeid(int)) {
    //   logs.push_back("Binary operator can only be applied to numbers");
    //   return left;
    // }

    switch (binaryExpression->getOperatorToken()->getKind()) {
    case SyntaxKindUtils::SyntaxKind::PlusToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Addition;
      break;
    case SyntaxKindUtils::SyntaxKind::MinusToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Subtraction;
      break;
    case SyntaxKindUtils::SyntaxKind::StarToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Multiplication;
      break;
    case SyntaxKindUtils::SyntaxKind::SlashToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Division;
      break;
    case SyntaxKindUtils::SyntaxKind::PercentToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Modulus;
      break;
    case SyntaxKindUtils::SyntaxKind::EqualsEqualsToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Equals;
      break;
    case SyntaxKindUtils::SyntaxKind::BangEqualsToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::NotEquals;
      break;
    case SyntaxKindUtils::SyntaxKind::AmpersandAmpersandToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::LogicalAnd;
      break;
    case SyntaxKindUtils::SyntaxKind::PipePipeToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::LogicalOr;
      break;
    case SyntaxKindUtils::SyntaxKind::LessToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Less;
      break;
    case SyntaxKindUtils::SyntaxKind::LessOrEqualsToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::LessOrEquals;
      break;
    case SyntaxKindUtils::SyntaxKind::GreaterToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Greater;
      break;
    case SyntaxKindUtils::SyntaxKind::GreaterOrEqualsToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::GreaterOrEquals;
      break;
    case SyntaxKindUtils::SyntaxKind::EqualsToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Assignment;
      break;
    case SyntaxKindUtils::SyntaxKind::AmpersandToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::BitwiseAnd;
      break;
    case SyntaxKindUtils::SyntaxKind::PipeToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::BitwiseOr;
      break;
    case SyntaxKindUtils::SyntaxKind::CaretToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::BitwiseXor;
      break;
    default:
      throw "Unexpected binary operator";
    }
    return std::make_unique<BoundBinaryExpression>(std::move(left), op,
                                                   std::move(right));
  }

    // Assignment Expression

  case SyntaxKindUtils::SyntaxKind::AssignmentExpression: {
    std::shared_ptr<AssignmentExpressionSyntax> assignmentExpression =
        std::dynamic_pointer_cast<AssignmentExpressionSyntax>(syntax);
    std::shared_ptr<BoundLiteralExpression<std::any>> identifierExpression =
        std::dynamic_pointer_cast<BoundLiteralExpression<std::any>>(
            bindExpression(assignmentExpression->getLeft()));
    std::shared_ptr<BoundExpression> right =
        bindExpression(assignmentExpression->getRight());
    std::string variable_str =
        std::any_cast<std::string>(identifierExpression->getValue());
    BinderKindUtils::BoundBinaryOperatorKind op;
    if (!root->tryLookupVariable(variable_str)) {
      logs.push_back("Error: Variable " + variable_str + " does not exist");
      return identifierExpression;
    }
    if (root->variables[variable_str].isConst) {
      logs.push_back("Error: Variable " + variable_str + " is const");
      return identifierExpression;
    }

    switch (assignmentExpression->getOperatorToken()->getKind()) {
    case SyntaxKindUtils::SyntaxKind::EqualsToken:
      op = BinderKindUtils::BoundBinaryOperatorKind::Assignment;
      break;
    default:
      throw "Unexpected assignment operator";
    }
    return std::make_shared<BoundAssignmentExpression>(
        std::move(identifierExpression), op, std::move(right));
  }

  case SyntaxKindUtils::SyntaxKind::VariableExpression: {
    std::shared_ptr<VariableExpressionSyntax> variableExpressionSyntax =
        std::dynamic_pointer_cast<VariableExpressionSyntax>(syntax);

    std::shared_ptr<BoundLiteralExpression<std::any>> identifierExpression =
        std::dynamic_pointer_cast<BoundLiteralExpression<std::any>>(
            bindExpression(variableExpressionSyntax->getIdentifier()));
    std::string variable_str =
        std::any_cast<std::string>(identifierExpression->getValue());
    if (!root->tryLookupVariable(variable_str)) {
      logs.push_back("Error: Variable " + variable_str + " does not exist");
      return identifierExpression;
    }
    return std::make_shared<BoundVariableExpression>(
        std::move(identifierExpression));
  }
  case SyntaxKindUtils::SyntaxKind::ParenthesizedExpression: {
    std::shared_ptr<ParenthesizedExpressionSyntax> parenthesizedExpression =
        std::dynamic_pointer_cast<ParenthesizedExpressionSyntax>(syntax);
    return bindExpression(parenthesizedExpression->getExpression());
  }
  case SyntaxKindUtils::SyntaxKind::CallExpression: {
    std::shared_ptr<CallExpressionSyntax> callExpression =
        std::dynamic_pointer_cast<CallExpressionSyntax>(syntax);
    std::shared_ptr<BoundLiteralExpression<std::any>> identifier =
        std::dynamic_pointer_cast<BoundLiteralExpression<std::any>>(
            bindExpression(callExpression->getIdentifier()));

    Utils::FunctionSymbol functionSymbol =
        Utils::BuiltInFunctions::getFunctionSymbol(
            std::any_cast<std::string>(identifier->getValue()));

    if (functionSymbol.kind == Utils::SymbolKind::None) {
      logs.push_back("Error: Function " +
                     std::any_cast<std::string>(identifier->getValue()) +
                     " does not exist");
      return identifier;
    }

    // if (callExpression->getArguments().size() != functionSymbol.arity()) {
    //   logs.push_back("Error: Function " +
    //                  std::any_cast<std::string>(identifier->getValue()) +
    //                  " requires " + std::to_string(functionSymbol.arity()) +
    //                  " arguments");
    //   return identifier;
    // }

    std::vector<std::shared_ptr<BoundExpression>> arguments;
    for (int i = 0; i < callExpression->getArguments().size(); i++) {
      arguments.push_back(bindExpression(callExpression->getArguments()[i]));
    }

    return std::make_shared<BoundCallExpression>(functionSymbol, arguments);
  }
  default:
    throw "Unexpected syntax";
  }
}
std::shared_ptr<BoundScope>
Binder::CreateParentScope(std::shared_ptr<BoundScopeGlobal> parent) {
  std::stack<std::shared_ptr<BoundScopeGlobal>> stack =
      std::stack<std::shared_ptr<BoundScopeGlobal>>();

  while (parent != nullptr) {
    stack.push(parent);
    parent = parent->previous;
  }

  std::shared_ptr<BoundScope> current = nullptr;

  while (!stack.empty()) {
    parent = stack.top();
    stack.pop();
    std::shared_ptr<BoundScope> scope = std::make_shared<BoundScope>(current);
    for (auto &pair : parent->variables) {
      scope->variables[pair.first] = pair.second;
    }
    current = scope;
  }

  return current;
}

Binder::Binder(std::shared_ptr<BoundScope> parent) {
  this->root = std::make_shared<BoundScope>(parent);
}

std::shared_ptr<BoundScopeGlobal>
Binder::bindGlobalScope(std::shared_ptr<BoundScopeGlobal> previous,
                        std::shared_ptr<CompilationUnitSyntax> syntax) {

  std::shared_ptr<Binder> binder =
      std::make_shared<Binder>(Binder::CreateParentScope(previous));
  std::shared_ptr<BoundStatement> statement =
      binder->bindStatement(syntax->getStatement());
  std::vector<std::string> logs = binder->logs;
  std::map<std::string, struct Utils::Variable> variables =
      binder->root->variables;
  return std::make_shared<BoundScopeGlobal>(std::move(previous), variables,
                                            logs, std::move(statement));
}
