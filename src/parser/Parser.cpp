#include "Parser.h"

Parser::Parser(std::vector<std::string> text) {
  Lexer *lexer = new Lexer(text);
  std::shared_ptr<SyntaxToken<std::any>> token;

  if (lexer->logs.size()) {
    this->logs = lexer->logs;
  }
  do {
    token = (lexer->nextToken());
    if (token->getKind() != SyntaxKindUtils::SyntaxKind::WhitespaceToken &&
        token->getKind() != SyntaxKindUtils::SyntaxKind::EndOfLineToken) {
      this->tokens.push_back((token));
    }
    if (token->getKind() == SyntaxKindUtils::SyntaxKind::BadToken) {
      this->logs.push_back(
          Utils::getLineNumberAndPosition(
              std::make_shared<SyntaxToken<std::any>>(*token)) +
          "ERROR: unexpected character <" + token->getText() + ">");
    }
  } while (token->getKind() != SyntaxKindUtils::SyntaxKind::EndOfFileToken);
}

std::shared_ptr<SyntaxToken<std::any>> Parser::peek(int offset) {
  int index = this->position + offset;
  if (index >= this->tokens.size()) {

    return std::make_shared<SyntaxToken<std::any>>(
        *this->tokens[this->tokens.size() - 1]);
  }
  return std::make_shared<SyntaxToken<std::any>>(*this->tokens[index]);
}

std::shared_ptr<SyntaxToken<std::any>> Parser::getCurrent() {
  return this->peek(0);
}

std::shared_ptr<SyntaxToken<std::any>> Parser::nextToken() {
  std::shared_ptr<SyntaxToken<std::any>> current = this->getCurrent();
  this->position++;
  return current;
}

std::shared_ptr<SyntaxToken<std::any>>
Parser::match(SyntaxKindUtils::SyntaxKind kind) {
  if (this->getCurrent()->getKind() == kind) {
    return this->nextToken();
  }
  this->logs.push_back(
      Utils::getLineNumberAndPosition(
          std::make_shared<SyntaxToken<std::any>>(*this->getCurrent())) +
      "ERROR: unexpected token <" + this->getCurrent()->getText() +
      ">, expected <" + SyntaxKindUtils::enum_to_string_map[kind] + ">");

  return std::make_shared<SyntaxToken<std::any>>(
      this->getCurrent()->getLineNumber(),
      SyntaxKindUtils::SyntaxKind::EndOfFileToken,
      this->getCurrent()->getPosition(), "\0", 0);
}

CompilationUnitSyntax *Parser::parseCompilationUnit() {
  StatementSyntax *statement = this->parseStatement();
  std::shared_ptr<SyntaxToken<std::any>> endOfFileToken =
      this->match(SyntaxKindUtils::SyntaxKind::EndOfFileToken);
  return new CompilationUnitSyntax(this->logs, statement, (endOfFileToken));
}

BlockStatementSyntax *Parser::parseBlockStatement() {
  std::shared_ptr<SyntaxToken<std::any>> openBraceToken =
      this->match(SyntaxKindUtils::SyntaxKind::OpenBraceToken);

  std::vector<StatementSyntax *> statements;
  while (this->getCurrent()->getKind() !=
             SyntaxKindUtils::SyntaxKind::CloseBraceToken &&

         this->getCurrent()->getKind() !=
             SyntaxKindUtils::SyntaxKind::EndOfFileToken

  ) {
    StatementSyntax *statement = this->parseStatement();
    statements.push_back(statement);
  }

  std::shared_ptr<SyntaxToken<std::any>> closeBraceToken =
      this->match(SyntaxKindUtils::SyntaxKind::CloseBraceToken);

  return new BlockStatementSyntax((openBraceToken), statements,
                                  (closeBraceToken));
}

ExpressionStatementSyntax *Parser::parseExpressionStatement() {
  ExpressionSyntax *expression = this->parseExpression();
  return new ExpressionStatementSyntax(expression);
}
StatementSyntax *Parser::parseStatement() {
  switch (this->getCurrent()->getKind()) {
  case SyntaxKindUtils::SyntaxKind::OpenBraceToken:
    return (StatementSyntax *)this->parseBlockStatement();
  case SyntaxKindUtils::SyntaxKind::VarKeyword:
  case SyntaxKindUtils::SyntaxKind::ConstKeyword:
    return (StatementSyntax *)this->parseVariableDeclaration();
  case SyntaxKindUtils::SyntaxKind::IfKeyword:
    return (StatementSyntax *)this->parseIfStatement();
  case SyntaxKindUtils::SyntaxKind::WhileKeyword:
    return (StatementSyntax *)this->parseWhileStatement();

  case SyntaxKindUtils::SyntaxKind::ForKeyword:
    return (StatementSyntax *)this->parseForStatement();
  case SyntaxKindUtils::SyntaxKind::EndOfLineToken:
  case SyntaxKindUtils::SyntaxKind::EndOfFileToken:
    return (StatementSyntax *)(new SyntaxToken<std::any>(*this->nextToken()));
  default:
    return (StatementSyntax *)this->parseExpressionStatement();
  }
}

