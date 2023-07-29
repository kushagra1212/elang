
#pragma once
#include "../../syntax/CompilationUnitSyntax.h"
#include "../../syntax/SyntaxKindUtils.h"
#include "../../syntax/expression/AssignmentExpressionSyntax.h"
#include "../../syntax/expression/BinaryExpressionSyntax.h"
#include "../../syntax/expression/CallExpressionSyntax/CallExpressionSyntax.h"
#include "../../syntax/expression/ExpressionSyntax.h"
#include "../../syntax/expression/LiteralExpressionSyntax.h"
#include "../../syntax/expression/ParenthesizedExpressionSyntax.h"
#include "../../syntax/expression/UnaryExpressionSyntax.h"
#include "../../syntax/expression/VariableExpressionSyntax.h"
#include "../../syntax/statements/BlockStatementSyntax/BlockStatementSyntax.h"
#include "../../syntax/statements/ExpressionStatementSyntax/ExpressionStatementSyntax.h"
#include "../../syntax/statements/ForStatementSyntax/ForStatementSyntax.h"
#include "../../syntax/statements/IfStatementSyntax/IfStatementSyntax.h"
#include "../../syntax/statements/StatementSyntax.h"
#include "../../syntax/statements/VariableDeclarationSyntax/VariableDeclarationSyntax.h"
#include "../../syntax/statements/WhileStatementSyntax/WhileStatementSyntax.h"
#include "../../utils/Utils.h"
#include "../BinderKindUtils.h"
#include "../BoundAssignmentExpression/BoundAssignmentExpression.h"
#include "../BoundBinaryExpression/BoundBinaryExpression.h"
#include "../BoundBlockStatement/BoundBlockStatement.h"
#include "../BoundCallExpression/BoundCallExpression.h"
#include "../BoundExpression.h"
#include "../BoundExpressionStatement/BoundExpressionStatement.h"
#include "../BoundForStatement/BoundForStatement.h"
#include "../BoundIfStatement/BoundIfStatement.h"
#include "../BoundLiteralExpression/BoundLiteralExpression.h"
#include "../BoundStatement/BoundStatement.h"
#include "../BoundUnaryExpression/BoundUnaryExpression.h"
#include "../BoundVariableDeclaration/BoundVariableDeclaration.h"
#include "../BoundVariableExpression/BoundVariableExpression.h"
#include "../BoundWhileStatement/BoundWhileStatement.h"
#include "BoundScope/BoundScope.h"
#include "BoundScopeGlobal/BoundScopeGlobal.h"
#include <stack>
class Binder {
private:
  std::vector<std::string> logs;

private:
  BoundScope *root;

public:
  Binder(BoundScope *root);

public:
  BoundExpression *bindExpression(ExpressionSyntax *syntax);

public:
  static BoundScopeGlobal *
  bindGlobalScope(BoundScopeGlobal *previous,
                  std::shared_ptr<CompilationUnitSyntax> syntax);

public:
  static BoundScope *CreateParentScope(BoundScopeGlobal *parent);

public:
  BoundStatement *bindStatement(StatementSyntax *syntax);
};
