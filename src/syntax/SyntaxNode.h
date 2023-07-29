
#ifndef __SYNTAXNODE_H__
#define __SYNTAXNODE_H__
#include "../Common.h"
#include "SyntaxKindUtils.h"
class SyntaxNode {

public:
  virtual SyntaxKindUtils::SyntaxKind getKind() = 0;

public:
  virtual std::vector<std::shared_ptr<SyntaxNode>> getChildren() = 0;
};
#endif