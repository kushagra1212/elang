#pragma once
#include "../../../Common.h"
#include "../../../utils/Utils.h"
#include "../../SyntaxToken.h"
#include "../BlockStatementSyntax/BlockStatementSyntax.h"
class WhileStatementSyntax : public StatementSyntax {
private:
  std::shared_ptr<SyntaxToken<std::any>> _whileKeyword;
  ExpressionSyntax *_condition;
  BlockStatementSyntax *_body;

public:
  WhileStatementSyntax(std::shared_ptr<SyntaxToken<std::any>> whileKeyword,
                       ExpressionSyntax *condition, BlockStatementSyntax *body);

  SyntaxKindUtils::SyntaxKind getKind() override;

  std::vector<SyntaxNode *> getChildren() override;

  std::shared_ptr<SyntaxToken<std::any>> getWhileKeyword();

  ExpressionSyntax *getCondition() const;

  BlockStatementSyntax *getBody() const;
};