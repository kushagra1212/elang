#ifndef PARENTHESIZEDEXPRESSIONSYNTAX_H
#define PARENTHESIZEDEXPRESSIONSYNTAX_H
#include "../SyntaxToken.h"
#include "ExpressionSyntax.h"
class ParenthesizedExpressionSyntax : public ExpressionSyntax {
private:
  std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken;
  ExpressionSyntax *expression;
  std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken;

public:
  std::vector<SyntaxNode *> children;

  ParenthesizedExpressionSyntax(
      std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken,
      ExpressionSyntax *expression,
      std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken);

public:
  SyntaxKindUtils::SyntaxKind getKind();

public:
  std::shared_ptr<SyntaxToken<std::any>> getOpenParenthesisToken();

public:
  ExpressionSyntax *getExpression();

public:
  std::shared_ptr<SyntaxToken<std::any>> getCloseParenthesisToken();

public:
  std::vector<SyntaxNode *> getChildren();
};
#endif