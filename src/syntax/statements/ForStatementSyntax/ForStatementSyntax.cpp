#include "ForStatementSyntax.h"

ForStatementSyntax::ForStatementSyntax(
    std::shared_ptr<StatementSyntax> initialization,
    std::shared_ptr<ExpressionSyntax> upperBound,
    std::shared_ptr<BlockStatementSyntax> statement)
    : initialization(initialization), upperBound(upperBound),
      statement(statement) {}

std::shared_ptr<BlockStatementSyntax> ForStatementSyntax::getStatement() const {
  return this->statement;
}

SyntaxKindUtils::SyntaxKind ForStatementSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::ForStatement;
}

std::vector<std::shared_ptr<SyntaxNode>> ForStatementSyntax::getChildren() {
  return {this->initialization,
          std::dynamic_pointer_cast<SyntaxNode>(this->upperBound),
          std::dynamic_pointer_cast<SyntaxNode>(this->statement)};
}

std::shared_ptr<ExpressionSyntax> ForStatementSyntax::getUpperBound() const {
  return this->upperBound;
}

std::shared_ptr<StatementSyntax> ForStatementSyntax::getInitialization() const {
  return this->initialization;
}