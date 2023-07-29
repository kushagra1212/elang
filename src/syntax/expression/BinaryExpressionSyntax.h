#ifndef BINARYEXPRESSIONSYNTAX_H
#define BINARYEXPRESSIONSYNTAX_H
#include "../SyntaxToken.h"
#include "ExpressionSyntax.h"
class BinaryExpressionSyntax : public ExpressionSyntax {

private:
  std::shared_ptr<ExpressionSyntax> left;
  std::shared_ptr<SyntaxToken<std::any>> operatorToken;
  std::shared_ptr<ExpressionSyntax> right;

public:
  BinaryExpressionSyntax(std::shared_ptr<ExpressionSyntax> left,
                         std::shared_ptr<SyntaxToken<std::any>> operatorToken,
                         std::shared_ptr<ExpressionSyntax> right);

public:
  SyntaxKindUtils::SyntaxKind getKind();

public:
  std::shared_ptr<ExpressionSyntax> getLeft();

public:
  std::shared_ptr<SyntaxToken<std::any>> getOperatorToken();

public:
  std::shared_ptr<ExpressionSyntax> getRight();

public:
  std::vector<std::shared_ptr<SyntaxNode>> getChildren();
};
#endif