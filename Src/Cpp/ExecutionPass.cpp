#include "../Headers/ExecutionPass.hpp"

#include <iostream>

void ExecutionPass::Execute(GrammarUnit* unit) {
    if (unit == nullptr)
        return;

    if (isGrammarUnitStatement(unit))
        ExecuteStatement(reinterpret_cast<StatementUnit*>(unit));
    else if (isGrammarUnitStatement(unit))
        ExecuteScope(reinterpret_cast<ScopeUnit*>(unit));
}

void ExecutionPass::ExecuteStatement(StatementUnit* unit) {
    switch(unit->getType())
    {
        case GrammarUnitType::IF:
            ExecuteIf(reinterpret_cast<IfUnit*>(unit));
            break;
        case GrammarUnitType::LOOP:
            ExecuteLoop(reinterpret_cast<LoopUnit*>(unit));
            break;
        case GrammarUnitType::VAR_DECL:
            ExecuteVarDecl(reinterpret_cast<VarDeclUnit*>(unit));
            break;
        case GrammarUnitType::PRINT:
            ExecutePrint(reinterpret_cast<PrintUnit*>(unit));
            break;
        case GrammarUnitType::VAR_ASSIGN:
            ExecuteVarAssign(reinterpret_cast<VarAssignUnit*>(unit));
            break;

        default:
            std::cerr << "Error in ExecutionPass: unknown StatementUnitType\n";
            break;
    }
}

int ExecutionPass::ExecuteExpression(ExpressionUnit* unit) {
    return 0;
}

void ExecutionPass::ExecuteScope(ScopeUnit* scope) {
    for (StatementUnit* unit : *scope) {
        ExecuteStatement(unit);
    }
}
