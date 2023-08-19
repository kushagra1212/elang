#include "OrIfStatementSyntax.h"

OrIfStatementSyntax::OrIfStatementSyntax(
    std::unique_ptr<SyntaxToken<std::any>> orKeyword,
    std::unique_ptr<SyntaxToken<std::any>> ifKeyword,
    std::unique_ptr<ExpressionSyntax> condition,
    std::unique_ptr<BlockStatementSyntax> statement) {
  this->orKeyword = std::move(orKeyword);
  this->ifKeyword = std::move(ifKeyword);
  this->condition = std::move(condition);
  this->statement = std::move(statement);

  // Add children

  _children.push_back(this->orKeyword.get());
  _children.push_back(this->ifKeyword.get());
  _children.push_back(this->condition.get());
  _children.push_back(this->statement.get());
}

std::unique_ptr<SyntaxToken<std::any>> OrIfStatementSyntax::getIfKeyword() {
  return std::move(ifKeyword);
}

std::unique_ptr<ExpressionSyntax> OrIfStatementSyntax::getCondition() {
  return std::move(condition);
}

std::unique_ptr<BlockStatementSyntax> OrIfStatementSyntax::getStatement() {
  return std::move(statement);
}

std::unique_ptr<SyntaxToken<std::any>> OrIfStatementSyntax::getOrKeyword() {
  return std::move(orKeyword);
}

SyntaxKindUtils::SyntaxKind OrIfStatementSyntax::getKind() const {
  return SyntaxKindUtils::SyntaxKind::OrIfStatement;
}

std::string OrIfStatementSyntax::getLineNumberAndColumn() {
  return orKeyword->getLineNumberAndColumn();
}

std::vector<SyntaxNode *> OrIfStatementSyntax::getChildren() {
  return this->_children;
}

std::unique_ptr<SyntaxToken<std::any>> &OrIfStatementSyntax::getIfKeywordPtr() {
  return this->ifKeyword;
}
std::unique_ptr<ExpressionSyntax> &OrIfStatementSyntax::getConditionPtr() {
  return this->condition;
}
std::unique_ptr<BlockStatementSyntax> &OrIfStatementSyntax::getStatementPtr() {
  return this->statement;
}
std::unique_ptr<SyntaxToken<std::any>> &OrIfStatementSyntax::getOrKeywordPtr() {
  return this->orKeyword;
}