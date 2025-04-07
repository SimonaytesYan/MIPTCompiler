#include "Grammar.hpp"
#include "IRBuilderPass.hpp"

#include <iostream>
#include "llvm/ADT/APInt.h"


void IRBuilderPass::buildIR(const GrammarUnit* unit) {
    // TODO init modules

    // TODO generate main

    module_->print(llvm::outs(), nullptr);
}

llvm::Function* IRBuilderPass::buildMain(const GrammarUnit* unit) {
    llvm::FunctionType *func_type =
        llvm::FunctionType::get(llvm::Type::getVoidTy(*context_), false);

    llvm::Function* func =
        llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                               "my_main", module_.get());
    if (func == nullptr) {
        std::cerr << "IRBuilder: error in main function creation\n";
        return nullptr;
    }

    llvm::BasicBlock* entry_basic_block =
        llvm::BasicBlock::Create(*context_, "entry", func);
    builder_->SetInsertPoint(entry_basic_block);

    named_values_.clear();

    buildIRScope(reinterpret_cast<const ScopeUnit*>(unit));
    builder_->CreateRetVoid();

    return func;
}

void IRBuilderPass::buildIRScope(const ScopeUnit* unit) {

    for (StatementUnit* unit : *unit) {
        buildIRStatement(unit);
    }
}

void IRBuilderPass::buildIRStatement(const StatementUnit* unit) {
    switch(unit->getType())
    {
        case GrammarUnitType::IF:
            buildIRIf(reinterpret_cast<const IfUnit*>(unit));
            break;
        case GrammarUnitType::LOOP:
            buildIRLoop(reinterpret_cast<const LoopUnit*>(unit));
            break;
        case GrammarUnitType::VAR_DECL:
            buildIRVarDecl(reinterpret_cast<const VarDeclUnit*>(unit));
            break;
        case GrammarUnitType::PRINT:
            buildIRPrint(reinterpret_cast<const PrintUnit*>(unit));
            break;
        case GrammarUnitType::VAR_ASSIGN:
            buildIRVarAssign(reinterpret_cast<const VarAssignUnit*>(unit));
            break;

        default:
            break;
    }
    std::cerr << "IRBuilderPass: unknown StatementUnitType\n";
}


llvm::Value* IRBuilderPass::buildIRVarDecl(const VarDeclUnit* unit) {
    return nullptr; // TODO implement
}

llvm::Value* IRBuilderPass::buildIRVarAssign(const VarAssignUnit* unit) {
    return nullptr; // TODO implement
}

void IRBuilderPass::buildIRIf(const IfUnit* unit) {

    llvm::Value* cond_value = buildIRExpression(unit->condition());
    llvm::Value* one = getLLVMInt(1);
    cond_value =
        builder_->CreateCmp(llvm::CmpInst::Predicate::ICMP_EQ, cond_value, one);

    llvm::Function* func = builder_->GetInsertBlock()->getParent();

    llvm::BasicBlock* true_branch_block =
        llvm::BasicBlock::Create(*context_, "true_branch", func);
    llvm::BasicBlock* false_branch_block =
        llvm::BasicBlock::Create(*context_, "false_branch", func);
    llvm::BasicBlock* final_block =
        llvm::BasicBlock::Create(*context_, "result_branch", func);

    builder_->CreateCondBr(cond_value, true_branch_block, false_branch_block);

    builder_->SetInsertPoint(true_branch_block);
    buildIRScope(unit->true_branch());
    builder_->CreateBr(final_block);

    builder_->SetInsertPoint(false_branch_block);
    buildIRScope(unit->false_branch());
    builder_->CreateBr(final_block);
}

llvm::Value* IRBuilderPass::getLLVMInt(int value) {
    return llvm::ConstantInt::get(*context_, llvm::APInt(32, value, true));
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
            return builder_->CreateAdd(left_value, right_value, "addtmp");
        case GrammarUnitType::SUB:
            return builder_->CreateSub(left_value, right_value, "subtmp");
        case GrammarUnitType::MUL:
            return builder_->CreateMul(left_value, right_value, "multmp");
        case GrammarUnitType::DIV:
            return builder_->CreateSDiv(left_value, right_value, "divtmp");
        default:
            break;
    }

    std::cerr << "IRBuilderPass: unknown binary operator type\n";
    return nullptr;
}

llvm::Value* IRBuilderPass::buildIRUnaryOperator(const UnaryOperUnit* unit) {
    llvm::Value* zero = getLLVMInt(0);
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
    return getLLVMInt(unit->num());
}

llvm::Value* IRBuilderPass::buildIRVar(const VarUnit* unit) {
    llvm::Value *var = named_values_[unit->name()];
    if (!var) {
        std::cerr << "buildIRObject: Unknown variable\n";
        return nullptr;
    }

    return var;
}