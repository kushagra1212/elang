#pragma once
#include "../../Common.h"
#include "../../syntax/expression/ExpressionSyntax.h"
#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
#include "../BoundStatement/BoundStatement.h"

class BoundVariableDeclaration : public BoundStatement {
private:
  std::string _variable;
  std::shared_ptr<BoundExpression> _initializer;
  bool _isConst;

public:
  BoundVariableDeclaration(std::string variable, bool isConst,
                           std::shared_ptr<BoundExpression> initializer);
  BinderKindUtils::BoundNodeKind getKind() override;
  std::string getVariable() const;
  std::shared_ptr<BoundExpression> getInitializer() const;

  bool isConst() const;
};
