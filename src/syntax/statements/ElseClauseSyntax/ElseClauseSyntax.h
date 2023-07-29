#pragma once
#include "../../SyntaxKindUtils.h"
#include "../../SyntaxNode.h"
#include "../../SyntaxToken.h"
#include "../BlockStatementSyntax/BlockStatementSyntax.h"
#include "../StatementSyntax.h"
class ElseClauseSyntax : public SyntaxNode {
private:
  std::shared_ptr<SyntaxToken<std::any>> elseKeyword;
  BlockStatementSyntax *statement;

public:
  ElseClauseSyntax(std::shared_ptr<SyntaxToken<std::any>> elseKeyword,
                   BlockStatementSyntax *statement);
  std::shared_ptr<SyntaxToken<std::any>> getElseKeyword();
  BlockStatementSyntax *getStatement() const;
  SyntaxKindUtils::SyntaxKind getKind() override;
  std::vector<SyntaxNode *> getChildren() override;
};
