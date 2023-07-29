#include "ElseClauseSyntax.h"

ElseClauseSyntax::ElseClauseSyntax(
    std::shared_ptr<SyntaxToken<std::any>> elseKeyword,
    std::shared_ptr<BlockStatementSyntax> statement)
    : elseKeyword((elseKeyword)), statement(statement) {}

std::shared_ptr<SyntaxToken<std::any>> ElseClauseSyntax::getElseKeyword() {
  return (elseKeyword);
}

std::shared_ptr<BlockStatementSyntax> ElseClauseSyntax::getStatement() const {
  return statement;
}

SyntaxKindUtils::SyntaxKind ElseClauseSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::ElseClause;
}

std::vector<std::shared_ptr<SyntaxNode>> ElseClauseSyntax::getChildren() {
  return {std::dynamic_pointer_cast<SyntaxNode>(elseKeyword),
          std::dynamic_pointer_cast<SyntaxNode>(statement)};
}
