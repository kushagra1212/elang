#pragma once
#include "../SyntaxToken.h"
#include "ExpressionSyntax.h"
#include <any>
#include <string>

template <typename T> class LiteralExpressionSyntax : public ExpressionSyntax {

private:
  std::shared_ptr<SyntaxToken<std::any>> token;
  T value;

public:
  LiteralExpressionSyntax(std::shared_ptr<SyntaxToken<std::any>> token,
                          T value);

public:
  SyntaxKindUtils::SyntaxKind getKind();

public:
  std::string getKindText();

public:
  std::shared_ptr<SyntaxToken<std::any>> getToken();

public:
  std::vector<std::shared_ptr<SyntaxNode>> getChildren();

public:
  T getValue();
};