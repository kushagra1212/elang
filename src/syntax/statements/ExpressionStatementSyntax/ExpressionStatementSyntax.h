#pragma once
#include "../../expression/ExpressionSyntax.h"
#include "../StatementSyntax.h"

class ExpressionStatementSyntax : public StatementSyntax {
private:
  std::shared_ptr<ExpressionSyntax> expression;

public:
  ExpressionStatementSyntax(std::shared_ptr<ExpressionSyntax> expression);

  SyntaxKindUtils::SyntaxKind getKind();

  std::vector<std::shared_ptr<SyntaxNode>> getChildren();

  std::shared_ptr<ExpressionSyntax> getExpression();
};