#pragma once
#include "../../../Common.h"
#include "../../../utils/Utils.h"
#include "../../SyntaxKindUtils.h"
#include "../../SyntaxToken.h"
#include "../../expression/ExpressionSyntax.h"
#include "../../expression/TypeExpressionSyntax/TypeExpressionSyntax.h"
#include "../StatementSyntax.h"

class VariableDeclarationSyntax : public StatementSyntax {
 private:
  std::unique_ptr<SyntaxToken<std::any>> _keyword;
  std::unique_ptr<SyntaxToken<std::any>> _identifier;
  std::unique_ptr<ExpressionSyntax> _initializer;
  std::unique_ptr<TypeExpressionSyntax> _typeExpr;

 public:
  SyntaxKindUtils::SyntaxKind getKind() const override;
  std::vector<SyntaxNode *> getChildren() override;
  DiagnosticUtils::SourceLocation getSourceLocation() const override;

  // Setters
  inline void setKeyword(std::unique_ptr<SyntaxToken<std::any>> keyword) {
    _keyword = std::move(keyword);
  }

  inline void setIdentifier(std::unique_ptr<SyntaxToken<std::any>> identifier) {
    _identifier = std::move(identifier);
  }

  inline void setInitializer(std::unique_ptr<ExpressionSyntax> initializer) {
    _initializer = std::move(initializer);
  }

  inline void setTypeExpr(std::unique_ptr<TypeExpressionSyntax> typeExpr) {
    _typeExpr = std::move(typeExpr);
  }

  // Getters
  inline auto getIdentifierRef()
      -> const std::unique_ptr<SyntaxToken<std::any>> & {
    return _identifier;
  }

  inline auto getKeywordRef()
      -> const std::unique_ptr<SyntaxToken<std::any>> & {
    return _keyword;
  }

  inline auto getTypeRef() -> const std::unique_ptr<TypeExpressionSyntax> & {
    return _typeExpr;
  }

  inline auto getInitializerRef() -> const std::unique_ptr<ExpressionSyntax> & {
    return _initializer;
  }
};