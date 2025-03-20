#include "../Headers/ExecutionPass.hpp"

#include <iostream>
#include <assert.h>

void ExecutionPass::Execute(GrammarUnit* unit) {
    if (unit == nullptr)
        return;

    if (isGrammarUnitStatement(unit))
        ExecuteStatement(reinterpret_cast<StatementUnit*>(unit));
    else if (isGrammarUnitStatement(unit))
        ExecuteScope(reinterpret_cast<ScopeUnit*>(unit));
}

void ExecutionPass::ExecuteScope(ScopeUnit* scope) {
    for (StatementUnit* unit : *scope) {
        ExecuteStatement(unit);
    }
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
    if (isGrammarUnitObject(unit))
        return ExecuteObject(reinterpret_cast<ObjectUnit*>(unit));
    if (isGrammarUnitOperator(unit))
        return ExecuteOperator(reinterpret_cast<OperatorUnit*>(unit));

    std::cerr << "Error in ExecutionPass: unknown Expression type\n";
    return 0;
}

int ExecutionPass::ExecuteObject(ObjectUnit* unit) {
    switch (unit->getType())
    {
    case GrammarUnitType::VAR:
        return vars_.GetVarValue(reinterpret_cast<VarUnit*>(unit)->name());
        return 0;

    case GrammarUnitType::NUM:
        return reinterpret_cast<NumUnit*>(unit)->num();
    default:
        break;
    }
    return 0;
}

int ExecutionPass::ExecuteOperator(OperatorUnit* unit) {
    return 0;
}
