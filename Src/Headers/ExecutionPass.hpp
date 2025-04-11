#pragma once

#include "Grammar.hpp"
#include "VariableList.hpp"

#include <iostream>
#include <string>
#include <variant>
#include <vector>

class ExecutionPass {

  public:
    void execute(const GrammarUnit* unit);

  private:
    void executeScope(const ScopeUnit* unit);

    void executeStatement(const StatementUnit* unit);
    void executeIf(const IfUnit* unit);
    void executeLoop(const LoopUnit* unit);
    void executePrint(const PrintUnit* unit);
    void executeVarDecl(const VarDeclUnit* unit);
    void executeVarAssign(const VarAssignUnit* unit);

    int executeExpression(const ExpressionUnit* unit);
    int executeObject(const ObjectUnit* unit);
    int executeOperator(const OperatorUnit* unit);

  private:
    VariableList vars_;
};
