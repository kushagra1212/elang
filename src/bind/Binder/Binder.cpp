
#include "Binder.h"

std::unique_ptr<BoundStatement> Binder::bindExpressionStatement(
    ExpressionStatementSyntax *expressionStatement) {
  ExpressionSyntax *exps = expressionStatement->getExpressionPtr().get();

  std::unique_ptr<BoundExpression> boundExpression =
      std::move(bindExpression(exps));

  return std::make_unique<BoundExpressionStatement>(exps->getSourceLocation(),
                                                    std::move(boundExpression));
}

std::unique_ptr<BoundStatement>
Binder::bindBlockStatement(BlockStatementSyntax *blockStatement) {

  this->root = std::make_unique<BoundScope>(std::move(this->root));

  std::unique_ptr<BoundBlockStatement> boundBlockStatement =
      std::make_unique<BoundBlockStatement>(blockStatement->getSourceLocation(),
                                            false);

  for (int i = 0; i < blockStatement->getStatements().size(); i++) {

    std::unique_ptr<BoundStatement> statement =
        std::move(bindStatement(blockStatement->getStatements()[i].get()));

    boundBlockStatement->addStatement(std::move(statement));
  }

  this->root = std::move(this->root->parent);

  return std::move(boundBlockStatement);
}

std::unique_ptr<BoundStatement> Binder::bindVariableDeclaration(
    VariableDeclarationSyntax *variableDeclaration) {

  std::unique_ptr<BoundExpression> boundInitializerExpression =
      std::move(bindExpression(variableDeclaration->getInitializerPtr().get()));

  std::string variable_str = std::any_cast<std::string>(
      variableDeclaration->getIdentifierPtr()->getValue());

  bool isConst = variableDeclaration->getKeywordPtr()->getKind() ==
                 SyntaxKindUtils::SyntaxKind::ConstKeyword;

  if (!root->tryDeclareVariable(
          variable_str,
          Utils::Variable(nullptr, isConst, variableDeclaration->getType()))) {

    this->_diagnosticHandler->addDiagnostic(
        Diagnostic("Variable " + variable_str + " Already Exists",
                   DiagnosticUtils::DiagnosticLevel::Error,
                   DiagnosticUtils::DiagnosticType::Semantic,
                   Utils::getSourceLocation(
                       variableDeclaration->getIdentifierPtr().get())));
  }

  return std::make_unique<BoundVariableDeclaration>(
      variableDeclaration->getSourceLocation(), variable_str, isConst,
      variableDeclaration->getType(), std::move(boundInitializerExpression));
}

std::unique_ptr<BoundOrIfStatement>
Binder::bindOrIfStatement(OrIfStatementSyntax *orIfStatement) {

  std::unique_ptr<BoundExpression> boundCondition =
      std::move(bindExpression(orIfStatement->getConditionPtr().get()));

  std::unique_ptr<BoundStatement> boundThenStatement =
      std::move(bindStatement(orIfStatement->getStatementPtr().get()));

  return std::make_unique<BoundOrIfStatement>(
      orIfStatement->getSourceLocation(), std::move(boundCondition),
      std::move(boundThenStatement));
}

std::unique_ptr<BoundStatement>
Binder::bindIfStatement(IfStatementSyntax *ifStatement) {

  std::unique_ptr<BoundIfStatement> boundIfStatement =
      std::make_unique<BoundIfStatement>(ifStatement->getSourceLocation());

  std::unique_ptr<BoundExpression> boundCondition =
      std::move(bindExpression(ifStatement->getConditionPtr().get()));

  std::unique_ptr<BoundStatement> boundThenStatement =
      std::move(bindStatement(ifStatement->getStatementPtr().get()));

  boundIfStatement->addCondition(std::move(boundCondition));
  boundIfStatement->addThenStatement(std::move(boundThenStatement));

  for (int i = 0; i < ifStatement->getOrIfStatementsPtr().size(); i++) {
    std::unique_ptr<BoundOrIfStatement> boundOrIfStatement = std::move(
        bindOrIfStatement(ifStatement->getOrIfStatementsPtr()[i].get()));
    boundIfStatement->addOrIfStatement(std::move(boundOrIfStatement));
  }

  std::unique_ptr<BoundStatement> boundElseStatement = nullptr;

  if (ifStatement->getElseClausePtr() != nullptr) {
    boundElseStatement = std::move(bindStatement(
        ifStatement->getElseClausePtr()->getStatementPtr().get()));
    boundIfStatement->addElseStatement(std::move(boundElseStatement));
  }

  return std::move(boundIfStatement);
}
std::unique_ptr<BoundStatement>
Binder::bindWhileStatement(WhileStatementSyntax *whileStatement) {

  this->root = std::make_unique<BoundScope>(std::move(this->root));
  this->root->makeBreakableAndContinuable();

  std::unique_ptr<BoundExpression> boundCondition =
      std::move(bindExpression(whileStatement->getConditionPtr().get()));

  std::unique_ptr<BoundStatement> boundBody =
      bindStatement(whileStatement->getBodyPtr().get());

  this->root = std::move(this->root->parent);

  return std::make_unique<BoundWhileStatement>(
      whileStatement->getSourceLocation(), std::move(boundCondition),
      std::move(boundBody));
}

