#ifndef PARENTHESIZEDEXPRESSIONSYNTAX_H
#define PARENTHESIZEDEXPRESSIONSYNTAX_H
#include "../SyntaxToken.h"
#include "ExpressionSyntax.h"
class ParenthesizedExpressionSyntax : public ExpressionSyntax {
private:
  std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken;
  std::shared_ptr<ExpressionSyntax> expression;
  std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken;

public:
  ParenthesizedExpressionSyntax(
      std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken,
      std::shared_ptr<ExpressionSyntax> expression,
      std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken);

public:
  SyntaxKindUtils::SyntaxKind getKind();

public:
  std::shared_ptr<SyntaxToken<std::any>> getOpenParenthesisToken();

public:
  std::shared_ptr<ExpressionSyntax> getExpression();

public:
  std::shared_ptr<SyntaxToken<std::any>> getCloseParenthesisToken();

public:
  std::vector<std::shared_ptr<SyntaxNode>> getChildren();
};
#endif