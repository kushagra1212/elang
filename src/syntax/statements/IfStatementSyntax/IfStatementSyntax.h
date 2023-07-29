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
                    ExpressionSyntax *condition,
                    BlockStatementSyntax *statement,
                    ElseClauseSyntax *elseClause);

  std::shared_ptr<SyntaxToken<std::any>> getIfKeyword();
  ExpressionSyntax *getCondition() const;
  BlockStatementSyntax *getStatement() const;
  ElseClauseSyntax *getElseClause() const;

  SyntaxKindUtils::SyntaxKind getKind() override;
  std::vector<SyntaxNode *> getChildren() override;

private:
  std::shared_ptr<SyntaxToken<std::any>> ifKeyword;
  ExpressionSyntax *condition;
  BlockStatementSyntax *statement;
  ElseClauseSyntax *elseClause;
};