std::unique_ptr<BoundStatement>
Binder::bindForStatement(ForStatementSyntax *forStatement) {
  this->root = std::make_unique<BoundScope>(std::move(this->root));
  this->root->makeBreakableAndContinuable();
  std::unique_ptr<BoundStatement> boundIntializer =
      std::move(bindStatement(forStatement->getInitializationPtr().get()));

  std::unique_ptr<BoundExpression> boundUpperBound =
      std::move(bindExpression(forStatement->getUpperBoundPtr().get()));

  std::unique_ptr<BoundExpression> boundStepExpression = nullptr;

  if (forStatement->getStepExpressionPtr().get())
    boundStepExpression =
        std::move(bindExpression(forStatement->getStepExpressionPtr().get()));

  std::unique_ptr<BoundStatement> boundBody =
      std::move(bindStatement(forStatement->getStatementPtr().get()));

  this->root = std::move(this->root->parent);
  return std::make_unique<BoundForStatement>(
      forStatement->getSourceLocation(), std::move(boundIntializer),
      std::move(boundUpperBound), std::move(boundStepExpression),
      std::move(boundBody));
}

std::unique_ptr<BoundStatement>
Binder::bindBreakStatement(BreakStatementSyntax *breakStatement) {
  if (!this->root->isBreakable()) {

    this->_diagnosticHandler->addDiagnostic(Diagnostic(
        "Break Statement Outside of Loop",
        DiagnosticUtils::DiagnosticLevel::Error,
        DiagnosticUtils::DiagnosticType::Semantic,
        Utils::getSourceLocation(breakStatement->getBreakKeywordPtr().get())));
  }
  return std::make_unique<BoundBreakStatement>(
      breakStatement->getSourceLocation());
}

std::unique_ptr<BoundStatement>
Binder::bindContinueStatement(ContinueStatementSyntax *continueStatement) {
  if (!this->root->isContinuable()) {

    this->_diagnosticHandler->addDiagnostic(
        Diagnostic("Continue Statement Outside Of Loop",
                   DiagnosticUtils::DiagnosticLevel::Error,
                   DiagnosticUtils::DiagnosticType::Semantic,
                   Utils::getSourceLocation(
                       continueStatement->getContinueKeywordPtr().get())));
  }
  return std::make_unique<BoundContinueStatement>(
      continueStatement->getSourceLocation());
}

std::unique_ptr<BoundStatement>
Binder::bindReturnStatement(ReturnStatementSyntax *returnStatement) {
  std::unique_ptr<BoundExpression> boundExpression = nullptr;
  if (!this->root->isInFunction()) {

    this->_diagnosticHandler->addDiagnostic(
        Diagnostic("Return Statement Outside Of Function",
                   DiagnosticUtils::DiagnosticLevel::Error,
                   DiagnosticUtils::DiagnosticType::Semantic,
                   Utils::getSourceLocation(
                       returnStatement->getReturnKeywordPtr().get())));
  } else {
    ExpressionSyntax *returnExpression =
        returnStatement->getExpressionPtr().get();

    if (returnExpression) {
      boundExpression = std::move(bindExpression(returnExpression));
    }
  }

  return std::make_unique<BoundReturnStatement>(
      returnStatement->getSourceLocation(), std::move(boundExpression));
}

