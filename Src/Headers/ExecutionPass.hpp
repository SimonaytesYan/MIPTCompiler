#pragma once

#include "Grammar.hpp"
#include <string>
#include <vector>

class ExecutionPass {

  public:
    void Execute(GrammarUnit* unit);

  private:
    void ExecuteScope(ScopeUnit* unit);

    void ExecuteStatement(StatementUnit* unit);
    void ExecuteIf(IfUnit* unit);
    void ExecuteLoop(LoopUnit* unit);
    void ExecutePrint(PrintUnit* unit);
    void ExecuteVarDecl(VarDeclUnit* unit);
    void ExecuteVarAssign(VarAssignUnit* unit); 

    int ExecuteExpression(ExpressionUnit* unit);
    int ExecuteObject(ObjectUnit* unit);
    int ExecuteOperator(OperatorUnit* unit);
  private:
    // std::vector<> vars;
};
