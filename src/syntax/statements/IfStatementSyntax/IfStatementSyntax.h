#pragma once
#include "../../SyntaxKindUtils.h"
#include "../../SyntaxToken.h"
#include "../../expression/ExpressionSyntax.h"
#include "../BlockStatementSyntax/BlockStatementSyntax.h"
#include "../ElseClauseSyntax/ElseClauseSyntax.h"
#include "../StatementSyntax.h"
class IfStatementSyntax : public StatementSyntax {
public:
  IfStatementSyntax(std::shared_ptr<SyntaxToken<std::any>> ifKeyword,
                    std::shared_ptr<ExpressionSyntax> condition,
                    std::shared_ptr<BlockStatementSyntax> statement,
                    std::shared_ptr<ElseClauseSyntax> elseClause);

  std::shared_ptr<SyntaxToken<std::any>> getIfKeyword();
  std::shared_ptr<ExpressionSyntax> getCondition() const;
  std::shared_ptr<BlockStatementSyntax> getStatement() const;
  std::shared_ptr<ElseClauseSyntax> getElseClause() const;

  SyntaxKindUtils::SyntaxKind getKind() override;
  std::vector<std::shared_ptr<SyntaxNode>> getChildren() override;

private:
  std::shared_ptr<SyntaxToken<std::any>> ifKeyword;
  std::shared_ptr<ExpressionSyntax> condition;
  std::shared_ptr<BlockStatementSyntax> statement;
  std::shared_ptr<ElseClauseSyntax> elseClause;
};
