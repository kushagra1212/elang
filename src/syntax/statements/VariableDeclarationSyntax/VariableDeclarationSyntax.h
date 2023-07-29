#pragma once
#include "../../../Common.h"
#include "../../SyntaxKindUtils.h"
#include "../../SyntaxToken.h"
#include "../../expression/ExpressionSyntax.h"
#include "../StatementSyntax.h"

class VariableDeclarationSyntax : public StatementSyntax {
private:
  std::shared_ptr<SyntaxToken<std::any>> _keyword;
  std::shared_ptr<SyntaxToken<std::any>> _identifier;
  std::shared_ptr<SyntaxToken<std::any>> _equalsToken;
  std::shared_ptr<ExpressionSyntax> _initializer;

public:
  VariableDeclarationSyntax(std::shared_ptr<SyntaxToken<std::any>> keyword,
                            std::shared_ptr<SyntaxToken<std::any>> identifier,
                            std::shared_ptr<SyntaxToken<std::any>> equalsToken,
                            std::shared_ptr<ExpressionSyntax> initializer);
  SyntaxKindUtils::SyntaxKind getKind() override;
  std::vector<std::shared_ptr<SyntaxNode>> getChildren() override;
  std::shared_ptr<SyntaxToken<std::any>> getIdentifier();
  std::shared_ptr<SyntaxToken<std::any>> getEqualsToken();
  std::shared_ptr<ExpressionSyntax> getInitializer() const;
  std::shared_ptr<SyntaxToken<std::any>> getKeyword();
};