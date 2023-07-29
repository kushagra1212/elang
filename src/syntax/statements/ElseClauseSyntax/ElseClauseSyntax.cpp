#include "ElseClauseSyntax.h"

ElseClauseSyntax::ElseClauseSyntax(
    std::shared_ptr<SyntaxToken<std::any>> elseKeyword,
    BlockStatementSyntax *statement)
    : elseKeyword((elseKeyword)), statement(statement) {}

std::shared_ptr<SyntaxToken<std::any>> ElseClauseSyntax::getElseKeyword() {
  return (elseKeyword);
}

BlockStatementSyntax *ElseClauseSyntax::getStatement() const {
  return statement;
}

SyntaxKindUtils::SyntaxKind ElseClauseSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::ElseClause;
}

std::vector<SyntaxNode *> ElseClauseSyntax::getChildren() {
  return {new SyntaxToken<std::any>(*elseKeyword), (SyntaxNode *)statement};
}
