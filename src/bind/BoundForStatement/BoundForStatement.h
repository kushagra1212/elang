#ifndef BOUND_FOR_STATEMENT_H
#define BOUND_FOR_STATEMENT_H

#include "../BinderKindUtils.h"
#include "../BoundExpression.h"
#include "../BoundLiteralExpression/BoundLiteralExpression.h"
#include "../BoundStatement/BoundStatement.h"
#include "../BoundVariableDeclaration/BoundVariableDeclaration.h"

class BoundForStatement : public BoundStatement {

public:
  BoundForStatement(std::shared_ptr<BoundStatement> initialization,
                    std::shared_ptr<BoundExpression> upperBound,

                    std::shared_ptr<BoundStatement> statement);

  std::shared_ptr<BoundStatement> getInitialization() const;

  std::shared_ptr<BoundStatement> getStatement() const;

  std::shared_ptr<BoundExpression> getUpperBound() const;

  BinderKindUtils::BoundNodeKind getKind() override;

private:
  std::shared_ptr<BoundStatement> initialization;

  std::shared_ptr<BoundStatement> statement;

  std::shared_ptr<BoundExpression> upperBound;
};

#endif // BOUND_FOR_STATEMENT_H