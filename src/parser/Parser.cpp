#include "Parser.h"

Parser::Parser(std::vector<std::string> text) {
  std::shared_ptr<Lexer> lexer = std::make_shared<Lexer>(text);
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
      this->logs.push_back(Utils::getLineNumberAndPosition(token) +
                           "ERROR: unexpected character <" + token->getText() +
                           ">");
    }
  } while (token->getKind() != SyntaxKindUtils::SyntaxKind::EndOfFileToken);
}

std::shared_ptr<SyntaxToken<std::any>> Parser::peek(int offset) {
  int index = this->position + offset;
  if (index >= this->tokens.size()) {

    return this->tokens[this->tokens.size() - 1];
  }
  return this->tokens[index];
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
  this->logs.push_back(Utils::getLineNumberAndPosition(this->getCurrent()) +
                       "ERROR: unexpected token <" +
                       this->getCurrent()->getText() + ">, expected <" +
                       SyntaxKindUtils::enum_to_string_map[kind] + ">");

  return std::make_shared<SyntaxToken<std::any>>(
      this->getCurrent()->getLineNumber(),
      SyntaxKindUtils::SyntaxKind::EndOfFileToken,
      this->getCurrent()->getPosition(), "\0", 0);
}

std::shared_ptr<CompilationUnitSyntax> Parser::parseCompilationUnit() {
  std::shared_ptr<StatementSyntax> statement = this->parseStatement();
  std::shared_ptr<SyntaxToken<std::any>> endOfFileToken =
      this->match(SyntaxKindUtils::SyntaxKind::EndOfFileToken);
  return std::make_shared<CompilationUnitSyntax>(this->logs, statement,
                                                 std::move(endOfFileToken));
}

std::shared_ptr<BlockStatementSyntax> Parser::parseBlockStatement() {
  std::shared_ptr<SyntaxToken<std::any>> openBraceToken =
      this->match(SyntaxKindUtils::SyntaxKind::OpenBraceToken);

  std::vector<std::shared_ptr<StatementSyntax>> statements;
  while (this->getCurrent()->getKind() !=
             SyntaxKindUtils::SyntaxKind::CloseBraceToken &&

         this->getCurrent()->getKind() !=
             SyntaxKindUtils::SyntaxKind::EndOfFileToken

  ) {
    std::shared_ptr<StatementSyntax> statement = this->parseStatement();
    statements.push_back(statement);
  }

  std::shared_ptr<SyntaxToken<std::any>> closeBraceToken =
      this->match(SyntaxKindUtils::SyntaxKind::CloseBraceToken);

  return std::make_shared<BlockStatementSyntax>(std::move(openBraceToken),
                                                std::move(statements),
                                                std::move(closeBraceToken));
}

std::shared_ptr<ExpressionStatementSyntax> Parser::parseExpressionStatement() {
  std::shared_ptr<ExpressionSyntax> expression = this->parseExpression();
  return std::make_shared<ExpressionStatementSyntax>(expression);
}
std::shared_ptr<StatementSyntax> Parser::parseStatement() {
  switch (this->getCurrent()->getKind()) {
  case SyntaxKindUtils::SyntaxKind::OpenBraceToken:
    return std::dynamic_pointer_cast<StatementSyntax>(
        this->parseBlockStatement());
  case SyntaxKindUtils::SyntaxKind::VarKeyword:
  case SyntaxKindUtils::SyntaxKind::ConstKeyword:
    return (std::shared_ptr<StatementSyntax>)this->parseVariableDeclaration();
  case SyntaxKindUtils::SyntaxKind::IfKeyword:
    return this->parseIfStatement();
  case SyntaxKindUtils::SyntaxKind::WhileKeyword:
    return this->parseWhileStatement();

  case SyntaxKindUtils::SyntaxKind::ForKeyword:
    return this->parseForStatement();
  case SyntaxKindUtils::SyntaxKind::EndOfLineToken:
  case SyntaxKindUtils::SyntaxKind::EndOfFileToken:
    return std::dynamic_pointer_cast<StatementSyntax>(this->nextToken());
  default:
    return this->parseExpressionStatement();
  }
}