std::unique_ptr<BoundStatement> Binder::bindStatement(StatementSyntax *syntax) {

  switch (syntax->getKind()) {
  case SyntaxKindUtils::SyntaxKind::ExpressionStatement: {

    return std::move(
        bindExpressionStatement((ExpressionStatementSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::BlockStatement: {

    return std::move(bindBlockStatement((BlockStatementSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::VariableDeclaration: {

    return std::move(
        bindVariableDeclaration((VariableDeclarationSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::IfStatement: {
    return std::move(bindIfStatement((IfStatementSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::WhileStatement: {
    return std::move(bindWhileStatement((WhileStatementSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::ForStatement: {
    return std::move(bindForStatement((ForStatementSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::BreakKeyword: {
    return std::move(bindBreakStatement((BreakStatementSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::ContinueKeyword: {
    return std::move(bindContinueStatement((ContinueStatementSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::ReturnStatement: {
    return std::move(bindReturnStatement((ReturnStatementSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::BringStatementSyntax: {
    return std::move(bindBringStatement((BringStatementSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::ContainerStatement: {
    return std::move(
        bindContainerStatement((ContainerStatementSyntax *)syntax));
  }
  default:
    throw "Unexpected syntax";
  }
}

std::unique_ptr<BoundExpression> Binder::bindContainerExpression(
    ContainerExpressionSyntax *containerExpression) {

  std::unique_ptr<BoundContainerExpression> boundContainerExpression =
      std::make_unique<BoundContainerExpression>(
          containerExpression->getSourceLocation());

  for (int i = 0; i < containerExpression->getElementsRef().size(); i++) {
    std::unique_ptr<BoundExpression> boundExpression = nullptr;

    if (containerExpression->getElementsRef()[i]->getKind() ==
        SyntaxKindUtils::SyntaxKind::ContainerExpression) {
      boundExpression = std::move(bindContainerExpression(
          (ContainerExpressionSyntax *)containerExpression->getElementsRef()[i]
              .get()));
    } else {
      boundExpression = std::move(
          bindExpression(containerExpression->getElementsRef()[i].get()));
    }

    boundContainerExpression->setElement(std::move(boundExpression));
  }

  return std::move(boundContainerExpression);
}

std::unique_ptr<BoundExpression> Binder::bindBracketedExpression(
    BracketedExpressionSyntax *bracketedExpression) {

  SyntaxKindUtils::SyntaxKind kind =
      bracketedExpression->getExpressionRef()->getKind();

  std::unique_ptr<BoundBracketedExpression> boundBracketedExpression =
      std::make_unique<BoundBracketedExpression>(
          bracketedExpression->getSourceLocation());

  switch (kind) {
  case SyntaxKindUtils::SyntaxKind::ContainerExpression: {
    boundBracketedExpression->setExpression(std::move(bindContainerExpression(
        (ContainerExpressionSyntax *)bracketedExpression->getExpressionRef()
            .get())));
    break;
  }

  case SyntaxKindUtils::SyntaxKind::FillExpression: {
    boundBracketedExpression->setExpression(std::move(bindFillExpression(
        (FillExpressionSyntax *)bracketedExpression->getExpressionRef()
            .get())));
    break;
  }
  default: {
    this->_diagnosticHandler->addDiagnostic(Diagnostic(
        "Invalid Bracketed Expression", DiagnosticUtils::DiagnosticLevel::Error,
        DiagnosticUtils::DiagnosticType::Semantic,
        bracketedExpression->getSourceLocation()));
    break;
  }
  }

  return std::move(boundBracketedExpression);
}

std::unique_ptr<BoundStatement>
Binder::bindContainerStatement(ContainerStatementSyntax *containerSyntax) {

  std::string variable_str = std::any_cast<std::string>(
      containerSyntax->getIdentifierTokenRef()->getValue());

  bool isConst = containerSyntax->getIdentifierTokenRef()->getKind() ==
                 SyntaxKindUtils::SyntaxKind::ConstKeyword;

  if (!root->tryDeclareVariable(
          variable_str,
          Utils::Variable(nullptr, isConst, containerSyntax->getType()))) {

    this->_diagnosticHandler->addDiagnostic(
        Diagnostic("Container " + variable_str + " Already Exists",
                   DiagnosticUtils::DiagnosticLevel::Error,
                   DiagnosticUtils::DiagnosticType::Semantic,
                   Utils::getSourceLocation(
                       containerSyntax->getIdentifierTokenRef().get())));
  }

  std::unique_ptr<BoundContainerStatement> boundContainerStatement =
      std::make_unique<BoundContainerStatement>(
          containerSyntax->getSourceLocation(), containerSyntax->getType(),
          std::move(variable_str));

  for (const auto &exp : containerSyntax->getContainerSizeExpressionsRef()) {
    boundContainerStatement->addContainerSizeExpression(
        std::move(bindExpression(exp.get())));
  }

  SyntaxKindUtils::SyntaxKind containerExpressionKind =
      containerSyntax->getContainerExpressionRef()->getKind();

  // if (containerExpressionKind !=
  //     SyntaxKindUtils::SyntaxKind::BracketedExpression) {
  //   this->_diagnosticHandler->addDiagnostic(Diagnostic(
  //       "Invalid BracketedExpression Expression",
  //       DiagnosticUtils::DiagnosticLevel::Error,
  //       DiagnosticUtils::DiagnosticType::Semantic,
  //       containerSyntax->getContainerExpressionRef()->getSourceLocation()));

  //   return std::move(boundContainerStatement);
  // }

  boundContainerStatement->setRHSExpression(std::move(
      bindExpression(containerSyntax->getContainerExpressionRef().get())));

  return std::move(boundContainerStatement);
}

std::unique_ptr<BoundStatement>
Binder::bindBringStatement(BringStatementSyntax *bringStatement) {
  this->_diagnosticHandler->addParentDiagnostics(
      bringStatement->getDiagnosticHandlerPtr().get());

  if (!Utils::Node::isPathExists(bringStatement->getAbsoluteFilePath()) ||
      !Utils::Node::isPathAbsolute(bringStatement->getAbsoluteFilePath())) {

    bringStatement->getDiagnosticHandlerPtr()->addDiagnostic(Diagnostic(
        "File <" + bringStatement->getRelativeFilePathPtr() + "> not found",
        DiagnosticUtils::DiagnosticLevel::Error,
        DiagnosticUtils::DiagnosticType::Semantic,
        Utils::getSourceLocation(bringStatement->getBringKeywordPtr().get())));
    return std::make_unique<BoundBringStatement>(
        bringStatement->getSourceLocation(),
        bringStatement->getDiagnosticHandlerPtr().get(), nullptr);
  }

  if (Utils::Node::isCycleDetected(bringStatement->getAbsoluteFilePath())) {
    bringStatement->getDiagnosticHandlerPtr()->addDiagnostic(Diagnostic(
        "Found Circular Dependency <" +
            bringStatement->getRelativeFilePathPtr() + "> ",
        DiagnosticUtils::DiagnosticLevel::Error,
        DiagnosticUtils::DiagnosticType::Semantic,
        Utils::getSourceLocation(bringStatement->getBringKeywordPtr().get())));

    return std::make_unique<BoundBringStatement>(
        bringStatement->getSourceLocation(),
        bringStatement->getDiagnosticHandlerPtr().get(), nullptr);
  }
  if (Utils::Node::isPathVisited(bringStatement->getAbsoluteFilePathPtr())) {
    return std::make_unique<BoundBringStatement>(
        bringStatement->getSourceLocation(),
        bringStatement->getDiagnosticHandlerPtr().get(), nullptr);
  }

  Utils::Node::addPath(bringStatement->getAbsoluteFilePathPtr());

  if (bringStatement->getIsChoosyImportPtr()) {
    std::unordered_map<std::string, int> memberMap =
        getMemberMap(bringStatement->getCompilationUnitPtr()->getMembers(),
                     bringStatement->getCompilationUnitPtr().get());

    for (const auto &expression : bringStatement->getExpressionsPtr()) {
      if (expression->getKind() !=
          SyntaxKindUtils::SyntaxKind::IdentifierToken) {
        this->_diagnosticHandler->addDiagnostic(
            Diagnostic("Unexpected Token <" +
                           SyntaxKindUtils::to_string(expression->getKind()) +
                           ">, Expected <" +
                           SyntaxKindUtils::to_string(
                               SyntaxKindUtils::SyntaxKind::IdentifierToken) +
                           ">",
                       DiagnosticUtils::DiagnosticLevel::Error,
                       DiagnosticUtils::DiagnosticType::Syntactic,
                       (expression->getSourceLocation())));
        continue;
      }

      if (memberMap.find(expression->getText()) == memberMap.end()) {
        this->_diagnosticHandler->addDiagnostic(Diagnostic(
            "Identifier <" + expression->getText() + "> not found in <" +
                bringStatement->getRelativeFilePathPtr() + ">",
            DiagnosticUtils::DiagnosticLevel::Error,
            DiagnosticUtils::DiagnosticType::Syntactic,
            (expression->getSourceLocation())));
        continue;
      }
    }
  }
  Utils::Node::removePath(bringStatement->getAbsoluteFilePathPtr());
  std::unique_ptr<BoundScopeGlobal> globalScope =
      std::move(Binder::bindGlobalScope(
          nullptr, bringStatement->getCompilationUnitPtr().get(),
          bringStatement->getDiagnosticHandlerPtr().get()));

  for (auto &function : globalScope->functions) {
    if (!this->root->tryDeclareFunction(function.second)) {
      this->_diagnosticHandler->addDiagnostic(
          Diagnostic("Function " + function.first + " Already Declared",
                     DiagnosticUtils::DiagnosticLevel::Error,
                     DiagnosticUtils::DiagnosticType::Semantic,
                     Utils::getSourceLocation(
                         bringStatement->getBringKeywordPtr().get())));
    }
  }

  for (auto &variable : globalScope->variables) {
    if (!this->root->tryDeclareVariable(variable.first, variable.second)) {
      this->_diagnosticHandler->addDiagnostic(
          Diagnostic("Variable " + variable.first + " Already Declared",
                     DiagnosticUtils::DiagnosticLevel::Error,
                     DiagnosticUtils::DiagnosticType::Semantic,
                     Utils::getSourceLocation(
                         bringStatement->getBringKeywordPtr().get())));
    }
  }

  return std::make_unique<BoundBringStatement>(
      bringStatement->getSourceLocation(),
      bringStatement->getDiagnosticHandlerPtr().get(), std::move(globalScope));
}

std::unique_ptr<BoundLiteralExpression<std::any>>
Binder::bindLiteralExpression(ExpressionSyntax *syntax) {

  LiteralExpressionSyntax<std::any> *literalSyntax =
      static_cast<LiteralExpressionSyntax<std::any> *>(syntax);

  std::any value = literalSyntax->getValue();

  return std::make_unique<BoundLiteralExpression<std::any>>(
      literalSyntax->getSourceLocation(), value,
      literalSyntax->getSyntaxKind());
}

std::unique_ptr<BoundExpression>
Binder::bindIndexExpression(IndexExpressionSyntax *indexExpression) {

  std::string variableName = std::any_cast<std::string>(
      indexExpression->getIndexIdentifierExpressionPtr()->getValue());

  if (!root->tryLookupVariable(variableName)) {
    this->_diagnosticHandler->addDiagnostic(
        Diagnostic("Variable " + variableName + " Does Not Exist",
                   DiagnosticUtils::DiagnosticLevel::Error,
                   DiagnosticUtils::DiagnosticType::Semantic,
                   Utils::getSourceLocation(
                       indexExpression->getIndexIdentifierExpressionPtr()
                           ->getTokenPtr()
                           .get())));

    return std::move(bindExpression(
        indexExpression->getIndexIdentifierExpressionPtr().get()));
  }

  Utils::Variable variable = root->tryGetVariable(variableName);

  if (!Utils::isContainerType(variable.type)) {
    this->_diagnosticHandler->addDiagnostic(
        Diagnostic("Variable " + variableName + " is not a container",
                   DiagnosticUtils::DiagnosticLevel::Error,
                   DiagnosticUtils::DiagnosticType::Semantic,
                   Utils::getSourceLocation(
                       indexExpression->getIndexIdentifierExpressionPtr()
                           ->getTokenPtr()
                           .get())));
  }
  std::unique_ptr<BoundLiteralExpression<std::any>> boundIdentifierExpression(
      (BoundLiteralExpression<std::any> *)bindExpression(
          indexExpression->getIndexIdentifierExpressionPtr().get())
          .release());

  std::unique_ptr<BoundIndexExpression> boundIndexExp =
      std::make_unique<BoundIndexExpression>(
          indexExpression->getSourceLocation(),
          std::move(boundIdentifierExpression));

  for (const auto &indexExp : indexExpression->getIndexExpressionsRef()) {
    boundIndexExp->addBoundIndexExpression(
        std::move(bindExpression(indexExp.get())));
  }

  return std::move(boundIndexExp);
}

std::unique_ptr<BoundExpression>
Binder::bindunaryExpression(UnaryExpressionSyntax *unaryExpression) {
  std::unique_ptr<BoundExpression> boundOperand =
      std::move(bindExpression(unaryExpression->getOperandPtr().get()));
  BinderKindUtils::BoundUnaryOperatorKind op =
      BinderKindUtils::getUnaryOperatorKind(
          unaryExpression->getOperatorTokenPtr()->getKind());

  return std::make_unique<BoundUnaryExpression>(
      unaryExpression->getSourceLocation(), op, std::move(boundOperand));
}

std::unique_ptr<BoundExpression>
Binder::bindBinaryExpression(BinaryExpressionSyntax *binaryExpression) {
  std::unique_ptr<BoundExpression> boundLeft =
      std::move(bindExpression(binaryExpression->getLeftPtr().get()));
  std::unique_ptr<BoundExpression> boundRight =
      std::move(bindExpression(binaryExpression->getRightPtr().get()));
  BinderKindUtils::BoundBinaryOperatorKind op =
      BinderKindUtils::getBinaryOperatorKind(
          binaryExpression->getOperatorTokenPtr()->getKind());

  return std::make_unique<BoundBinaryExpression>(
      binaryExpression->getSourceLocation(), std::move(boundLeft), op,
      std::move(boundRight));
}

std::unique_ptr<BoundExpression> Binder::bindAssignmentExpression(
    AssignmentExpressionSyntax *assignmentExpression) {

  std::string variable_str = "";

  if (auto literalExpression =
          dynamic_cast<LiteralExpressionSyntax<std::any> *>(
              assignmentExpression->getLeftPtr().get())) {
    variable_str = std::any_cast<std::string>(literalExpression->getValue());
  } else if (auto indexExpression = dynamic_cast<IndexExpressionSyntax *>(
                 assignmentExpression->getLeftPtr().get())) {
    variable_str = std::any_cast<std::string>(
        indexExpression->getIndexIdentifierExpressionPtr()->getValue());
  } else {
    this->_diagnosticHandler->addDiagnostic(
        Diagnostic("Invalid Assignment Expression",
                   DiagnosticUtils::DiagnosticLevel::Error,
                   DiagnosticUtils::DiagnosticType::Semantic,
                   Utils::getSourceLocation(
                       assignmentExpression->getOperatorTokenPtr().get())));

    return std::move(bindExpression(assignmentExpression->getLeftPtr().get()));
  }

  std::unique_ptr<BoundExpression> boundIdentifierExpression(
      bindExpression(assignmentExpression->getLeftPtr().get()));

  BinderKindUtils::BoundBinaryOperatorKind op =
      BinderKindUtils::getBinaryOperatorKind(
          assignmentExpression->getOperatorTokenPtr()->getKind());

  if (!root->tryLookupVariable(variable_str)) {

    this->_diagnosticHandler->addDiagnostic(
        Diagnostic("Can Not Assign To Undeclared Variable " + variable_str,
                   DiagnosticUtils::DiagnosticLevel::Error,
                   DiagnosticUtils::DiagnosticType::Semantic,
                   Utils::getSourceLocation(
                       assignmentExpression->getOperatorTokenPtr().get())));

    return std::move(boundIdentifierExpression);
  }

  if (root->tryGetVariable(variable_str).isConst) {

    this->_diagnosticHandler->addDiagnostic(
        Diagnostic("Can Not Assign To Constant Variable " + variable_str,
                   DiagnosticUtils::DiagnosticLevel::Error,
                   DiagnosticUtils::DiagnosticType::Semantic,
                   Utils::getSourceLocation(
                       assignmentExpression->getOperatorTokenPtr().get())));

    return std::move(boundIdentifierExpression);
  }

  std::unique_ptr<BoundExpression> boundRight =
      bindExpression(assignmentExpression->getRightPtr().get());
  return std::make_unique<BoundAssignmentExpression>(
      assignmentExpression->getSourceLocation(),
      root->tryGetVariable(variable_str), std::move(boundIdentifierExpression),
      op, std::move(boundRight));
}

std::unique_ptr<BoundExpression>
Binder::bindCallExpression(CallExpressionSyntax *callExpression) {
  std::unique_ptr<BoundLiteralExpression<std::any>> boundIdentifier(
      (BoundLiteralExpression<std::any> *)bindExpression(
          (callExpression->getIdentifierPtr().get()))
          .release());

  const std::string functionName =
      std::any_cast<std::string>(boundIdentifier->getValue());

  if (BuiltInFunction::isBuiltInFunction(functionName)) {

    const std::unique_ptr<BoundFunctionDeclaration> &boundBuiltinFunction =
        BuiltInFunction::getBuiltInFunction(functionName);

    if (callExpression->getArguments().size() !=
        boundBuiltinFunction->getParametersRef().size()) {

      this->_diagnosticHandler->addDiagnostic(Diagnostic(
          "Function " + boundBuiltinFunction->getFunctionNameRef() +
              " requires " +
              std::to_string(boundBuiltinFunction->getParametersRef().size()) +
              " arguments but " +
              std::to_string(callExpression->getArguments().size()) +
              " arguments were given",
          DiagnosticUtils::DiagnosticLevel::Error,
          DiagnosticUtils::DiagnosticType::Semantic,
          Utils::getSourceLocation(
              callExpression->getIdentifierPtr()->getTokenPtr().get())));
    }
  }

  std::unique_ptr<BoundCallExpression> boundCallExpression =
      std::make_unique<BoundCallExpression>(
          callExpression->getSourceLocation());

  boundCallExpression->setCallerIdentifier(std::move(boundIdentifier));

  for (int i = 0; i < callExpression->getArguments().size(); i++) {
    boundCallExpression->addArgument(
        std::move(bindExpression(callExpression->getArguments()[i].get())));
  }

  this->_callExpressions.push_back(boundCallExpression.get());
  return std::move(boundCallExpression);
}

std::unique_ptr<BoundExpression>
Binder::bindFillExpression(FillExpressionSyntax *fillExpression) {

  std::unique_ptr<BoundFillExpression> boundFillExpression =
      std::make_unique<BoundFillExpression>(
          fillExpression->getSourceLocation());

  if (fillExpression->getSizeToFillExpressionRef().get()) {
    boundFillExpression->setSizeToFill(std::move(
        bindExpression(fillExpression->getSizeToFillExpressionRef().get())));
  }

  boundFillExpression->setElementToFill(std::move(
      bindExpression(fillExpression->getElementExpressionRef().get())));

  return std::move(boundFillExpression);
}

std::unique_ptr<BoundExpression>
Binder::bindExpression(ExpressionSyntax *syntax) {

  switch (syntax->getKind()) {
  case SyntaxKindUtils::SyntaxKind::LiteralExpression: {

    return std::move(
        bindLiteralExpression((LiteralExpressionSyntax<std::any> *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::UnaryExpression: {
    return std::move(bindunaryExpression((UnaryExpressionSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::BinaryExpression: {
    return std::move(bindBinaryExpression((BinaryExpressionSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::AssignmentExpression: {
    return std::move(
        bindAssignmentExpression((AssignmentExpressionSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::VariableExpressionSyntax: {
    return std::move(
        bindVariableExpression((VariableExpressionSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::ArrayVariableExpressionSyntax: {
    return std::move(
        bindArrayVariableExpression((ArrayVariableExpressionSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::ParenthesizedExpression: {
    ParenthesizedExpressionSyntax *parenthesizedExpression =
        (ParenthesizedExpressionSyntax *)syntax;
    return bindExpression(parenthesizedExpression->getExpression().get());
  }
  case SyntaxKindUtils::SyntaxKind::CallExpression: {
    return std::move(bindCallExpression((CallExpressionSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::IndexExpression: {
    return std::move(bindIndexExpression((IndexExpressionSyntax *)syntax));
  }
  case SyntaxKindUtils::SyntaxKind::BracketedExpression: {
    return std::move(
        bindBracketedExpression((BracketedExpressionSyntax *)syntax));
  }

  default:
    throw "Unexpected syntax";
  }
  return nullptr;
}

std::unique_ptr<BoundVariableExpression> Binder::bindVariableExpression(
    VariableExpressionSyntax *variableExpressionSyntax) {
  std::string variable_str = variableExpressionSyntax->getVariableName();

  return std::make_unique<BoundVariableExpression>(
      variableExpressionSyntax->getSourceLocation(),
      std::move(bindLiteralExpression(
          variableExpressionSyntax->getIdentifierTokenRef().get())),
      variableExpressionSyntax->isConstant(),
      variableExpressionSyntax->getVariableType());
}

std::unique_ptr<BoundVariableExpression>
Binder::bindArrayVariableExpression(ArrayVariableExpressionSyntax *arrayExpr) {

  std::string variable_str = arrayExpr->getVariableName();

  std::unique_ptr<BoundArrayVariableExpression> boundArrayExp =
      std::make_unique<BoundArrayVariableExpression>(
          arrayExpr->getSourceLocation(),
          std::move(
              bindLiteralExpression(arrayExpr->getIdentifierTokenRef().get())),
          arrayExpr->isConstant(), arrayExpr->getVariableType());

  for (const auto &size : arrayExpr->getSizeExpresionsRef()) {

    boundArrayExp->addSizeExpression(
        std::move(bindLiteralExpression(size.get())));
  }

  return std::move(boundArrayExp);
}

std::unique_ptr<BoundStatement>
Binder::bindFunctionDeclaration(FunctionDeclarationSyntax *syntax) {

  this->root = std::make_unique<BoundScope>(std::move(this->root));

  std::string function_name = syntax->getIdentifierTokenPtr()->getText();

  if (BuiltInFunction::isBuiltInFunction(function_name)) {
    this->_diagnosticHandler->addDiagnostic(Diagnostic(
        "Function " + function_name + " Already Exists",
        DiagnosticUtils::DiagnosticLevel::Error,
        DiagnosticUtils::DiagnosticType::Semantic,
        Utils::getSourceLocation(syntax->getIdentifierTokenPtr().get())));
  }

  std::unique_ptr<BoundFunctionDeclaration> fd =
      std::make_unique<BoundFunctionDeclaration>(syntax->getSourceLocation());

  fd->setFunctionName(function_name);
  for (int i = 0; i < syntax->getParametersPtr().size(); i++) {
    const std::string &variable_str =
        syntax->getParametersPtr()[i]->getVariableName();

    if (!this->root->tryDeclareVariable(
            variable_str,
            Utils::Variable(
                nullptr, false,
                syntax->getParametersPtr()[i]->getVariableType()))) {

      this->_diagnosticHandler->addDiagnostic(
          Diagnostic("Parameter " + variable_str + " Already Declared",
                     DiagnosticUtils::DiagnosticLevel::Error,
                     DiagnosticUtils::DiagnosticType::Semantic,
                     syntax->getParametersPtr()[i]->getSourceLocation()));
    }

    if (syntax->getParametersPtr()[i]->getKind() ==
        SyntaxKindUtils::SyntaxKind::ArrayVariableExpressionSyntax) {

      fd->addParameter(std::move(this->bindArrayVariableExpression(
          static_cast<ArrayVariableExpressionSyntax *>(
              syntax->getParametersPtr()[i].release()))));
    } else {
      fd->addParameter(std::move(this->bindVariableExpression(
          syntax->getParametersPtr()[i].release())));
    }
  }
  fd->setReturnType(std::move(bindTypeExpression(
      (TypeExpressionSyntax *)syntax->getReturnExpression().get())));

  this->root->incrementFunctionCount();

  std::unique_ptr<BoundBlockStatement> boundBody(
      (BoundBlockStatement *)bindStatement(syntax->getBodyPtr().get())
          .release());

  fd->setFunctionBody(std::move(boundBody));

  this->root->decrementFunctionCount();

  if (!this->root->tryDeclareFunction(fd.get())) {

    this->_diagnosticHandler->addDiagnostic(Diagnostic(
        "Function " + function_name + " Already Declared",
        DiagnosticUtils::DiagnosticLevel::Error,
        DiagnosticUtils::DiagnosticType::Semantic,
        Utils::getSourceLocation(syntax->getFunctionKeywordPtr().get())));
  }

  this->root = std::move(this->root->parent);
  return std::move(fd);
}

std::unique_ptr<BoundExpression>
Binder::bindTypeExpression(TypeExpressionSyntax *typeExpressionSyntax) {
  Utils::type type = typeExpressionSyntax->getType();
  switch (typeExpressionSyntax->getKind()) {
  case SyntaxKindUtils::SyntaxKind::ArrayTypeExpression: {
    std::unique_ptr<BoundArrayTypeExpression> boundArrayTypeExpression =
        std::make_unique<BoundArrayTypeExpression>(
            typeExpressionSyntax->getSourceLocation(), type);

    for (const auto &size :
         ((ArrayTypeExpressionSyntax *)typeExpressionSyntax)->getDimensions()) {
      boundArrayTypeExpression->addDimension(
          std::move(bindExpression(size.get())));
    }

    return std::move(boundArrayTypeExpression);
  }
  case SyntaxKindUtils::SyntaxKind::PrimitiveTypeExpression: {
    return std::move(std::make_unique<BoundTypeExpression>(
        typeExpressionSyntax->getSourceLocation(), type));
  }
  default: {
    break;
  }
  }

  this->_diagnosticHandler->addDiagnostic(Diagnostic(
      "Invalid Type Expression", DiagnosticUtils::DiagnosticLevel::Error,
      DiagnosticUtils::DiagnosticType::Semantic,
      typeExpressionSyntax->getSourceLocation()));

  return nullptr;
}

std::unique_ptr<BoundStatement>
Binder::bindGlobalStatement(GlobalStatementSyntax *syntax) {
  return bindStatement(syntax->getStatementPtr().get());
}

Binder::Binder(std::unique_ptr<BoundScope> parent,
               DiagnosticHandler *diagnosticHandler) {

  BuiltInFunction::setupBuiltInFunctions();

  this->root = std::make_unique<BoundScope>(std::move(parent));
  this->_diagnosticHandler = diagnosticHandler;
}

void Binder::verifyAllCallsAreValid(Binder *binder) {

  std::vector<BoundFunctionDeclaration *> functions =
      binder->root->getAllFunctions();

  std::unordered_map<std::string, BoundFunctionDeclaration *>
      functionDefinitionMap;

  for (const auto &function : functions) {
    functionDefinitionMap[function->getFunctionNameRef()] = function;
  }

  for (const auto &[name, function] : binder->dependencyFunctions) {
    functionDefinitionMap[name] = function;
  }

  for (const auto &function : BuiltInFunction::getBuiltInFunctions()) {
    functionDefinitionMap[function->getFunctionNameRef()] = function.get();
  }

  for (BoundCallExpression *callExpression : binder->_callExpressions) {

    if (functionDefinitionMap.find(callExpression->getCallerNameRef()) ==
        functionDefinitionMap.end()) {

      binder->_diagnosticHandler->addDiagnostic(Diagnostic(
          "Function " + callExpression->getCallerNameRef() + " does not exist",
          DiagnosticUtils::DiagnosticLevel::Error,
          DiagnosticUtils::DiagnosticType::Semantic,
          callExpression->getCallerIdentifierPtr()->getLocation()));

      continue;
    }

    BoundFunctionDeclaration *fd =
        functionDefinitionMap.at(callExpression->getCallerNameRef());

    if (callExpression->getArgumentsRef().size() !=
        fd->getParametersRef().size()) {

      binder->_diagnosticHandler->addDiagnostic(Diagnostic(
          "Function " + callExpression->getCallerNameRef() + " requires " +
              std::to_string(fd->getParametersRef().size()) + " arguments",
          DiagnosticUtils::DiagnosticLevel::Error,
          DiagnosticUtils::DiagnosticType::Semantic,
          callExpression->getCallerIdentifierPtr()->getLocation()));

      continue;
    }
  }
}

std::unique_ptr<BoundScopeGlobal>
Binder::bindGlobalScope(std::unique_ptr<BoundScopeGlobal> previousGlobalScope,
                        CompilationUnitSyntax *syntax,
                        DiagnosticHandler *diagnosticHandler) {

  std::unique_ptr<Binder> binder =
      std::make_unique<Binder>(nullptr, diagnosticHandler);

  if (previousGlobalScope) {
    binder->root->variables = previousGlobalScope->variables;
    binder->root->functions = previousGlobalScope->functions;
  }

  std::unique_ptr<BoundBlockStatement> _globalBoundBlockStatement =
      std::make_unique<BoundBlockStatement>(DiagnosticUtils::SourceLocation(),
                                            true);

  std::vector<std::unique_ptr<MemberSyntax>> &members = syntax->getMembers();

  for (int i = 0; i < members.size(); i++) {
    switch (members[i].get()->getKind()) {
    case SyntaxKindUtils::SyntaxKind::FunctionDeclarationSyntax: {

      std::unique_ptr<BoundStatement> _statement =
          std::move(binder->bindFunctionDeclaration(
              (FunctionDeclarationSyntax *)members[i].get()));
      _globalBoundBlockStatement->addStatement(std::move(_statement));
      break;
    }
    case SyntaxKindUtils::SyntaxKind::GlobalStatement: {

      std::unique_ptr<BoundStatement> _statement =
          std::move(binder->bindGlobalStatement(
              (GlobalStatementSyntax *)members[i].get()));

      _globalBoundBlockStatement->addStatement(std::move(_statement));
      break;
    }
    default:
      throw "Unexpected global member";
      break;
    }
  }

  verifyAllCallsAreValid(binder.get());

  // if (previousGlobalScope != nullptr) {
  //   diagnosticHandler->addParentDiagnostics(
  //       previousGlobalScope->_diagnosticHandler);
  // }

  return std::make_unique<BoundScopeGlobal>(
      std::move(previousGlobalScope), binder->root->variables,
      binder->root->functions, diagnosticHandler,
      std::move(_globalBoundBlockStatement));
}

// Utils

auto Binder::getMemberMap(
    const std::vector<std::unique_ptr<MemberSyntax>> &members,
    CompilationUnitSyntax *nestedCompilationUnit)
    -> std::unordered_map<std::string, int> {

  std::unordered_map<std::string, int> memberMap;

  for (std::unique_ptr<MemberSyntax> &member :
       nestedCompilationUnit->getMembers()) {
    if (member->getKind() ==
        SyntaxKindUtils::SyntaxKind::FunctionDeclarationSyntax) {
      FunctionDeclarationSyntax *functionDeclaration =
          dynamic_cast<FunctionDeclarationSyntax *>(member.get());
      memberMap[functionDeclaration->getIdentifierTokenPtr()->getText()] = 1;
    } else if (member->getKind() ==
               SyntaxKindUtils::SyntaxKind::GlobalStatement) {
      GlobalStatementSyntax *globalStatement =
          dynamic_cast<GlobalStatementSyntax *>(member.get());
      if (globalStatement->getStatementPtr()->getKind() ==
          SyntaxKindUtils::SyntaxKind::VariableDeclaration) {
        VariableDeclarationSyntax *variableDeclaration =
            dynamic_cast<VariableDeclarationSyntax *>(
                globalStatement->getStatementPtr().get());
        memberMap[variableDeclaration->getIdentifierPtr()->getText()] = 1;
      } else if (globalStatement->getStatementPtr()->getKind() ==
                 SyntaxKindUtils::SyntaxKind::ContainerStatement) {
        ContainerStatementSyntax *containerDeclaration =
            dynamic_cast<ContainerStatementSyntax *>(
                globalStatement->getStatementPtr().get());
        memberMap[containerDeclaration->getIdentifierTokenRef()->getText()] = 1;
      }
    }
  }

  return memberMap;
}
