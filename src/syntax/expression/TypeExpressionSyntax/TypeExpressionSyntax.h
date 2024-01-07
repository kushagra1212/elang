#ifndef __FLOW__WING__TYPEEXPRESSIONSYNTAX_H__
#define __FLOW__WING__TYPEEXPRESSIONSYNTAX_H__
#include "../../../Common.h"
#include "../../../utils/Utils.h"
#include "../../SyntaxToken.h"
#include "../ExpressionSyntax.h"
#include "../LiteralExpressionSyntax.h"

class TypeExpressionSyntax : public ExpressionSyntax {
 private:
  std::unique_ptr<SyntaxToken<std::any>> _type;

 public:
  TypeExpressionSyntax(std::unique_ptr<SyntaxToken<std::any>> type);
  virtual SyntaxKindUtils::SyntaxKind getKind() const override;
  virtual std::vector<SyntaxNode *> getChildren() override;
  virtual DiagnosticUtils::SourceLocation getSourceLocation() const override;

  inline auto getTypeRef() const
      -> const std::unique_ptr<SyntaxToken<std::any>> & {
    return _type;
  }
};

#endif  // __FLOW__WING__TYPEEXPRESSIONSYNTAX_H__
