#include "Repl.h"
Repl::Repl()
    : seeTree(false), braceCount(0), previousEvaluator(nullptr), text() {

  SyntaxKindUtils::init_enum_to_string_map();
}

Repl::~Repl() {}

void Repl::run() { runWithStream(std::cin, std::cout); }

void Repl::runWithStream(std::istream &inputStream,
                         std::ostream &outputStream) {
  printWelcomeMessage(outputStream);

  std::string line;

  while (true) {
    if (braceCount || text.size()) {
      outputStream << YELLOW << "... " << RESET;
    } else {
      outputStream << GREEN << ">>> " << RESET;
    }
    line = "";
    std::getline(inputStream, line);
    // Common Function to run both REPL and tests
    if (line.empty() && !text.size()) {
      continue;
    }
    if (line == "`:exit") {
      break;
    }

    if (handleSpecialCommands(line)) {
      continue;
    }
    text.push_back(line);

    braceCount = countBraces(line, '{') - countBraces(line, '}') + braceCount;

    if (braceCount) {
      continue;
    }
    std::unique_ptr<Parser> parser = std::make_unique<Parser>(text);

    std::shared_ptr<CompilationUnitSyntax> compilationUnit =
        (parser->parseCompilationUnit());
    if (!line.empty() && compilationUnit->logs.size()) {
      continue;
    } else {
      compileAndEvaluate((compilationUnit), outputStream);
    }
    text = std::vector<std::string>();
  }
}

void Repl::compileAndEvaluate(
    std::shared_ptr<CompilationUnitSyntax> compilationUnit,
    std::ostream &outputStream) {

  std::shared_ptr<Evaluator> currentEvaluator =
      previousEvaluator == nullptr
          ? std::make_shared<Evaluator>(nullptr, (compilationUnit))
          : std::make_shared<Evaluator>((previousEvaluator), (compilationUnit));

  BoundScopeGlobal *globalScope = currentEvaluator->getRoot();
  compilationUnit->logs.insert(compilationUnit->logs.end(),
                               globalScope->logs.begin(),
                               globalScope->logs.end());

  text = std::vector<std::string>();

  if (seeTree) {
    Utils::prettyPrint(compilationUnit->getStatement());
  }

  if (compilationUnit->logs.size()) {
    for (const std::string &log : compilationUnit->logs) {
      outputStream << RED << log << RESET << "\n";
    }
  } else {
    try {
      currentEvaluator->evaluateStatement(globalScope->statement);
      std::any result = currentEvaluator->last_value;

      if (currentEvaluator->getRoot()->logs.size()) {
        for (const std::string &log : currentEvaluator->getRoot()->logs) {
          outputStream << RED << log << RESET << "\n";
        }
      } else {
        if (result.type() == typeid(int)) {
          int intValue = std::any_cast<int>(result);
          outputStream << intValue << "\n";
        } else if (result.type() == typeid(bool)) {
          bool boolValue = std::any_cast<bool>(result);
          outputStream << (boolValue ? "true" : "false") << "\n";
        } else if (result.type() == typeid(std::string)) {
          std::string stringValue = std::any_cast<std::string>(result);
          outputStream << stringValue << "\n";
        } else if (result.type() == typeid(double)) {
          double doubleValue = std::any_cast<double>(result);
          outputStream << doubleValue << "\n";
        } else if (result.type() == typeid(std::nullptr_t)) {
          // Handle the nullptr_t case if needed
        } else {
          throw std::runtime_error("Unexpected result type");
        }
      }
    } catch (const std::exception &e) {
      outputStream << RED << e.what() << RESET << "\n";
    }
  }

  previousEvaluator = std::move(currentEvaluator);
}

void Repl::runForTest(std::istream &inputStream, std::ostream &outputStream) {

  SyntaxKindUtils::init_enum_to_string_map();
  std::string line;

  while (std::getline(inputStream, line)) {

    if (line.empty()) {
      continue;
    }

    if (line == ":exit") {
      break;
    }

    if (handleSpecialCommands(line)) {
      continue;
    }

    text.push_back(line);
    braceCount = countBraces(line, '{') - countBraces(line, '}') + braceCount;

    if (braceCount) {
      continue;
    }
    std::unique_ptr<Parser> parser = std::make_unique<Parser>(text);

    std::shared_ptr<CompilationUnitSyntax> compilationUnit =
        parser->parseCompilationUnit();

    this->compileAndEvaluate((compilationUnit), outputStream);
  }
}

void Repl::printWelcomeMessage(std::ostream &outputStream) {
  outputStream << YELLOW << "Welcome to the " << GREEN << "elang" << YELLOW
               << " REPL!" << RESET << std::endl;
  outputStream << YELLOW
               << "Type `:exit` to exit, `:cls` to clear the screen, "
                  "and `:tree` to see the AST.\n";
}

bool Repl::handleSpecialCommands(const std::string &line) {
  if (line == ":cls") {
    system("clear");
    return true;
  } else if (line == ":tree") {
    seeTree = true;
    return true;
  }
  return false;
}

int Repl::countBraces(const std::string &line, char brace) {
  int count = 0;
  for (char c : line) {
    if (c == brace) {
      count++;
    }
  }
  return count;
}

bool Repl::isTreeVisible() const { return seeTree; }