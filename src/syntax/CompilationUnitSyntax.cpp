#include "CompilationUnitSyntax.h"

CompilationUnitSyntax::CompilationUnitSyntax(
    std::vector<std::string> &logs, std::shared_ptr<StatementSyntax> statement,
    std::shared_ptr<SyntaxToken<std::any>> endOfFileToken) {
  this->statement = statement;
  this->endOfFileToken = (endOfFileToken);
  this->logs = logs;
}

SyntaxKindUtils::SyntaxKind CompilationUnitSyntax::getKind() {
  return SyntaxKindUtils::SyntaxKind::CompilationUnit;
}

std::shared_ptr<StatementSyntax> CompilationUnitSyntax::getStatement() {
  return this->statement;
}

std::shared_ptr<SyntaxToken<std::any>>
CompilationUnitSyntax::getEndOfFileToken() {
  return (this->endOfFileToken);
}
