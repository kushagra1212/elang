#include "IfStatementSyntax.h"

IfStatementSyntax::IfStatementSyntax(
    std::shared_ptr<SyntaxToken<std::any>> ifKeyword,
    ExpressionSyntax *condition, BlockStatementSyntax *statement,
    ElseClauseSyntax *elseClause)
    : ifKeyword((ifKeyword)), condition(condition), statement(statement),
      elseClause(elseClause) {}

std::shared_ptr<SyntaxToken<std::any>> IfStatementSyntax::getIfKeyword() {
  return (ifKeyword);
}

ExpressionSyntax *IfStatementSyntax::getCondition() const { return condition; }

BlockStatementSyntax *IfStatementSyntax::getStatement() const {
  return statement;
}

ElseClauseSyntax *IfStatementSyntax::getElseClause() const {
  return elseClause;
}

SyntaxKindUtils::SyntaxKind IfStatementSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::IfStatement;
}

std::vector<SyntaxNode *> IfStatementSyntax::getChildren() {
  std::vector<SyntaxNode *> children = {new SyntaxToken<std::any>(*ifKeyword),
                                        condition, (SyntaxNode *)statement};
  if (elseClause != nullptr) {
    children.push_back(elseClause);
  }
  return children;
}