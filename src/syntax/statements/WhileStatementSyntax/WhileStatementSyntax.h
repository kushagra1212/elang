#pragma once
#include "../../../Common.h"
#include "../../../utils/Utils.h"
#include "../../SyntaxToken.h"
#include "../BlockStatementSyntax/BlockStatementSyntax.h"
class WhileStatementSyntax : public StatementSyntax {
private:
  std::shared_ptr<SyntaxToken<std::any>> _whileKeyword;
  std::shared_ptr<ExpressionSyntax> _condition;
  std::shared_ptr<BlockStatementSyntax> _body;

public:
  WhileStatementSyntax(std::shared_ptr<SyntaxToken<std::any>> whileKeyword,
                       std::shared_ptr<ExpressionSyntax> condition,
                       std::shared_ptr<BlockStatementSyntax> body);

  SyntaxKindUtils::SyntaxKind getKind() override;

  std::vector<std::shared_ptr<SyntaxNode>> getChildren() override;

  std::shared_ptr<SyntaxToken<std::any>> getWhileKeyword();

  std::shared_ptr<ExpressionSyntax> getCondition() const;

  std::shared_ptr<BlockStatementSyntax> getBody() const;
};