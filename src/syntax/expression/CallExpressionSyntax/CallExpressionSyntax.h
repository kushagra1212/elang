#ifndef CALL_EXPRESSION_SYNTAX_H
#define CALL_EXPRESSION_SYNTAX_H

#include "../../SyntaxKindUtils.h"
#include "../../SyntaxNode.h"
#include "../../SyntaxToken.h"
#include "../ExpressionSyntax.h"

class CallExpressionSyntax : public ExpressionSyntax {
public:
  CallExpressionSyntax(
      std::shared_ptr<ExpressionSyntax> identifier,
      std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken,
      std::vector<std::shared_ptr<ExpressionSyntax>> arguments,
      std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken);

  std::shared_ptr<ExpressionSyntax> getIdentifier() const;
  std::shared_ptr<SyntaxToken<std::any>> getOpenParenthesisToken();
  std::vector<std::shared_ptr<ExpressionSyntax>> getArguments() const;
  std::shared_ptr<SyntaxToken<std::any>> getCloseParenthesisToken();
  SyntaxKindUtils::SyntaxKind getKind() override;

  std::vector<std::shared_ptr<SyntaxNode>> getChildren() override;

private:
  std::shared_ptr<ExpressionSyntax> identifier;
  std::shared_ptr<SyntaxToken<std::any>> openParenthesisToken;
  std::vector<std::shared_ptr<ExpressionSyntax>> arguments;
  std::shared_ptr<SyntaxToken<std::any>> closeParenthesisToken;
};

#endif // CALL_EXPRESSION_SYNTAX_H
