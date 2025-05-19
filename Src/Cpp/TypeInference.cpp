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

void inferInScope(ScopeUnit* unit) {
    TYPE_INF_ASSERT(false, "Not implemented!")
}

void inferInStatement(StatementUnit* unit) {
    TYPE_INF_ASSERT(false, "Not implemented!")
}

void inferInIf(StatementUnit* unit) {
    TYPE_INF_ASSERT(false, "Not implemented!")
}

void inferInPrint(StatementUnit* unit) {
    TYPE_INF_ASSERT(false, "Not implemented!")
}

void inferInLoop(StatementUnit* unit) {
    TYPE_INF_ASSERT(false, "Not implemented!")
}

void inferInVarDecl(StatementUnit* unit) {
    TYPE_INF_ASSERT(false, "Not implemented!")
}

void inferInVarAssign(StatementUnit* unit) {
    TYPE_INF_ASSERT(false, "Not implemented!")
}

void inferInExpresion(ExpressionUnit* unit) {
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

void inferInBinaryOp(BinaryOperUnit* unit) {
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

void inferInUnaryOp(UnaryOperUnit* unit) {
    inferInExpresion(unit->operand());
    TYPE_INF_ASSERT(isFloat(unit->operand()) || isInteger(unit->operand()),
                    "Not basic type in unary operation expr")
    unit->setExprType(unit->operand()->exprType()->copy());
}

void inferInObject(ObjectUnit* unit) {
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

void inferInVar(VarUnit* unit) {
    TYPE_INF_ASSERT(false, "Not implemented!")
}

void inferInNum(NumUnit* unit) {
    unit->setExprType(new IntegerExprType());
}

void inferInFloat(FloatUnit* unit) {
    unit->setExprType(new FloatExprType());
}

void inferInArray(ArrayUnit* unit) {
    ExpressionType* expr_type = nullptr;
    for (auto elem : unit->arrayElements()) {
        inferInExpresion(elem);
        if (expr_type == nullptr) {
            expr_type = elem->exprType();
        }
        else {
            TYPE_INF_ASSERT(expr_type == elem->exprType(), "Elements in array has different types")
        }
    }

    unit->setExprType(new ArrayVarType(unit->arrayElements().size(), expr_type));
}