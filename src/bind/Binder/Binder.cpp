
#include "Binder.h"

BoundStatement *Binder::bindStatement(StatementSyntax *syntax) {
  switch (syntax->getKind()) {
  case SyntaxKindUtils::SyntaxKind::ExpressionStatement: {
    ExpressionStatementSyntax *expressionStatement =
        (ExpressionStatementSyntax *)syntax;
    BoundExpression *expression =
        bindExpression(expressionStatement->getExpression());
    return new BoundExpressionStatement(expression);
  }
  case SyntaxKindUtils::SyntaxKind::BlockStatement: {
    BlockStatementSyntax *blockStatement = (BlockStatementSyntax *)syntax;

    this->root = new BoundScope(this->root);

    std::vector<BoundStatement *> statements;
    for (int i = 0; i < blockStatement->getStatements().size(); i++) {
      statements.push_back(bindStatement(blockStatement->getStatements()[i]));
    }

    this->root = this->root->parent;
    return new BoundBlockStatement(statements);
  }
  case SyntaxKindUtils::SyntaxKind::VariableDeclaration: {
    VariableDeclarationSyntax *variableDeclaration =
        (VariableDeclarationSyntax *)syntax;
    BoundExpression *initializer =
        bindExpression(variableDeclaration->getInitializer());

    std::string variable_str = std::any_cast<std::string>(
        variableDeclaration->getIdentifier()->getValue());
    bool isConst = variableDeclaration->getKeyword()->getKind() ==
                   SyntaxKindUtils::SyntaxKind::ConstKeyword;

    if (!root->tryDeclareVariable(variable_str,
                                  Utils::Variable(nullptr, isConst))) {
      logs.push_back("Error: Variable " + variable_str + " already exists");
    }

    return new BoundVariableDeclaration(variable_str, isConst, initializer);
  }
  case SyntaxKindUtils::SyntaxKind::IfStatement: {
    IfStatementSyntax *ifStatement = (IfStatementSyntax *)syntax;
    BoundExpression *condition = bindExpression(ifStatement->getCondition());
    BoundStatement *thenStatement =
        bindStatement((StatementSyntax *)(ifStatement->getStatement()));
    BoundStatement *elseStatement = nullptr;
    if (ifStatement->getElseClause() != nullptr) {
      elseStatement = bindStatement(
          (StatementSyntax *)ifStatement->getElseClause()->getStatement());
    }
    return new BoundIfStatement(condition, thenStatement, elseStatement);
  }
  case SyntaxKindUtils::SyntaxKind::WhileStatement: {
    WhileStatementSyntax *whileStatement = (WhileStatementSyntax *)syntax;
    BoundExpression *condition = bindExpression(whileStatement->getCondition());
    BoundStatement *body =
        bindStatement((StatementSyntax *)whileStatement->getBody());
    return new BoundWhileStatement(condition, body);
  }

  case SyntaxKindUtils::SyntaxKind::ForStatement: {
    ForStatementSyntax *forStatement = (ForStatementSyntax *)syntax;
    BoundStatement *intializer = (BoundStatement *)bindStatement(
        (StatementSyntax *)forStatement->getInitialization());

    BoundExpression *upperBound =
        (BoundLiteralExpression<std::any> *)bindExpression(
            forStatement->getUpperBound());

    BoundStatement *body =
        bindStatement((StatementSyntax *)forStatement->getStatement());

    return new BoundForStatement(intializer, upperBound, (body));
  }
  default:
    throw "Unexpected syntax";
  }
}

