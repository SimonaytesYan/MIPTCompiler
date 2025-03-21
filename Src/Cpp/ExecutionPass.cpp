#include "../Headers/ExecutionPass.hpp"

#include <iostream>
#include <assert.h>

void ExecutionPass::execute(GrammarUnit* unit) {
    if (unit == nullptr)
        return;

    if (isGrammarUnitStatement(unit))
        executeStatement(reinterpret_cast<StatementUnit*>(unit));
    else if (isGrammarUnitStatement(unit))
        executeScope(reinterpret_cast<ScopeUnit*>(unit));
}

void ExecutionPass::executeScope(ScopeUnit* scope) {
    vars_.startScope();

    for (StatementUnit* unit : *scope) {
        executeStatement(unit);
    }
}

void ExecutionPass::executeStatement(StatementUnit* unit) {
    switch(unit->getType())
    {
        case GrammarUnitType::IF:
            executeIf(reinterpret_cast<IfUnit*>(unit));
            break;
        case GrammarUnitType::LOOP:
            executeLoop(reinterpret_cast<LoopUnit*>(unit));
            break;
        case GrammarUnitType::VAR_DECL:
            executeVarDecl(reinterpret_cast<VarDeclUnit*>(unit));
            break;
        case GrammarUnitType::PRINT:
            executePrint(reinterpret_cast<PrintUnit*>(unit));
            break;
        case GrammarUnitType::VAR_ASSIGN:
            executeVarAssign(reinterpret_cast<VarAssignUnit*>(unit));
            break;

        default:
            std::cerr << "Error in ExecutionPass: unknown StatementUnitType\n";
            break;
    }
}

int ExecutionPass::executeExpression(ExpressionUnit* unit) {
    if (isGrammarUnitObject(unit))
        return executeObject(reinterpret_cast<ObjectUnit*>(unit));
    if (isGrammarUnitOperator(unit))
        return executeOperator(reinterpret_cast<OperatorUnit*>(unit));

    std::cerr << "Error in ExecutionPass: unknown Expression type\n";
    return 0;
}

int ExecutionPass::executeObject(ObjectUnit* unit) {
    switch (unit->getType())
    {
    case GrammarUnitType::VAR:
        return vars_.getVarValue(reinterpret_cast<VarUnit*>(unit)->name());
        return 0;

    case GrammarUnitType::NUM:
        return reinterpret_cast<NumUnit*>(unit)->num();
    default:
        break;
    }
    return 0;
}

int ExecutionPass::executeOperator(OperatorUnit* unit) {
    return 0;
}