ForStatementSyntax *Parser::parseForStatement() {
  std::shared_ptr<SyntaxToken<std::any>> keyword =
      this->match(SyntaxKindUtils::SyntaxKind::ForKeyword);
  bool hadOpenParenthesis = false;
  if (this->getCurrent()->getKind() ==
      SyntaxKindUtils::SyntaxKind::OpenParenthesisToken) {
    this->match(SyntaxKindUtils::SyntaxKind::OpenParenthesisToken);
    hadOpenParenthesis = true;
  }

  StatementSyntax *statementSyntax = nullptr;

  if (this->getCurrent()->getKind() ==
      SyntaxKindUtils::SyntaxKind::VarKeyword) {
    statementSyntax = this->parseVariableDeclaration();
  } else {
    statementSyntax = this->parseExpressionStatement();
  }

  std::shared_ptr<SyntaxToken<std::any>> toKeyword =
      this->match(SyntaxKindUtils::SyntaxKind::ToKeyword);

  ExpressionSyntax *upperBound = this->parseExpression();

  if (hadOpenParenthesis) {
    this->match(SyntaxKindUtils::SyntaxKind::CloseParenthesisToken);
  }

  BlockStatementSyntax *statement = this->parseBlockStatement();
  return new ForStatementSyntax(statementSyntax, upperBound, statement);
}

IfStatementSyntax *Parser::parseIfStatement() {
  std::shared_ptr<SyntaxToken<std::any>> keyword =
      this->match(SyntaxKindUtils::SyntaxKind::IfKeyword);
  ExpressionSyntax *condition = this->parseExpression();

  BlockStatementSyntax *statement = this->parseBlockStatement();
  ElseClauseSyntax *elseClause = nullptr;
  if (this->getCurrent()->getKind() ==
      SyntaxKindUtils::SyntaxKind::ElseKeyword) {
    std::shared_ptr<SyntaxToken<std::any>> elseKeyword =
        this->match(SyntaxKindUtils::SyntaxKind::ElseKeyword);
    BlockStatementSyntax *elseStatement = this->parseBlockStatement();
    elseClause = new ElseClauseSyntax((elseKeyword), (elseStatement));
  }
  return new IfStatementSyntax((keyword), condition, statement, elseClause);
}

WhileStatementSyntax *Parser::parseWhileStatement() {
  std::shared_ptr<SyntaxToken<std::any>> keyword =
      this->match(SyntaxKindUtils::SyntaxKind::WhileKeyword);
  ExpressionSyntax *condition = this->parseExpression();
  BlockStatementSyntax *statement = this->parseBlockStatement();
  return new WhileStatementSyntax((keyword), condition, statement);
}

StatementSyntax *Parser::parseVariableDeclaration() {

  std::shared_ptr<SyntaxToken<std::any>> keyword = this->match(
      SyntaxKindUtils::SyntaxKind::VarKeyword == this->getCurrent()->getKind()
          ? SyntaxKindUtils::SyntaxKind::VarKeyword
          : SyntaxKindUtils::SyntaxKind::ConstKeyword);
  std::shared_ptr<SyntaxToken<std::any>> identifier =
      this->match(SyntaxKindUtils::SyntaxKind::IdentifierToken);
  std::shared_ptr<SyntaxToken<std::any>> equalsToken =
      this->match(SyntaxKindUtils::SyntaxKind::EqualsToken);
  ExpressionSyntax *initializer = this->parseExpression();
  return new VariableDeclarationSyntax((keyword), (identifier), (equalsToken),
                                       initializer);
}

ExpressionSyntax *Parser::parseExpression(int parentPrecedence) {

  ExpressionSyntax *left;
  int unaryOperatorPrecedence =
      this->getCurrent()->getUnaryOperatorPrecedence();

  if (unaryOperatorPrecedence != 0 &&
      unaryOperatorPrecedence >= parentPrecedence) {
    std::shared_ptr<SyntaxToken<std::any>> operatorToken = this->nextToken();
    ExpressionSyntax *operand = this->parseExpression(unaryOperatorPrecedence);
    left = new UnaryExpressionSyntax((operatorToken), operand);
  } else {
    left = this->parsePrimaryExpression();
  }

  while (true) {
    int precedence = this->getCurrent()->getBinaryOperatorPrecedence();
    if (precedence == 0 || precedence <= parentPrecedence) {
      break;
    }
    std::shared_ptr<SyntaxToken<std::any>> operatorToken = this->nextToken();

    ExpressionSyntax *right = this->parseExpression(precedence);

    left = new BinaryExpressionSyntax(left, (operatorToken), right);
  }

  return left;
}

