#ifndef CALL_EXPRESSION_SYNTAX_H
#define CALL_EXPRESSION_SYNTAX_H

#include "../../SyntaxKindUtils.h"
#include "../../SyntaxNode.h"
#include "../../SyntaxToken.h"
#include "../ExpressionSyntax.h"
#include "../LiteralExpressionSyntax.h"

class CallExpressionSyntax : public ExpressionSyntax {
private:
  std::unique_ptr<LiteralExpressionSyntax<std::any>> _identifier;
  std::unique_ptr<SyntaxToken<std::any>> _openParenthesisToken;
  std::vector<std::unique_ptr<ExpressionSyntax>> _arguments;
  std::unique_ptr<SyntaxToken<std::any>> _closeParenthesisToken;

public:
  CallExpressionSyntax(
      std::unique_ptr<LiteralExpressionSyntax<std::any>> identifier);

  std::unique_ptr<LiteralExpressionSyntax<std::any>> getIdentifier();
  std::unique_ptr<SyntaxToken<std::any>> getOpenParenthesisToken();
  std::vector<std::unique_ptr<ExpressionSyntax>> &getArguments();

  void addArgument(std::unique_ptr<ExpressionSyntax> argument);

  void setOpenParenthesisToken(
      std::unique_ptr<SyntaxToken<std::any>> openParenthesisToken);

  void setCloseParenthesisToken(
      std::unique_ptr<SyntaxToken<std::any>> closeParenthesisToken);

  std::unique_ptr<SyntaxToken<std::any>> getCloseParenthesisToken();

  SyntaxKindUtils::SyntaxKind getKind() const override;

  std::vector<SyntaxNode *> getChildren() override;

  std::string getLineNumberAndColumn() override;
};

#endif // CALL_EXPRESSION_SYNTAX_H
