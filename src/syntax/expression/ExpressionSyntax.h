

#ifndef __EXPRESSIONSYNTAX_H__
#define __EXPRESSIONSYNTAX_H__
#include "../SyntaxNode.h"
class ExpressionSyntax : public SyntaxNode {

public:
  virtual SyntaxKindUtils::SyntaxKind getKind() = 0;

public:
  virtual std::vector<std::shared_ptr<SyntaxNode>> getChildren() = 0;
};
#endif