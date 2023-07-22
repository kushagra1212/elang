#pragma once
#include "../../SyntaxKindUtils.h"
#include "../../SyntaxToken.h"
#include "../../expression/ExpressionSyntax.h"
#include "../ElseClauseSyntax/ElseClauseSyntax.h"
#include "../StatementSyntax.h"
class IfStatementSyntax : public StatementSyntax {
public:
  IfStatementSyntax(SyntaxToken<std::any> *ifKeyword,
                    ExpressionSyntax *condition, StatementSyntax *statement,
                    ElseClauseSyntax *elseClause);

  SyntaxToken<std::any> *getIfKeyword() const;
  ExpressionSyntax *getCondition() const;
  StatementSyntax *getStatement() const;
  ElseClauseSyntax *getElseClause() const;

  SyntaxKindUtils::SyntaxKind getKind() override;
  std::vector<SyntaxNode *> getChildren() override;

private:
  SyntaxToken<std::any> *ifKeyword;
  ExpressionSyntax *condition;
  StatementSyntax *statement;
  ElseClauseSyntax *elseClause;
};