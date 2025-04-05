#pragma once

#include "Grammar.hpp"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include <memory>


class IRBuilderPass {

  public:
    void buildIR(const GrammarUnit* unit);

  private:
    void buildIRScope(const ScopeUnit* unit);

    void buildIRStatement(const StatementUnit* unit);
    void buildIRIf(const IfUnit* unit);
    void buildIRLoop(const LoopUnit* unit);
    void buildIRPrint(const PrintUnit* unit);
    void buildIRVarDecl(const VarDeclUnit* unit);
    void buildIRVarAssign(const VarAssignUnit* unit);

    int buildIRExpression(const ExpressionUnit* unit);
    int buildIRObject(const ObjectUnit* unit);
    int buildIROperator(const OperatorUnit* unit);


  private:
    std::unique_ptr<IRBuilder<>> builder_;
    std::unique_ptr<LLVMContext> context_;
};