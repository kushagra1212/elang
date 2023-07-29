#pragma once
#include "../SyntaxNode.h"
class StatementSyntax : public SyntaxNode {
public:
  virtual SyntaxKindUtils::SyntaxKind getKind() = 0;

public:
  virtual std::vector<std::shared_ptr<SyntaxNode>> getChildren() = 0;
};