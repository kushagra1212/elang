
#ifndef REPL_H
#define REPL_H
#include "../Common.h"
#include "../compiler/Evaluator.h"
#include "../parser/Parser.h"
#include "../utils/Utils.h"
class Repl {
public:
  Repl();
  ~Repl();

  // Run the REPL with default input/output streams
  void run();

  // Run the REPL with custom input/output streams for testing
  void runWithStream(std::istream &inputStream, std::ostream &outputStream);
  void runForTest(std::istream &inputStream, std::ostream &outputStream);
  bool isTreeVisible() const;

  // Helper functions
  void printWelcomeMessage(std::ostream &outputStream);
  void processInput(const std::string &line);
  bool handleSpecialCommands(const std::string &line);
  int countBraces(const std::string &line, char brace);

  void compileAndEvaluate(CompilationUnitSyntax *compilationUnit,
                          std::ostream &outputStream);

  void compileAndEvaluateForTests(const std::string &line,
                                  std::ostream &outputStream);
  // Data members
private:
  std::shared_ptr<Evaluator> previousEvaluator;
  bool seeTree;
  std::vector<std::string> text;
  int braceCount;
};

#endif // REPL_H