std::shared_ptr<ForStatementSyntax> Parser::parseForStatement() {
  std::shared_ptr<SyntaxToken<std::any>> keyword =
      this->match(SyntaxKindUtils::SyntaxKind::ForKeyword);
  bool hadOpenParenthesis = false;
  if (this->getCurrent()->getKind() ==
      SyntaxKindUtils::SyntaxKind::OpenParenthesisToken) {
    this->match(SyntaxKindUtils::SyntaxKind::OpenParenthesisToken);
    hadOpenParenthesis = true;
  }

  std::shared_ptr<StatementSyntax> statementSyntax = nullptr;

  if (this->getCurrent()->getKind() ==
      SyntaxKindUtils::SyntaxKind::VarKeyword) {
    statementSyntax = this->parseVariableDeclaration();
  } else {
    statementSyntax = this->parseExpressionStatement();
  }

  std::shared_ptr<SyntaxToken<std::any>> toKeyword =
      this->match(SyntaxKindUtils::SyntaxKind::ToKeyword);

  std::shared_ptr<ExpressionSyntax> upperBound = this->parseExpression();

  if (hadOpenParenthesis) {
    this->match(SyntaxKindUtils::SyntaxKind::CloseParenthesisToken);
  }

  std::shared_ptr<BlockStatementSyntax> statement = this->parseBlockStatement();
  return std::make_shared<ForStatementSyntax>(
      std::move(statementSyntax), std::move(upperBound), std::move(statement));
}

std::shared_ptr<IfStatementSyntax> Parser::parseIfStatement() {
  std::shared_ptr<SyntaxToken<std::any>> keyword =
      this->match(SyntaxKindUtils::SyntaxKind::IfKeyword);
  std::shared_ptr<ExpressionSyntax> condition = this->parseExpression();

  std::shared_ptr<BlockStatementSyntax> statement = this->parseBlockStatement();
  std::shared_ptr<ElseClauseSyntax> elseClause = nullptr;
  if (this->getCurrent()->getKind() ==
      SyntaxKindUtils::SyntaxKind::ElseKeyword) {
    std::shared_ptr<SyntaxToken<std::any>> elseKeyword =
        this->match(SyntaxKindUtils::SyntaxKind::ElseKeyword);
    std::shared_ptr<BlockStatementSyntax> elseStatement =
        this->parseBlockStatement();
    elseClause =
        std::make_shared<ElseClauseSyntax>((elseKeyword), (elseStatement));
  }
  return std::make_shared<IfStatementSyntax>(
      std::move(keyword), std::move(condition), std::move(statement),
      std::move(elseClause));
}

std::shared_ptr<WhileStatementSyntax> Parser::parseWhileStatement() {
  std::shared_ptr<SyntaxToken<std::any>> keyword =
      this->match(SyntaxKindUtils::SyntaxKind::WhileKeyword);
  std::shared_ptr<ExpressionSyntax> condition = this->parseExpression();
  std::shared_ptr<BlockStatementSyntax> statement = this->parseBlockStatement();
  return std::make_shared<WhileStatementSyntax>(
      std::move(keyword), std::move(condition), std::move(statement));
}

std::shared_ptr<StatementSyntax> Parser::parseVariableDeclaration() {

  std::shared_ptr<SyntaxToken<std::any>> keyword = this->match(
      SyntaxKindUtils::SyntaxKind::VarKeyword == this->getCurrent()->getKind()
          ? SyntaxKindUtils::SyntaxKind::VarKeyword
          : SyntaxKindUtils::SyntaxKind::ConstKeyword);
  std::shared_ptr<SyntaxToken<std::any>> identifier =
      this->match(SyntaxKindUtils::SyntaxKind::IdentifierToken);
  std::shared_ptr<SyntaxToken<std::any>> equalsToken =
      this->match(SyntaxKindUtils::SyntaxKind::EqualsToken);
  std::shared_ptr<ExpressionSyntax> initializer = this->parseExpression();
  return std::make_shared<VariableDeclarationSyntax>(
      std::move(keyword), std::move(identifier), std::move(equalsToken),
      std::move(initializer));
}

std::shared_ptr<ExpressionSyntax>
Parser::parseExpression(int parentPrecedence) {

  std::shared_ptr<ExpressionSyntax> left;
  int unaryOperatorPrecedence =
      this->getCurrent()->getUnaryOperatorPrecedence();

  if (unaryOperatorPrecedence != 0 &&
      unaryOperatorPrecedence >= parentPrecedence) {
    std::shared_ptr<SyntaxToken<std::any>> operatorToken = this->nextToken();
    std::shared_ptr<ExpressionSyntax> operand =
        this->parseExpression(unaryOperatorPrecedence);
    left = std::make_shared<UnaryExpressionSyntax>(std::move(operatorToken),
                                                   std::move(operand));
  } else {
    left = this->parsePrimaryExpression();
  }

  while (true) {
    int precedence = this->getCurrent()->getBinaryOperatorPrecedence();
    if (precedence == 0 || precedence <= parentPrecedence) {
      break;
    }
    std::shared_ptr<SyntaxToken<std::any>> operatorToken = this->nextToken();

    std::shared_ptr<ExpressionSyntax> right = this->parseExpression(precedence);

    left = std::make_shared<BinaryExpressionSyntax>(
        std::move(left), std::move(operatorToken), std::move(right));
  }

  return left;
}

