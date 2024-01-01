#ifndef UTILS_H
#define UTILS_H
#include "../Common.h"
#include "../bind/BinderKindUtils.h"
#include "../bind/BoundLiteralExpression/BoundLiteralExpression.h"
#include "../bind/BoundNode.h"
#include "../bind/BoundStatement/BoundStatement.h"
#include "../diagnostics/DiagnosticUtils/DiagnosticUtils.h"
#include "../interpreter/InterpreterUtils/InterpreterConversions/InterpreterConversion.h"
#include "../syntax/CompilationUnitSyntax.h"
#include "../syntax/MemberSyntax.h"
#include "../syntax/SyntaxNode.h"
#include "../syntax/expression/LiteralExpressionSyntax.h"
#include "../syntax/statements/GlobalStatementSyntax/GlobalStatementSyntax.h"
#include <regex>

#include <filesystem>
#include <typeinfo>

namespace Utils {
enum type {
  INT8,
  INT16,
  INT32,
  INT64,
  DECIMAL,
  BOOL,
  STRING,
  NOTHING,
  UNKNOWN,

  // Container Types

  INT8_CONTAINER,
  INT16_CONTAINER,
  INT32_CONTAINER,
  INT64_CONTAINER,
  DECIMAL_CONTAINER,
  BOOL_CONTAINER,
  STRING_CONTAINER,
  UNKNOWN_CONTAINER,
};
void prettyPrint(SyntaxNode *node, std::string indent = "", bool isLast = true);

void prettyPrint(CompilationUnitSyntax *compilationUnit,
                 std::string indent = "", bool isLast = true);

void prettyPrint(BoundStatement *statement, std::string indent = "",
                 bool isLast = true);
void prettyPrint(BoundNode *statement, std::string indent = "",
                 bool isLast = true);
void printErrors(const std::vector<std::string> &errors,
                 std::ostream &outputStream, bool isWarning = false);

const std::string getFileName(const std::string &filePath);

const std::string concatErrors(const std::vector<std::string> &errors,
                               std::ostream &outputStream,
                               bool isWarning = false);
auto getStrongRandomString() -> std::string;
std::string getTypeString(const std::any &value);
Utils::type getTypeFromAny(const std::any &value);
std::string getSourceCode(CompilationUnitSyntax *node);
std::string getSourceCode(SyntaxNode *node, bool include);
std::string CE(const std::string &str);

Utils::type toContainerType(Utils::type type);
Utils::type toContainerElementType(Utils::type type);
auto isContainerType(Utils::type type) -> const bool;
auto isStaticTypedContainerType(Utils::type type) -> const bool;
auto isDynamicTypedContainerType(Utils::type type) -> const bool;
auto isStaticTypedPrimitiveType(Utils::type type) -> const bool;
auto isDynamicTypedPrimitiveType(Utils::type type) -> const bool;

auto isStaticTypedType(Utils::type type) -> const bool;
auto isDynamicTypedType(Utils::type type) -> const bool;

auto getFileContent(const std::string &filePath) -> std::string;
auto getSourceCodeFromFilePath(const std::string &filePath)
    -> std::vector<std::string>;
std::string getAbsoluteFilePath(std::string relativeFilePath);
std::vector<std::string> readLines(std::string absoluteFilePath);

void split(const std::string &s, const std::string &delim,
           std::vector<std::string> &tokens);

bool isInteger(const std::string &str);

bool isDouble(const std::string &str);
auto isSyntaxToken(SyntaxNode *node) -> bool;
auto typeToString(Utils::type type) -> std::string;

std::vector<std::string>
getAllFilesInDirectoryWithExtension(std::string directoryPath,
                                    std::string extension, bool recursive);
bool isSubstring(const std::string &s1, const std::string &s2);
enum class SymbolKind {
  Variable,
  Function,
  Parameter,
  Type,
  Namespace,
  Label,
  None
};

const std::string removeExtensionFromString(const std::string &filePath);
DiagnosticUtils::SourceLocation getSourceLocation(SyntaxToken<std::any> *token);

const std::string getNameExtension(const std::string &filePath);
const std::string getRelativePath(const std::string &filePath);
struct Variable {
  Utils::type type;
  std::any value;
  bool isConst;
  SymbolKind kind;
  Variable() = default;
  Variable(std::any value, bool isConst,
           Utils::type type = Utils::type::UNKNOWN) {
    this->value = value;
    this->isConst = isConst;
    this->kind = SymbolKind::Variable;
    this->type = type;
  }
};

class Node {
  static std::unordered_map<std::string, int> fileMap;
  static std::unordered_map<std::string, int> visitedMap;

public:
  static void addPath(std::string path) {
    fileMap[(path)] = 1;
    visitedMap[(path)] = 1;
  }

  static bool isPathVisited(std::string path) {
    return visitedMap.find((path)) != visitedMap.end();
  }

  static bool isPathAdded(std::string path) {
    return fileMap.find((path)) != fileMap.end();
  }

  static void clear() { fileMap.clear(); }

  static void removePath(std::string path) { fileMap.erase((path)); }

  static bool isPathExists(std::string path) {
    return std::filesystem::exists((path));
  }

  static bool isPathDirectory(std::string path) {
    return std::filesystem::is_directory((path));
  }

  static bool isPathRegularFile(std::string path) {
    return std::filesystem::is_regular_file((path));
  }

  static bool isPathSymlink(std::string path) {
    return std::filesystem::is_symlink((path));
  }

  static bool isPathEmpty(std::string path) {
    return std::filesystem::is_empty((path));
  }

  static bool isPathRelative(std::string path) {
    return std::filesystem::path((path)).is_relative();
  }

  static bool isPathAbsolute(std::string path) {
    return std::filesystem::path((path)).is_absolute();
  }

  static bool isCycleDetected(std::string path) { return fileMap[(path)] >= 1; }
};

} // namespace Utils

#endif // UTILS_H