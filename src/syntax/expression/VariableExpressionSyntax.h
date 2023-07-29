#pragma once
#include "ExpressionSyntax.h"
class VariableExpressionSyntax : public ExpressionSyntax {
private:
  std::shared_ptr<ExpressionSyntax> identifierExpression;

public:
  VariableExpressionSyntax(
      std::shared_ptr<ExpressionSyntax> identifierExpression);

public:
  SyntaxKindUtils::SyntaxKind getKind();

public:
  std::shared_ptr<ExpressionSyntax> getIdentifier();

public:
  std::vector<std::shared_ptr<SyntaxNode>> getChildren();
};