std::shared_ptr<ExpressionSyntax> Parser::parsePrimaryExpression() {
  switch (this->getCurrent()->getKind()) {
  case SyntaxKindUtils::OpenParenthesisToken: {
    std::shared_ptr<SyntaxToken<std::any>> left = this->nextToken();
    std::shared_ptr<ExpressionSyntax> expression = this->parseExpression();
    std::shared_ptr<SyntaxToken<std::any>> right =
        this->match(SyntaxKindUtils::SyntaxKind::CloseParenthesisToken);
    return std::make_shared<ParenthesizedExpressionSyntax>(
        std::move(left), std::move(expression), std::move(right));
  }
  case SyntaxKindUtils::NumberToken: {
    std::shared_ptr<SyntaxToken<std::any>> numberToken = this->nextToken();

    return std::make_shared<LiteralExpressionSyntax<std::any>>(
        std::move(numberToken), std::move(numberToken->getValue()));
  }

  case SyntaxKindUtils::StringToken: {
    std::shared_ptr<SyntaxToken<std::any>> stringToken = this->nextToken();
    return std::make_shared<LiteralExpressionSyntax<std::any>>(
        std::move(stringToken), stringToken->getValue());
  }
  case SyntaxKindUtils::TrueKeyword:
  case SyntaxKindUtils::FalseKeyword: {
    std::shared_ptr<SyntaxToken<std::any>> keywordToken = this->nextToken();
    bool value =
        keywordToken->getKind() == SyntaxKindUtils::SyntaxKind::TrueKeyword
            ? true
            : false;
    return std::make_shared<LiteralExpressionSyntax<std::any>>(
        std::move(keywordToken), value);
  }
  case SyntaxKindUtils::SyntaxKind::CommaToken: {
    return std::make_shared<LiteralExpressionSyntax<std::any>>(
        this->nextToken(), ",");
  }
  case SyntaxKindUtils::SyntaxKind::IdentifierToken: {
    return this->parseNameorCallExpression();
  }
  default:
    this->logs.push_back(Utils::getLineNumberAndPosition(this->getCurrent()) +
                         "ERROR: unexpected token <" +
                         this->getCurrent()->getText() + ">");
    return std::make_shared<LiteralExpressionSyntax<std::any>>(
        this->getCurrent(), (int)0);
  }
}

std::shared_ptr<ExpressionSyntax> Parser::parseNameorCallExpression() {
  if (this->peek(1)->getKind() == SyntaxKindUtils::SyntaxKind::EqualsToken) {
    std::shared_ptr<SyntaxToken<std::any>> identifierToken = this->nextToken();
    std::shared_ptr<SyntaxToken<std::any>> operatorToken = this->nextToken();
    std::shared_ptr<ExpressionSyntax> right = this->parseExpression();
    return std::make_shared<AssignmentExpressionSyntax>(
        std::make_shared<LiteralExpressionSyntax<std::any>>(
            std::move(identifierToken), identifierToken->getValue()),
        std::move(operatorToken), std::move(right));
  } else if (this->peek(1)->getKind() ==
             SyntaxKindUtils::SyntaxKind::OpenParenthesisToken) {
    std::shared_ptr<SyntaxToken<std::any>> identifierToken =
        this->match(SyntaxKindUtils::SyntaxKind::IdentifierToken);
    std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken =
        this->match(SyntaxKindUtils::SyntaxKind::OpenParenthesisToken);
    std::vector<std::shared_ptr<ExpressionSyntax>> arguments;
    while (this->getCurrent()->getKind() !=
               SyntaxKindUtils::SyntaxKind::CloseParenthesisToken &&
           this->getCurrent()->getKind() !=
               SyntaxKindUtils::SyntaxKind::EndOfFileToken) {
      std::shared_ptr<ExpressionSyntax> expression = this->parseExpression();
      arguments.push_back(expression);
      if (this->getCurrent()->getKind() !=
          SyntaxKindUtils::SyntaxKind::CloseParenthesisToken) {
        this->match(SyntaxKindUtils::SyntaxKind::CommaToken);
      }
    }

    std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken =
        this->match(SyntaxKindUtils::SyntaxKind::CloseParenthesisToken);
    return std::make_shared<CallExpressionSyntax>(
        std::make_shared<LiteralExpressionSyntax<std::any>>(
            std::move(identifierToken), identifierToken->getValue()),
        std::move(openParenthesisToken), arguments,
        std::move(closeParenthesisToken));
  } else {
    std::shared_ptr<SyntaxToken<std::any>> identifierToken = this->nextToken();
    return std::make_shared<VariableExpressionSyntax>(
        std::make_shared<LiteralExpressionSyntax<std::any>>(
            std::move(identifierToken), identifierToken->getValue()));
  }
}
