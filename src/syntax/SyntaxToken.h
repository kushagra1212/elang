
#ifndef SYNTAXTOKEN_H
#define SYNTAXTOKEN_H
#include "SyntaxNode.h"

template <typename T> class SyntaxToken : public SyntaxNode {
private:
  SyntaxKindUtils::SyntaxKind kind;

private:
  int lineNumber;
  int position;

private:
  std::string text;

private:
  T value;

public:
  SyntaxToken(int lineNumber, SyntaxKindUtils::SyntaxKind kind, int position,
              std::string text, T value);

public:
  SyntaxKindUtils::SyntaxKind getKind();

public:
  int getPosition();

public:
  int getLineNumber();

public:
  std::string getText();

public:
  T getValue();

public:
  std::string getKindText();

public:
  int getUnaryOperatorPrecedence();

public:
  int getBinaryOperatorPrecedence();

public:
public:
  std::vector<std::shared_ptr<SyntaxNode>> children;
  std::vector<std::shared_ptr<SyntaxNode>> getChildren();
};
#endif