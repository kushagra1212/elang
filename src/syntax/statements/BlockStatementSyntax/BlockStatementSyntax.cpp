#include "BlockStatementSyntax.h"

BlockStatementSyntax::BlockStatementSyntax(
    std::shared_ptr<SyntaxToken<std::any>> openBraceToken,
    std::vector<StatementSyntax *> statements,
    std::shared_ptr<SyntaxToken<std::any>> closeBraceToken) {
  this->openBraceToken = (openBraceToken);
  this->statements = statements;
  this->closeBraceToken = (closeBraceToken);
}

SyntaxKindUtils::SyntaxKind BlockStatementSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::BlockStatement;
}

std::vector<SyntaxNode *> BlockStatementSyntax::getChildren() {
  std::vector<SyntaxNode *> children;
  children.push_back(new SyntaxToken<std::any>(*this->openBraceToken));
  for (int i = 0; i < this->statements.size(); i++) {
    children.push_back(this->statements[i]);
  }
  children.push_back(new SyntaxToken<std::any>(*this->closeBraceToken));
  return children;
}

std::shared_ptr<SyntaxToken<std::any>>
BlockStatementSyntax::getOpenBraceToken() {
  return (this->openBraceToken);
}

std::vector<StatementSyntax *> BlockStatementSyntax::getStatements() {
  return this->statements;
}

std::shared_ptr<SyntaxToken<std::any>>
BlockStatementSyntax::getCloseBraceToken() {
  return (this->closeBraceToken);
}