ExpressionSyntax *Parser::parsePrimaryExpression() {
  switch (this->getCurrent()->getKind()) {
  case SyntaxKindUtils::OpenParenthesisToken: {
    std::shared_ptr<SyntaxToken<std::any>> left = this->nextToken();
    ExpressionSyntax *expression = this->parseExpression();
    std::shared_ptr<SyntaxToken<std::any>> right =
        this->match(SyntaxKindUtils::SyntaxKind::CloseParenthesisToken);
    return new ParenthesizedExpressionSyntax((left), expression, (right));
  }
  case SyntaxKindUtils::NumberToken: {
    std::shared_ptr<SyntaxToken<std::any>> numberToken = this->nextToken();

    return new LiteralExpressionSyntax<std::any>((numberToken),
                                                 (numberToken->getValue()));
  }

  case SyntaxKindUtils::StringToken: {
    std::shared_ptr<SyntaxToken<std::any>> stringToken = this->nextToken();
    return new LiteralExpressionSyntax<std::any>((stringToken),
                                                 stringToken->getValue());
  }
  case SyntaxKindUtils::TrueKeyword:
  case SyntaxKindUtils::FalseKeyword: {
    std::shared_ptr<SyntaxToken<std::any>> keywordToken = this->nextToken();
    bool value =
        keywordToken->getKind() == SyntaxKindUtils::SyntaxKind::TrueKeyword
            ? true
            : false;
    return new LiteralExpressionSyntax<std::any>((keywordToken), value);
  }
  case SyntaxKindUtils::SyntaxKind::CommaToken: {
    return new LiteralExpressionSyntax<std::any>(this->nextToken(), ",");
  }
  case SyntaxKindUtils::SyntaxKind::IdentifierToken: {
    return this->parseNameorCallExpression();
  }
  default:
    this->logs.push_back(
        Utils::getLineNumberAndPosition(
            std::make_shared<SyntaxToken<std::any>>(*this->getCurrent())) +
        "ERROR: unexpected token <" + this->getCurrent()->getText() + ">");
    return new LiteralExpressionSyntax<std::any>(this->getCurrent(), (int)0);
  }
}

ExpressionSyntax *Parser::parseNameorCallExpression() {
  if (this->peek(1)->getKind() == SyntaxKindUtils::SyntaxKind::EqualsToken) {
    std::shared_ptr<SyntaxToken<std::any>> identifierToken = this->nextToken();
    std::shared_ptr<SyntaxToken<std::any>> operatorToken = this->nextToken();
    ExpressionSyntax *right = this->parseExpression();
    return new AssignmentExpressionSyntax(
        new LiteralExpressionSyntax<std::any>((identifierToken),
                                              identifierToken->getValue()),
        (operatorToken), right);
  } else if (this->peek(1)->getKind() ==
             SyntaxKindUtils::SyntaxKind::OpenParenthesisToken) {
    std::shared_ptr<SyntaxToken<std::any>> identifierToken =
        this->match(SyntaxKindUtils::SyntaxKind::IdentifierToken);
    std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken =
        this->match(SyntaxKindUtils::SyntaxKind::OpenParenthesisToken);
    std::vector<ExpressionSyntax *> arguments;
    while (this->getCurrent()->getKind() !=
               SyntaxKindUtils::SyntaxKind::CloseParenthesisToken &&
           this->getCurrent()->getKind() !=
               SyntaxKindUtils::SyntaxKind::EndOfFileToken) {
      ExpressionSyntax *expression = this->parseExpression();
      arguments.push_back(expression);
      if (this->getCurrent()->getKind() !=
          SyntaxKindUtils::SyntaxKind::CloseParenthesisToken) {
        this->match(SyntaxKindUtils::SyntaxKind::CommaToken);
      }
    }

    std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken =
        this->match(SyntaxKindUtils::SyntaxKind::CloseParenthesisToken);
    return new CallExpressionSyntax(
        new LiteralExpressionSyntax<std::any>((identifierToken),
                                              identifierToken->getValue()),
        (openParenthesisToken), arguments, (closeParenthesisToken));
  } else {
    std::shared_ptr<SyntaxToken<std::any>> identifierToken = this->nextToken();
    return new VariableExpressionSyntax(new LiteralExpressionSyntax<std::any>(
        (identifierToken), identifierToken->getValue()));
  }
}
