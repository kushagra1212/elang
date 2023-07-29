#pragma once
#include "../../SyntaxToken.h"
#include "../StatementSyntax.h"
class BlockStatementSyntax : public StatementSyntax {
private:
  std::shared_ptr<SyntaxToken<std::any>> openBraceToken;
  std::vector<std::shared_ptr<StatementSyntax>> statements;
  std::shared_ptr<SyntaxToken<std::any>> closeBraceToken;

public:
  BlockStatementSyntax(std::shared_ptr<SyntaxToken<std::any>> openBraceToken,
                       std::vector<std::shared_ptr<StatementSyntax>> statements,
                       std::shared_ptr<SyntaxToken<std::any>> closeBraceToken);
  SyntaxKindUtils::SyntaxKind getKind();

  std::vector<std::shared_ptr<SyntaxNode>> getChildren();

  std::shared_ptr<SyntaxToken<std::any>> getOpenBraceToken();

  std::vector<std::shared_ptr<StatementSyntax>> getStatements();

  std::shared_ptr<SyntaxToken<std::any>> getCloseBraceToken();
};