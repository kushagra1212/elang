#include "Lexer.h"

Lexer::Lexer(std::vector<std::string> text) { this->text = text; }

char Lexer::getCurrent() {
  if (this->position >= this->text[lineNumber].length()) {
    return '\0';
  }
  return this->text[lineNumber][this->position];
}
void Lexer::updatePosition() {
  this->position = 0;
  this->lineNumber++;
}
void Lexer::next() { this->position++; }

std::shared_ptr<SyntaxToken<std::any>> Lexer::nextToken() {
  if (this->position >= this->text[lineNumber].length()) {

    this->updatePosition();
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::EndOfLineToken,
        this->position, "\0", 0);
  }

  if (this->lineNumber >= this->text.size()) {
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::EndOfFileToken,
        this->position, "\0", 0);
  }
  // check for int

  if (isdigit(this->getCurrent())) {
    int start = this->position;
    while (isdigit(this->getCurrent())) {
      this->next();
    }

    // check for double

    if (this->getCurrent() == '.') {
      this->next();
      while (isdigit(this->getCurrent())) {
        this->next();
      }
      int length = this->position - start;
      std::string text = this->text[lineNumber].substr(start, length);
      try {
        if (SyntaxKindUtils::isDouble(text) == false) {
          throw std::exception();
        }
      } catch (std::exception e) {
        std::shared_ptr<SyntaxToken<std::any>> newSyntaxToken =
            std::make_shared<SyntaxToken<std::any>>(
                this->lineNumber, SyntaxKindUtils::SyntaxKind::BadToken, start,
                text, 0);
        logs.push_back(
            Utils::getLineNumberAndPosition(
                std::make_shared<SyntaxToken<std::any>>(*newSyntaxToken)) +
            "ERROR: bad number input not double: " + text);
        return newSyntaxToken;
      }
      double res = stod(text);

      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::NumberToken, start,
          text, (res));
    }

    int length = this->position - start;
    std::string text = this->text[lineNumber].substr(start, length);
    try {
      if (SyntaxKindUtils::isInt64(text) == false) {
        throw std::exception();
      }
    } catch (std::exception e) {

      std::shared_ptr<SyntaxToken<std::any>> newSyntaxToken =
          std::make_shared<SyntaxToken<std::any>>(
              this->lineNumber, SyntaxKindUtils::SyntaxKind::BadToken, start,
              text, 0);

      logs.push_back(
          Utils::getLineNumberAndPosition(
              std::make_shared<SyntaxToken<std::any>>(*newSyntaxToken)) +
          "ERROR: bad number input not i32: " + text);
      return newSyntaxToken;
    }
    int res = stoi(text);

    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::NumberToken, start, text,
        res);
  }

  // Check true or false

  if (isalpha(this->getCurrent())) {
    int start = this->position;
    while (isalnum(this->getCurrent())) {
      this->next();
    }
    int length = this->position - start;
    std::string text = this->text[lineNumber].substr(start, length);
    if (text == "true") {
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::TrueKeyword, start,
          text, true);
    }
    if (text == "false") {
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::FalseKeyword, start,
          text, false);
    }
    if (text == "var") {
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::VarKeyword, start,
          text, "var");
    }

    if (text == "if") {
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::IfKeyword, start, text,
          "if");
    }

    if (text == "else") {
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::ElseKeyword, start,
          text, "else");
    }

    if (text == "while") {
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::WhileKeyword, start,
          text, "while");
    }

    if (text == "for") {
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::ForKeyword, start,
          text, "for");
    }

    if (text == "to") {
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::ToKeyword, start, text,
          "to");
    }

    if (text == "const") {
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::ConstKeyword, start,
          text, "const");
    }

    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::IdentifierToken, start,
        text, (text));
  }

  if (isspace(this->getCurrent())) {
    int start = this->position;
    while (isspace(this->getCurrent())) {
      this->next();
    }
    int length = this->position - start;
    std::string text = this->text[lineNumber].substr(start, length);
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::WhitespaceToken, start,
        text, 0);
  }

  switch (this->getCurrent()) {
  case '+':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::PlusToken,
        this->position++, "+", 0);
  case '-':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::MinusToken,
        this->position++, "-", 0);
  case '*':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::StarToken,
        this->position++, "*", 0);
  case '/':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::SlashToken,
        this->position++, "/", 0);
  case '(':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::OpenParenthesisToken,
        this->position++, "(", 0);
  case ')':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::CloseParenthesisToken,
        this->position++, ")", 0);
  case '&': {

    if (this->position + 1 < this->text[lineNumber].length() &&
        this->text[lineNumber][this->position + 1] == '&') {
      this->next();
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber,
          SyntaxKindUtils::SyntaxKind::AmpersandAmpersandToken,
          this->position++, "&&", 0);
    }

    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::AmpersandToken,
        this->position++, "&", 0);
  }
  case '^':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::CaretToken,
        this->position++, "^", 0);
  case '%':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::PercentToken,
        this->position++, "%", 0);
  case '~':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::TildeToken,
        this->position++, "~", 0);
  case '|': {

    if (this->position + 1 < this->text[lineNumber].length() &&
        this->text[lineNumber][this->position + 1] == '|') {
      this->next();
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::PipePipeToken,
          this->position++, "||", 0);
    }

    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::PipeToken,
        this->position++, "|", 0);
  }
  case '=': {

    if (this->position + 1 < this->text[lineNumber].length() &&
        this->text[lineNumber][this->position + 1] == '=') {
      this->next();
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::EqualsEqualsToken,
          this->position++, "==", 0);
    }
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::EqualsToken,
        this->position++, "=", 0);
  }

  case '!': {

    if (this->position + 1 < this->text[lineNumber].length() &&
        this->text[lineNumber][this->position + 1] == '=') {
      this->next();
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::BangEqualsToken,
          this->position++, "!=", 0);
    }
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::BangToken,
        this->position++, "!", 0);
  }

  case '<': {

    if (this->position + 1 < this->text[lineNumber].length() &&
        this->text[lineNumber][this->position + 1] == '=') {
      this->next();
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::LessOrEqualsToken,
          this->position++, "<=", 0);
    }
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::LessToken,
        this->position++, "<", 0);
  }

  case '>': {

    if (this->position + 1 < this->text[lineNumber].length() &&
        this->text[lineNumber][this->position + 1] == '=') {
      this->next();
      return std::make_shared<SyntaxToken<std::any>>(
          this->lineNumber, SyntaxKindUtils::SyntaxKind::GreaterOrEqualsToken,
          this->position++, ">=", 0);
    }
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::GreaterToken,
        this->position++, ">", 0);
  }

  case ';':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::SemicolonToken,
        this->position++, ";", 0);
  case ',':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::CommaToken,
        this->position++, ",", ",");
  case '{':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::OpenBraceToken,
        this->position++, "{", 0);
  case '}':
    return std::make_shared<SyntaxToken<std::any>>(
        this->lineNumber, SyntaxKindUtils::SyntaxKind::CloseBraceToken,
        this->position++, "}", 0);
  case '"':
    return (this->readString());

  default:

    std::shared_ptr<SyntaxToken<std::any>> newSyntaxToken =
        std::make_shared<SyntaxToken<std::any>>(
            this->lineNumber, SyntaxKindUtils::SyntaxKind::BadToken,
            this->position++,
            this->text[lineNumber].substr(this->position - 1, 1), 0);
    logs.push_back(
        Utils::getLineNumberAndPosition(
            std::make_shared<SyntaxToken<std::any>>(*newSyntaxToken)) +
        "ERROR: bad character input: " +
        this->text[lineNumber].substr(this->position, 1));

    return (newSyntaxToken);
  }
}

