#include "TypeInference.hpp"
#include "Utils.hpp"

#include <iostream>

static bool isBasicType(ExpressionUnit* unit) {
    return unit->exprType()->typeClass() == ExpressionType::TypeClass::BASIC;
}

static bool isFloat(ExpressionUnit* unit) {
    return isBasicType(unit) &&
           (static_cast<BasicExprType*>(unit->exprType())->basicType() == 
            BasicExprType::BasicType::FLOAT);
}

static bool isInteger(ExpressionUnit* unit) {
    return isBasicType(unit) &&
           (static_cast<BasicExprType*>(unit->exprType())->basicType() == 
            BasicExprType::BasicType::INTEGER);
}

void TypeSystem::inferInScope(ScopeUnit* unit) {
    var_table_.startScope();
    for (StatementUnit* statement : unit) {
        inferInStatement(statement);
    }
    var_table_.endScope();
}

void TypeSystem::inferInStatement(StatementUnit* unit) {
    switch (unit->getType())
    {
    case GrammarUnitType::IF:
        inferInIf(static_cast<IfUnit*>(unit));
        break;
    case GrammarUnitType::LOOP:
        inferInLoop(static_cast<LoopUnit*>(unit));
        break;
    case GrammarUnitType::PRINT:
        inferInPrint(static_cast<PrintUnit*>(unit));
        break;
    case GrammarUnitType::VAR_DECL:
        inferInVarDecl(static_cast<VarDeclUnit*>(unit));
        break;
    case GrammarUnitType::VAR_ASSIGN:
        inferInVarAssign(static_cast<VarAssignUnit*>(unit));
        break;
    
    default:
        break;
    }
}

void TypeSystem::inferInIf(IfUnit* unit) {
    inferInExpresion(unit->expression());
    inferInScope(unit->true_branch());
    inferInScope(unit->false_branch());
}

void TypeSystem::inferInPrint(PrintUnit* unit) {
    inferInExpresion(unit->expression())
}

void TypeSystem::inferInLoop(LoopUnit* unit) {
    inferInExpresion(unit->condition())
    inferInScope(unit->body())
}

void TypeSystem::inferInVarDecl(VarDeclUnit* unit) {
    inferInExpresion(unit->expr());
    unit->var()->setExprType(unit->expr()->exprType()->copy());

    unit->updateNameTypeVariable();
    unit->var()->setVariable(unit->getVariable());

    var_table_.insertVar(unit->getVariable());
}

void TypeSystem::inferInVarAssign(VarAssignUnit* unit) {
    inferInExpresion(unit->expr());

    Variable* variable = var_table_.findVar(unit->var()->name());
    unit->var()->setVariable(variable);
    
    TYPE_INF_ASSERT((ExpressionType::isEqual(variable->type(), unit->expr()->exprType())), 
                    "Variable type is not equal to siignment expression")
}

void TypeSystem::inferInExpresion(ExpressionUnit* unit) {
    if (isGrammarUnitObject(unit)) {
        inferInObject(static_cast<ObjectUnit*>(unit));
    }
    else if (isGrammarUnitBinaryOperator(unit)) {
        inferInBinaryOp(static_cast<BinaryOperUnit*>(unit));
    }
    else if (isGrammarUnitUnaryOperator(unit)) {
        inferInUnaryOp(static_cast<UnaryOperUnit*>(unit));
    }
}

void TypeSystem::inferInBinaryOp(BinaryOperUnit* unit) {
    inferInExpresion(unit->left());
    inferInExpresion(unit->right());
    
    TYPE_INF_ASSERT(isFloat(unit->left()) || isInteger(unit->left()),
                    "Left operand type is not int or float");
    TYPE_INF_ASSERT(isFloat(unit->right()) || isInteger(unit->right()),
                    "Right operand type is not int or float");

    if (isFloat(unit->right()) || isFloat(unit->left())) {
        unit->setExprType(new FloatExprType());
    }
    else {
        unit->setExprType(new IntegerExprType());
    }
}

void TypeSystem::inferInUnaryOp(UnaryOperUnit* unit) {
    inferInExpresion(unit->operand());
    TYPE_INF_ASSERT(isFloat(unit->operand()) || isInteger(unit->operand()),
                    "Not basic type in unary operation expr")
    unit->setExprType(unit->operand()->exprType()->copy());
}

void TypeSystem::inferInObject(ObjectUnit* unit) {
    switch (unit->getType())
    {
    case GrammarUnitType::NUM:
        inferInNum(static_cast<NumUnit*>(unit));
        break;
    case GrammarUnitType::FLOAT:
        inferInFloat(static_cast<FloatUnit*>(unit));
        break;
    case GrammarUnitType::VAR:
        inferInVar(static_cast<VarUnit*>(unit));
        break;
    case GrammarUnitType::ARRAY:
        inferInArray(static_cast<ArrayUnit*>(unit));
        break;
    
    default:
        break;
    }
}

void TypeSystem::inferInVar(VarUnit* unit) {
    TYPE_INF_ASSERT(false, "Not implemented!")
}

void TypeSystem::inferInNum(NumUnit* unit) {
    unit->setExprType(new IntegerExprType());
}

void TypeSystem::inferInFloat(FloatUnit* unit) {
    unit->setExprType(new FloatExprType());
}

void TypeSystem::inferInArray(ArrayUnit* unit) {
    ExpressionType* expr_type = nullptr;
    for (auto elem : unit->arrayElements()) {
        inferInExpresion(elem);
        if (expr_type == nullptr) {
            expr_type = elem->exprType();
        }
        else {
            TYPE_INF_ASSERT(expr_type == elem->exprType(),
                            "Elements in array has different types")
        }
    }

    unit->setExprType(new ArrayVarType(unit->arrayElements().size(), expr_type));
}

Variable* TypeSystem::VarTable::findVar(const std::string& name) {
    // Go from top of the 'stack' to the end
    auto begin_it = named_expr_types_.rbegin();
    auto end_it = named_expr_types_.rend();

    for (auto scope_it = begin_it; scope_it != end_it; ++scope_it) {
        Variable* var_in_cur_scope = (*scope_it)[name];
        if (var_in_cur_scope != nullptr) {
            return var_in_cur_scope;
        }
    }

    return nullptr;
}

void TypeSystem::VarTable::startScope() {
    named_expr_types_.push_back(std::map<std::string, Variable*>());
}

void TypeSystem::VarTable::endScope() {
    named_expr_types_.pop_back();
}

void TypeSystem::VarTable::insertVar(Variable* var) {
    named_expr_types_.push_back(var);
}