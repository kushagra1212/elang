#pragma once
#include "../../SyntaxToken.h"
#include "../StatementSyntax.h"
class BlockStatementSyntax : StatementSyntax {
private:
  std::shared_ptr<SyntaxToken<std::any>> openBraceToken;
  std::vector<StatementSyntax *> statements;
  std::shared_ptr<SyntaxToken<std::any>> closeBraceToken;

public:
  BlockStatementSyntax(std::shared_ptr<SyntaxToken<std::any>> openBraceToken,
                       std::vector<StatementSyntax *> statements,
                       std::shared_ptr<SyntaxToken<std::any>> closeBraceToken);
  SyntaxKindUtils::SyntaxKind getKind();

  std::vector<SyntaxNode *> getChildren();

  std::shared_ptr<SyntaxToken<std::any>> getOpenBraceToken();

  std::vector<StatementSyntax *> getStatements();

  std::shared_ptr<SyntaxToken<std::any>> getCloseBraceToken();
};