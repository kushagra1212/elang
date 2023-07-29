#ifndef LEXER_H
#define LEXER_H

#include "../Common.h"
#include "../syntax/SyntaxToken.h"
#include "../utils/Utils.h"
class Lexer {

private:
  std::vector<std::string> text;

private:
  int lineNumber;
  int position;

public:
  std::vector<std::string> logs;

public:
  Lexer(std::vector<std::string> text);

private:
  char getCurrent();

private:
  void next();

private:
  void updatePosition();

public:
  std::shared_ptr<SyntaxToken<std::any>> nextToken();

public:
  std::shared_ptr<SyntaxToken<std::any>> readString();
};
#endif // LEXER_H