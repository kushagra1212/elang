#pragma once
#include "../Common.h"
#include "BoundNode.h"
class BoundExpression : public BoundNode {
public:
  virtual const std::type_info &getType() = 0;
};