BoundExpression *Binder::bindExpression(ExpressionSyntax *syntax) {
  switch (syntax->getKind()) {
  case SyntaxKindUtils::SyntaxKind::LiteralExpression: {

    std::any value = ((LiteralExpressionSyntax<std::any> *)syntax)->getValue();
    return new BoundLiteralExpression<std::any>(value);
  }
  case SyntaxKindUtils::SyntaxKind::UnaryExpression: {
    UnaryExpressionSyntax *unaryExpression = (UnaryExpressionSyntax *)syntax;
    BoundExpression *operand = bindExpression(unaryExpression->getOperand());
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
    return new BoundUnaryExpression(op, operand);
  }
  case SyntaxKindUtils::SyntaxKind::BinaryExpression: {
    BinaryExpressionSyntax *binaryExpression = (BinaryExpressionSyntax *)syntax;
    BoundExpression *left = bindExpression(binaryExpression->getLeft());
    BoundExpression *right = bindExpression(binaryExpression->getRight());
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
    return new BoundBinaryExpression(left, op, right);
  }

    // Assignment Expression

  case SyntaxKindUtils::SyntaxKind::AssignmentExpression: {
    AssignmentExpressionSyntax *assignmentExpression =
        (AssignmentExpressionSyntax *)syntax;
    BoundLiteralExpression<std::any> *identifierExpression =
        (BoundLiteralExpression<std::any> *)bindExpression(
            assignmentExpression->getLeft());
    BoundExpression *right = bindExpression(assignmentExpression->getRight());
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
    return new BoundAssignmentExpression(identifierExpression, op, right);
  }

  case SyntaxKindUtils::SyntaxKind::VariableExpression: {
    VariableExpressionSyntax *variableExpressionSyntax =
        (VariableExpressionSyntax *)syntax;

    BoundLiteralExpression<std::any> *identifierExpression =
        (BoundLiteralExpression<std::any> *)bindExpression(
            variableExpressionSyntax->getIdentifier());
    std::string variable_str =
        std::any_cast<std::string>(identifierExpression->getValue());
    if (!root->tryLookupVariable(variable_str)) {
      logs.push_back("Error: Variable " + variable_str + " does not exist");
      return identifierExpression;
    }
    return new BoundVariableExpression(identifierExpression);
  }
  case SyntaxKindUtils::SyntaxKind::ParenthesizedExpression: {
    ParenthesizedExpressionSyntax *parenthesizedExpression =
        (ParenthesizedExpressionSyntax *)syntax;
    return bindExpression(parenthesizedExpression->getExpression());
  }
  case SyntaxKindUtils::SyntaxKind::CallExpression: {
    CallExpressionSyntax *callExpression = (CallExpressionSyntax *)syntax;
    BoundLiteralExpression<std::any> *identifier =
        (BoundLiteralExpression<std::any> *)bindExpression(
            callExpression->getIdentifier());

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

    std::vector<BoundExpression *> arguments;
    for (int i = 0; i < callExpression->getArguments().size(); i++) {
      arguments.push_back(bindExpression(callExpression->getArguments()[i]));
    }

    return new BoundCallExpression(functionSymbol, arguments);
  }
  default:
    throw "Unexpected syntax";
  }
}
BoundScope *Binder::CreateParentScope(BoundScopeGlobal *parent) {
  std::stack<BoundScopeGlobal *> stack = std::stack<BoundScopeGlobal *>();

  while (parent != nullptr) {
    stack.push(parent);
    parent = parent->previous;
  }

  BoundScope *current = nullptr;

  while (!stack.empty()) {
    parent = stack.top();
    stack.pop();
    BoundScope *scope = new BoundScope(current);
    for (auto &pair : parent->variables) {
      scope->variables[pair.first] = pair.second;
    }
    current = scope;
  }

  return current;
}

Binder::Binder(BoundScope *parent) { this->root = new BoundScope(parent); }

BoundScopeGlobal *
Binder::bindGlobalScope(BoundScopeGlobal *previous,
                        std::shared_ptr<CompilationUnitSyntax> syntax) {

  Binder *binder = new Binder(Binder::CreateParentScope(previous));
  BoundStatement *statement = binder->bindStatement(syntax->getStatement());
  std::vector<std::string> logs = binder->logs;
  std::map<std::string, struct Utils::Variable> variables =
      binder->root->variables;
  return new BoundScopeGlobal(previous, variables, logs, statement);
}
