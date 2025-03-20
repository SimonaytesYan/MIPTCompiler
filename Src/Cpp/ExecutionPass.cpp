#include "../Headers/ExecutionPass.hpp"

void ExecutionPass::Execute(GrammarUnit* unit) {
    if (unit == nullptr)
        return;

    if (isGrammarUnitStatement(unit))
        ExecuteStatement(reinterpret_cast<StatementUnit*>(unit));
    else if (isGrammarUnitStatement(unit))
        ExecuteScope(reinterpret_cast<ScopeUnit*>(unit));
}

void ExecutionPass::ExecuteStatement(StatementUnit* unit) {

}

int ExecutionPass::ExecuteExpression(ExpressionUnit* unit) {
    return 0;
}

void ExecutionPass::ExecuteScope(ScopeUnit* unit) {

}
