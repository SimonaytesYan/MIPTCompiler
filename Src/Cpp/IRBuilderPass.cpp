#include "Grammar.hpp"
#include "IRBuilderPass.hpp"

#include <iostream>
#include "llvm/ADT/APInt.h"

llvm::Function* IRBuilderPass::buildIR(const GrammarUnit* unit) {
    return nullptr;
}

llvm::Value* IRBuilderPass::buildIRExpression(const ExpressionUnit* unit) {
    if (isGrammarUnitObject(unit))
        return buildIRObject(reinterpret_cast<const ObjectUnit*>(unit));
    if (isGrammarUnitOperator(unit))
        return buildIROperator(reinterpret_cast<const OperatorUnit*>(unit));

    std::cerr << "IRBuilderPass: unknown Expression type";
    return 0;
}

llvm::Value* IRBuilderPass::buildIROperator(const OperatorUnit* unit) {
    const BinaryOperUnit* bin_oper = dynamic_cast<const BinaryOperUnit*>(unit);
    if (bin_oper) {
        return buildIRBinaryOperator(bin_oper);
    }

    const UnaryOperUnit* unary_oper = dynamic_cast<const UnaryOperUnit*>(unit);
    if (unary_oper) {
        return buildIRUnaryOperator(unary_oper);
    }

    std::cerr << "IRBuilderPass: unknown operator type\n";
    return 0;
}

llvm::Value* IRBuilderPass::buildIRBinaryOperator(const BinaryOperUnit* unit) {
    llvm::Value* left_value = buildIRExpression(unit->left());
    llvm::Value* right_value = buildIRExpression(unit->right());

    switch (unit->getType())
    {
        case GrammarUnitType::ADD:
            return builder_->CreateAdd(left_value, right_value);
        case GrammarUnitType::SUB:
            return builder_->CreateSub(left_value, right_value);
        case GrammarUnitType::MUL:
            return builder_->CreateMul(left_value, right_value);
        case GrammarUnitType::DIV:
            return builder_->CreateSDiv(left_value, right_value);
        default:
            break;
    }

    std::cerr << "IRBuilderPass: unknown binary operator type\n";
    return nullptr;
}

llvm::Value* IRBuilderPass::buildIRUnaryOperator(const UnaryOperUnit* unit) {
    llvm::Value* zero = llvm::ConstantInt::get(*context_, llvm::APInt(32, 0, true));
    llvm::Value* operand = buildIRExpression(unit->operand());

    switch (unit->getType())
    {
        case GrammarUnitType::UNARY_MINUS:
            return builder_->CreateSub(zero, operand);
        default:
            break;
    }

    std::cerr << "IRBuilderPass: unknown unary operator type\n";
    return nullptr;
}

llvm::Value* IRBuilderPass::buildIRObject(const ObjectUnit* unit) {
    switch (unit->getType())
    {
    case GrammarUnitType::VAR:
        return buildIRVar(reinterpret_cast<const VarUnit*>(unit));
    case GrammarUnitType::NUM:
        return buildIRNum(reinterpret_cast<const NumUnit*>(unit));
    default:
        break;
    }

    std::cerr << "buildIRObject: unknown GrammarUnitType\n";
    return nullptr;
}

llvm::Value* IRBuilderPass::buildIRNum(const NumUnit* unit) {
    return llvm::ConstantInt::get(*context_, llvm::APInt(32, unit->num(), true));
}

llvm::Value* IRBuilderPass::buildIRVar(const VarUnit* unit) {
    llvm::Value *var = named_values_[unit->name()];
    if (!var) {
        std::cerr << "buildIRObject: Unknown variable\n";
        return nullptr;
    }

    return var;
}