std::shared_ptr<SyntaxToken<std::any>> Lexer::readString() {
  int start = this->position++;
  std::string text = "";
  while (this->getCurrent() != '"') {
    if (this->getCurrent() == '\0') {

      std::shared_ptr<SyntaxToken<std::any>> newSyntaxToken =
          std::make_shared<SyntaxToken<std::any>>(
              this->lineNumber, SyntaxKindUtils::SyntaxKind::BadToken, start,
              this->text[lineNumber].substr(start, this->position - start), 0);

      logs.push_back(
          Utils::getLineNumberAndPosition(
              std::make_shared<SyntaxToken<std::any>>(*newSyntaxToken)) +
          "ERROR: unterminated string literal");
      return (newSyntaxToken);
    }
    if (this->getCurrent() == '\\') {
      this->next();
      switch (this->getCurrent()) {
      case '"':
        text += '"';
        break;
      case '\\':
        text += '\\';
        break;
      case 'n':
        text += '\n';
        break;
      case 'r':
        text += '\r';
        break;
      case 't':
        text += '\t';
        break;
      default:
        std::shared_ptr<SyntaxToken<std::any>> newSyntaxToken =
            std::make_shared<SyntaxToken<std::any>>(
                this->lineNumber, SyntaxKindUtils::SyntaxKind::BadToken, start,
                this->text[lineNumber].substr(start, this->position - start),
                0);
        logs.push_back(
            Utils::getLineNumberAndPosition(
                std::make_shared<SyntaxToken<std::any>>(*newSyntaxToken)) +
            "ERROR: bad character escape sequence: \\" +
            this->text[lineNumber].substr(this->position, 1));
        return (newSyntaxToken);
      }
    } else {
      text += this->getCurrent();
    }
    this->next();
  }
  this->next();
  return std::make_shared<SyntaxToken<std::any>>(
      this->lineNumber, SyntaxKindUtils::SyntaxKind::StringToken, start,
      this->text[lineNumber].substr(start, this->position - start), (text));

  //&(text)
}