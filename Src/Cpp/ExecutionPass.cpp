#include "../Headers/ExecutionPass.hpp"

#include <iostream>
#include <assert.h>

void ExecutionPass::execute(const GrammarUnit* unit) {
    if (unit == nullptr)
        return;

    if (isGrammarUnitStatement(unit))
        executeStatement(reinterpret_cast<const StatementUnit*>(unit));
    else if (isGrammarUnitStatement(unit))
        executeScope(reinterpret_cast<const ScopeUnit*>(unit));
}

void ExecutionPass::executeScope(const ScopeUnit* scope) {
    vars_.startScope();

    for (StatementUnit* unit : *scope) {
        executeStatement(unit);
    }

    vars_.endScope();
}

void ExecutionPass::executeStatement(const StatementUnit* unit) {
    switch(unit->getType())
    {
        case GrammarUnitType::IF:
            executeIf(reinterpret_cast<const IfUnit*>(unit));
            break;
        case GrammarUnitType::LOOP:
            executeLoop(reinterpret_cast<const LoopUnit*>(unit));
            break;
        case GrammarUnitType::VAR_DECL:
            executeVarDecl(reinterpret_cast<const VarDeclUnit*>(unit));
            break;
        case GrammarUnitType::PRINT:
            executePrint(reinterpret_cast<const PrintUnit*>(unit));
            break;
        case GrammarUnitType::VAR_ASSIGN:
            executeVarAssign(reinterpret_cast<const VarAssignUnit*>(unit));
            break;

        default:
            std::cerr << "Error in ExecutionPass: unknown StatementUnitType\n";
            break;
    }
}

void ExecutionPass::executePrint(const PrintUnit* unit) {
    std::cout << unit->expression() << "\n";
}

void ExecutionPass::executeLoop(const LoopUnit* unit) {
    while (executeExpression(unit->condition())) {
        executeScope(unit->body());
    }
}

void ExecutionPass::executeIf(const IfUnit* unit) {
    if (executeExpression(unit->condition())) {
        executeScope(unit->true_branch());
    }
    else {
        executeScope(unit->false_branch());
    }
}

void ExecutionPass::executeVarDecl(const VarDeclUnit* unit) {
    IntVar var;
    var.value_ = executeExpression(unit->expr());
    var.name_ = unit->var()->name();

    vars_.addNewVar(var);
}


void ExecutionPass::executeVarAssign(const VarAssignUnit* unit) {
    vars_.setVarValue(unit->var()->name(), executeExpression(unit->expr()));
}

int ExecutionPass::executeExpression(const ExpressionUnit* unit) {
    if (isGrammarUnitObject(unit))
        return executeObject(reinterpret_cast<const ObjectUnit*>(unit));
    if (isGrammarUnitOperator(unit))
        return executeOperator(reinterpret_cast<const OperatorUnit*>(unit));

    std::cerr << "Error in ExecutionPass: unknown Expression type\n";
    return 0;
}

int ExecutionPass::executeObject(const ObjectUnit* unit) {
    switch (unit->getType())
    {
    case GrammarUnitType::VAR:
        return vars_.getVarValue(reinterpret_cast<const VarUnit*>(unit)->name());
        return 0;

    case GrammarUnitType::NUM:
        return reinterpret_cast<const NumUnit*>(unit)->num();
    default:
        break;
    }
    return 0;
}

int ExecutionPass::executeOperator(const OperatorUnit* unit) {

    const BinaryOperUnit* bin_oper = dynamic_cast<const BinaryOperUnit*>(unit);
    if (bin_oper) {
        switch (bin_oper->getType())
        {
            case GrammarUnitType::ADD:
                return executeExpression(bin_oper->left()) +
                    executeExpression(bin_oper->right());
            case GrammarUnitType::SUB:
                return executeExpression(bin_oper->left()) -
                    executeExpression(bin_oper->right());
            case GrammarUnitType::MUL:
                return executeExpression(bin_oper->left()) *
                    executeExpression(bin_oper->right());
            case GrammarUnitType::DIV:
                return executeExpression(bin_oper->left()) /
                    executeExpression(bin_oper->right());
            default:
                std::cerr << "Error in Execution Pass: unknown binary operator type\n";
                return 0;
        }
    }

    const UnaryOperUnit* unary_oper = dynamic_cast<const UnaryOperUnit*>(unit);
    if (unary_oper) {
        switch (unary_oper->getType())
        {
            case GrammarUnitType::UNARY_MINUS:
                return -executeExpression(unary_oper->operand());
            default:
                std::cerr << "Error in Execution Pass: unknown unary operator type\n";
                return 0;
        }
    }

    std::cerr << "Error in Execution Pass: unknown operator type\n";
}
