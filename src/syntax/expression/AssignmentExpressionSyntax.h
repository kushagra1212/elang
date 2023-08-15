#pragma once
#include "../../Common.h"
#include "../SyntaxToken.h"
#include "ExpressionSyntax.h"
#include "LiteralExpressionSyntax.h"
class AssignmentExpressionSyntax : public ExpressionSyntax {
private:
  std::unique_ptr<LiteralExpressionSyntax<std::any>> _left;
  std::unique_ptr<SyntaxToken<std::any>> _operatorToken;
  std::unique_ptr<ExpressionSyntax> _right;

public:
  AssignmentExpressionSyntax(
      std::unique_ptr<LiteralExpressionSyntax<std::any>> left,
      std::unique_ptr<SyntaxToken<std::any>> operatorToken,
      std::unique_ptr<ExpressionSyntax> right);

  std::unique_ptr<SyntaxToken<std::any>> getOperatorToken();

  std::unique_ptr<ExpressionSyntax> getRight();

  std::unique_ptr<LiteralExpressionSyntax<std::any>> getLeft();

  SyntaxKindUtils::SyntaxKind getKind() const override;

  std::vector<SyntaxNode *> getChildren() override;

  std::string getLineNumberAndColumn() override;

  std::unique_ptr<SyntaxToken<std::any>> &getOperatorTokenPtr();

  std::unique_ptr<ExpressionSyntax> &getRightPtr();

  std::unique_ptr<LiteralExpressionSyntax<std::any>> &getLeftPtr();
};
