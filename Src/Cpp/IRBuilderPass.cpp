#include "Grammar.hpp"
#include "IRBuilderPass.hpp"

#include <iostream>
#include "llvm/ADT/APInt.h"

llvm::Function* IRBuilderPass::buildIR(const GrammarUnit* unit) {
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
        std::cerr << "buildIRObject: unknown GrammarUnitType\n";
        break;
    }
    return nullptr;
}

llvm::Value* IRBuilderPass::buildIRNum(const NumUnit* unit) {
    return llvm::ConstantInt::get(*context_, llvm::APInt(32, unit->num(), true));
}

llvm::Value* IRBuilderPass::buildIRVar(const VarUnit* unit) {
    llvm::Value *var = named_values_[unit->name()];
    if (!var) {
        std::cerr << "Unknown variable\n";
        return nullptr;
    }

    return var;
}