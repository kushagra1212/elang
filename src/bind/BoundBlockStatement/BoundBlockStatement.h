#pragma once
#include "../../Common.h"
#include "../BinderKindUtils.h"
#include "../BoundNode.h"
#include "../BoundSourceLocation/BoundSourceLocation.h"
#include "../BoundStatement/BoundStatement.h"

class BoundBlockStatement : public BoundStatement, public BoundSourceLocation {
private:
  std::vector<std::unique_ptr<BoundStatement>> _statements;
  bool _global;

public:
  BoundBlockStatement(const DiagnosticUtils::SourceLocation &location,
                      bool global);
  BoundBlockStatement(const DiagnosticUtils::SourceLocation &location);

  std::vector<std::unique_ptr<BoundStatement>> &getStatements();
  void addStatement(std::unique_ptr<BoundStatement> statement);
  bool getGlobal() const;

  BinderKindUtils::BoundNodeKind getKind() const;

  std::vector<BoundNode *> getChildren() override;
};