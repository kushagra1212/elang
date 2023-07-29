#include "BlockStatementSyntax.h"

BlockStatementSyntax::BlockStatementSyntax(
    std::shared_ptr<SyntaxToken<std::any>> openBraceToken,
    std::vector<std::shared_ptr<StatementSyntax>> statements,
    std::shared_ptr<SyntaxToken<std::any>> closeBraceToken) {
  this->openBraceToken = (openBraceToken);
  this->statements = statements;
  this->closeBraceToken = (closeBraceToken);
}

SyntaxKindUtils::SyntaxKind BlockStatementSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::BlockStatement;
}

std::vector<std::shared_ptr<SyntaxNode>> BlockStatementSyntax::getChildren() {
  std::vector<std::shared_ptr<SyntaxNode>> children;
  children.push_back((this->openBraceToken));
  for (int i = 0; i < this->statements.size(); i++) {
    children.push_back(this->statements[i]);
  }
  children.push_back((this->closeBraceToken));
  return children;
}

std::shared_ptr<SyntaxToken<std::any>>
BlockStatementSyntax::getOpenBraceToken() {
  return (this->openBraceToken);
}

std::vector<std::shared_ptr<StatementSyntax>>
BlockStatementSyntax::getStatements() {
  return this->statements;
}

std::shared_ptr<SyntaxToken<std::any>>
BlockStatementSyntax::getCloseBraceToken() {
  return (this->closeBraceToken);
}