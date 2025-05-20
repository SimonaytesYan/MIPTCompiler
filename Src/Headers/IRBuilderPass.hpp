#pragma once

#include "Grammar.hpp"

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <map>
#include <memory>
#include <vector>

class IRBuilderPass {
  public:
    IRBuilderPass();

    void buildIR(const GrammarUnit* unit);
    void buildAndDumpIR(const GrammarUnit* unit, std::string_view output_file_name = "");

  private:
    llvm::Function* buildMain(const GrammarUnit* unit);
    void buildIRScope(const ScopeUnit* unit);

    void buildIRStatement(const StatementUnit* unit);
    void buildIRIf(const IfUnit* unit);
    void buildIRLoop(const LoopUnit* unit);
    void buildIRPrint(const PrintUnit* unit);
    void buildIRVarDecl(const VarDeclUnit* unit);
    void buildIRVarAssign(const VarAssignUnit* unit);

    llvm::Value* buildIRExpression(const ExpressionUnit* unit);
    llvm::Value* buildIROperator(const OperatorUnit* unit);
    llvm::Value* buildIRBinaryOperator(const BinaryOperUnit* unit);
    llvm::Value* buildIRUnaryOperator(const UnaryOperUnit* unit);

    llvm::Value* buildIRObject(const ObjectUnit* unit);
    llvm::Value* buildIRNum(const NumUnit* unit);
    llvm::Value* buildIRVar(const VarUnit* unit);

  private:
    llvm::Value* createLLVMInt(int value);
    llvm::Value* emitConditionCheck(const ExpressionUnit* unit);
    void emitVarAssign(llvm::AllocaInst* var, const ExpressionUnit* unit);
  
    llvm::AllocaInst* declareTypedVar(const ExpressionType* var_type);

    void AddStdLibFunctions();

  private:
    llvm::IRBuilder<> builder_;
    llvm::LLVMContext context_;
    llvm::Module module_;
    std::map<Variable*, llvm::AllocaInst*> named_values_;
};
