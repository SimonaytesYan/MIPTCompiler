#pragma once

#include "Grammar.hpp"

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include <map>
#include <memory>

class IRBuilderPass {
  public:
    IRBuilderPass();

    void buildIR(const GrammarUnit* unit);
    llvm::Function* buildMain(const GrammarUnit* unit);

  private:
    void buildIRScope(const ScopeUnit* unit);

    void buildIRStatement(const StatementUnit* unit);
    void buildIRIf(const IfUnit* unit);
    void buildIRLoop(const LoopUnit* unit);
    void buildIRPrint(const PrintUnit* unit);
    llvm::Value* buildIRVarDecl(const VarDeclUnit* unit);
    llvm::Value* buildIRVarAssign(const VarAssignUnit* unit);

    llvm::Value* buildIRExpression(const ExpressionUnit* unit);
    llvm::Value* buildIROperator(const OperatorUnit* unit);
    llvm::Value* buildIRBinaryOperator(const BinaryOperUnit* unit);
    llvm::Value* buildIRUnaryOperator(const UnaryOperUnit* unit);

    llvm::Value* buildIRObject(const ObjectUnit* unit);
    llvm::Value* buildIRNum(const NumUnit* unit);
    llvm::Value* buildIRVar(const VarUnit* unit);

  private:
    llvm::Value* getLLVMInt(int value);

  private:
    llvm::IRBuilder<> builder_;
    llvm::LLVMContext context_;
    llvm::Module module_;
    std::map<std::string, llvm::Value*> named_values_;
};
