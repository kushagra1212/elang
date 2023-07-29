#ifndef UnaryExpressionSyntax_h__
#define UnaryExpressionSyntax_h__

#include "../SyntaxToken.h"
#include "ExpressionSyntax.h"
class UnaryExpressionSyntax : public ExpressionSyntax {

private:
  std::shared_ptr<SyntaxToken<std::any>> operatorToken;
  ExpressionSyntax *operand;

public:
  std::vector<SyntaxNode *> children;
  UnaryExpressionSyntax(std::shared_ptr<SyntaxToken<std::any>> operatorToken,
                        ExpressionSyntax *operand);

public:
  SyntaxKindUtils::SyntaxKind getKind();

public:
  std::shared_ptr<SyntaxToken<std::any>> getOperatorToken();

public:
  ExpressionSyntax *getOperand();

public:
  std::vector<SyntaxNode *> getChildren();
};

#endif // UnaryExpressionSyntax_h__