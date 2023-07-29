#ifndef UnaryExpressionSyntax_h__
#define UnaryExpressionSyntax_h__

#include "../SyntaxToken.h"
#include "ExpressionSyntax.h"
class UnaryExpressionSyntax : public ExpressionSyntax {

private:
  std::shared_ptr<SyntaxToken<std::any>> operatorToken;
  std::shared_ptr<ExpressionSyntax> operand;

public:
  UnaryExpressionSyntax(std::shared_ptr<SyntaxToken<std::any>> operatorToken,
                        std::shared_ptr<ExpressionSyntax> operand);

public:
  SyntaxKindUtils::SyntaxKind getKind() override;

public:
  std::shared_ptr<SyntaxToken<std::any>> getOperatorToken();

public:
  std::shared_ptr<ExpressionSyntax> getOperand();

public:
  std::vector<std::shared_ptr<SyntaxNode>> getChildren() override;
};

#endif // UnaryExpressionSyntax_h__