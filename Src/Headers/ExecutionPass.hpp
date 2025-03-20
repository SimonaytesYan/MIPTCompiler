#pragma once

#include "Grammar.hpp"
#include <string>
#include <vector>

class ExecutionPass {

  void Execute(GrammarUnit* unit);
  void ExecuteStatement(StatementUnit* unit);
  int ExecuteExpression(ExpressionUnit* unit);
  void ExecuteScope(ScopeUnit* unit);

  private:
    // std::vector<> vars;
};
