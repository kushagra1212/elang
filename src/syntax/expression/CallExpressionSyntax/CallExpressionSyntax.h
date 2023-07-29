#ifndef CALL_EXPRESSION_SYNTAX_H
#define CALL_EXPRESSION_SYNTAX_H

#include "../../SyntaxKindUtils.h"
#include "../../SyntaxNode.h"
#include "../../SyntaxToken.h"
#include "../ExpressionSyntax.h"

class CallExpressionSyntax : public ExpressionSyntax {
public:
  CallExpressionSyntax(
      ExpressionSyntax *identifier,
      std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken,
      std::vector<ExpressionSyntax *> arguments,
      std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken);

  ExpressionSyntax *getIdentifier() const;
  std::shared_ptr<SyntaxToken<std::any>> getOpenParenthesisToken();
  std::vector<ExpressionSyntax *> getArguments() const;
  std::shared_ptr<SyntaxToken<std::any>> getCloseParenthesisToken();
  SyntaxKindUtils::SyntaxKind getKind() override;

  std::vector<SyntaxNode *> getChildren() override;

private:
  ExpressionSyntax *identifier;
  std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken;
  std::vector<ExpressionSyntax *> arguments;
  std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken;
};

#endif // CALL_EXPRESSION_SYNTAX_H
