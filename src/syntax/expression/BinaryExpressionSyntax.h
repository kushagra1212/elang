#ifndef BINARYEXPRESSIONSYNTAX_H
#define BINARYEXPRESSIONSYNTAX_H
#include "../SyntaxToken.h"
#include "ExpressionSyntax.h"
class BinaryExpressionSyntax : public ExpressionSyntax {

private:
  ExpressionSyntax *left;
  std::shared_ptr<SyntaxToken<std::any>> operatorToken;
  ExpressionSyntax *right;

public:
  std::vector<SyntaxNode *> children;
  BinaryExpressionSyntax(ExpressionSyntax *left,
                         std::shared_ptr<SyntaxToken<std::any>> operatorToken,
                         ExpressionSyntax *right);

public:
  SyntaxKindUtils::SyntaxKind getKind();

public:
  ExpressionSyntax *getLeft();

public:
  std::shared_ptr<SyntaxToken<std::any>> getOperatorToken();

public:
  ExpressionSyntax *getRight();

public:
  std::vector<SyntaxNode *> getChildren();
};
#endif