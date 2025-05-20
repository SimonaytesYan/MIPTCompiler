#include "Grammar.hpp"
#include "IRBuilderPass.hpp"

#include <iostream>
#include "llvm/ADT/APInt.h"

IRBuilderPass::IRBuilderPass() :
    context_(),
    module_("my cool jit", context_),
    builder_(context_) { }

void IRBuilderPass::buildIR(const GrammarUnit* unit) {
    AddStdLibFunctions();

    buildMain(unit);
}

void IRBuilderPass::buildAndDumpIR(const GrammarUnit* unit, std::string_view output_file_name) {
    buildIR(unit);

    std::error_code error_code;
    llvm::raw_fd_ostream* out = &llvm::outs();
    if (output_file_name != "") {
        out = new llvm::raw_fd_ostream(output_file_name, error_code);
    }
    module_.print(*out, nullptr);

    if (output_file_name != "")
        delete out;
}

void IRBuilderPass::AddStdLibFunction(std::vector<llvm::Type*> arg_types, const char* name) {
    llvm::FunctionType* func_type =
        llvm::FunctionType::get(llvm::Type::getVoidTy(context_), arg_types, false);

    llvm::Function* func =
        llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                               name, module_);
    if (func == nullptr) {
        std::cerr << "IRBuilder: fail to add std lib function `" << name << "`\n";
        return;
    }
}

void IRBuilderPass::AddStdLibFunctions() {
    AddStdLibFunction(std::vector<llvm::Type*>(1, llvm::Type::getInt32Ty(context_)), "print_int");
    AddStdLibFunction(std::vector<llvm::Type*>(1, llvm::Type::getFloatTy(context_)), "print_float");
    AddStdLibFunction(std::vector<llvm::Type*>(1, llvm::PointerType::get(llvm::Type::getInt8Ty(context_), 0)), "print_string");
}

llvm::Function* IRBuilderPass::buildMain(const GrammarUnit* unit) {
    llvm::FunctionType* func_type =
        llvm::FunctionType::get(llvm::Type::getVoidTy(context_), false);

    llvm::Function* func =
        llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                               "main", module_);
    if (func == nullptr) {
        std::cerr << "IRBuilder: error in main function creation\n";
        return nullptr;
    }

    llvm::BasicBlock* entry_basic_block =
        llvm::BasicBlock::Create(context_, "entry", func);
    builder_.SetInsertPoint(entry_basic_block);

    buildIRScope(reinterpret_cast<const ScopeUnit*>(unit));

    builder_.CreateRetVoid();

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
            std::cerr << "IRBuilderPass: unknown StatementUnitType\n";
            break;
    }
}

llvm::Value* IRBuilderPass::emitConditionCheck(const ExpressionUnit* unit) {
    llvm::Value* cond_value = buildIRExpression(unit);
    llvm::Value* one = createLLVMInt(0);
    cond_value =
        builder_.CreateCmp(llvm::CmpInst::Predicate::ICMP_NE, cond_value, one);
    return cond_value;
}

void IRBuilderPass::buildIRIf(const IfUnit* unit) {
    llvm::Function* func = builder_.GetInsertBlock()->getParent();

    // Create all basic blocks
    llvm::BasicBlock* true_branch_block =
        llvm::BasicBlock::Create(context_, "if_true_branch", func);
    llvm::BasicBlock* false_branch_block =
        llvm::BasicBlock::Create(context_, "if_false_branch");
    llvm::BasicBlock* final_block =
        llvm::BasicBlock::Create(context_, "if_result_branch");

    // Emit if condition
    llvm::Value* cond_value = emitConditionCheck(unit->condition());
    builder_.CreateCondBr(cond_value, true_branch_block, false_branch_block);

    // Emit true branch
    builder_.SetInsertPoint(true_branch_block);
    buildIRScope(unit->true_branch());

    // true_branch_block = builder_.GetInsertBlock();
    builder_.CreateBr(final_block);

    // Emit false branch
    func->insert(func->end(), false_branch_block);

    builder_.SetInsertPoint(false_branch_block);
    buildIRScope(unit->false_branch());

    // false_branch_block = builder_.GetInsertBlock();
    builder_.CreateBr(final_block);

    // Emit final block
    func->insert(func->end(), final_block);
    builder_.SetInsertPoint(final_block);
}

void IRBuilderPass::buildIRLoop(const LoopUnit* unit) {
    // Create all basic blocks
    llvm::Function* func = builder_.GetInsertBlock()->getParent();

    // Create all basic blocks
    llvm::BasicBlock* cond_block =
        llvm::BasicBlock::Create(context_, "loop_cond", func);;
    llvm::BasicBlock* loop_block =
        llvm::BasicBlock::Create(context_, "loop_body");
    llvm::BasicBlock* final_block =
        llvm::BasicBlock::Create(context_, "loop_result");

    builder_.CreateBr(cond_block);

    // Emit loop condition
    builder_.SetInsertPoint(cond_block);
    llvm::Value* cond_value = emitConditionCheck(unit->condition());
    builder_.CreateCondBr(cond_value, loop_block, final_block);

    // Emit loop body
    func->insert(func->end(), loop_block);
    builder_.SetInsertPoint(loop_block);
    buildIRScope(unit->body());
    builder_.CreateBr(cond_block);

    // Emit final basic block
    func->insert(func->end(), final_block);
    builder_.SetInsertPoint(final_block);
}

void IRBuilderPass::buildIRPrint(const PrintUnit* unit) {
    if (unit->expression()->exprType()->typeClass() != ExpressionType::TypeClass::BASIC) {
        std::cerr << "buildIRPrint: fail to print this type\n";
        return;
    }
    const BasicExprType* basic = static_cast<const BasicExprType*>(unit->expression()->exprType());
    
    llvm::Function *callee_func = nullptr;
    switch (basic->basicType()) {
        case BasicExprType::BasicType::INTEGER:
            callee_func = module_.getFunction("print_int");
            break;
        case BasicExprType::BasicType::FLOAT:
            callee_func = module_.getFunction("print_float");
            break;
        case BasicExprType::BasicType::STRING:
            callee_func = module_.getFunction("print_string");
            break;
    }
    if (!callee_func) {
        std::cerr << "IRBuilder: Faild to find neccesery function\n";
        return;
    }

    // Emit argument
    llvm::Value* argument = buildIRExpression(unit->expression());
    // Emit call
    builder_.CreateCall(callee_func, argument);
}

llvm::Type* IRBuilderPass::translateToLLVMType(const ExpressionType* var_type, const ExpressionUnit* unit) {
    switch (var_type->typeClass())
    {
        case ExpressionType::TypeClass::BASIC: {
            const BasicExprType* basic = static_cast<const BasicExprType*>(var_type);
            switch (basic->basicType()) {
                case BasicExprType::BasicType::INTEGER:
                    return llvm::Type::getInt32Ty(context_);
                case BasicExprType::BasicType::FLOAT:
                    return llvm::Type::getFloatTy(context_);
                case BasicExprType::BasicType::STRING: {
                    const StringUnit* str_unit = static_cast<const StringUnit*>(unit);
                    return llvm::ArrayType::get(llvm::Type::getInt8Ty(context_), str_unit->str().size());
                }
            }
            return nullptr;
        }

        case ExpressionType::TypeClass::ARRAY: {
            const ArrayVarType* array = static_cast<const ArrayVarType*>(var_type);
            
            if (array->elemNum() == 0) {
                return llvm::ArrayType::get(llvm::Type::getInt32Ty(context_), 0);
            }
            return llvm::ArrayType::get(translateToLLVMType(array->elemType(), static_cast<const ArrayUnit*>(unit)->arrayElements()[0]), array->elemNum());
        }
    }

    return nullptr;
}

void IRBuilderPass::buildIRVarDecl(const VarDeclUnit* unit) {
    const std::string name = unit->var()->name();

    llvm::AllocaInst* alloc_var =
        builder_.CreateAlloca(translateToLLVMType(unit->expr()->exprType(), unit->expr()), nullptr, name);
    
    named_values_[unit->variable()] = alloc_var;

    emitVarAssign(alloc_var, unit->expr());
}

void IRBuilderPass::buildIRVarAssign(const VarAssignUnit* unit) {
    const std::string name = unit->var()->name();
    const Variable* variable = unit->var()->variable();

    llvm::AllocaInst* var = named_values_[variable];
    if (var == nullptr) {
        std::cerr << "IRBuilder: Var " << name << " is'n exist\n";
        return;
    }

    emitVarAssign(var, unit->expr());
}

void IRBuilderPass::emitVarAssign(llvm::AllocaInst* var, const ExpressionUnit* unit) {
    llvm::Value* var_value = buildIRExpression(unit);
    builder_.CreateStore(var_value, var);
}

llvm::Value* IRBuilderPass::createLLVMInt(int value) {
    return llvm::ConstantInt::get(context_, llvm::APInt(32, value, true));
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
            return builder_.CreateAdd(left_value, right_value, "addtmp");
        case GrammarUnitType::SUB:
            return builder_.CreateSub(left_value, right_value, "subtmp");
        case GrammarUnitType::MUL:
            return builder_.CreateMul(left_value, right_value, "multmp");
        case GrammarUnitType::DIV:
            return builder_.CreateSDiv(left_value, right_value, "divtmp");
        default:
            break;
    }

    std::cerr << "IRBuilderPass: unknown binary operator type\n";
    return nullptr;
}

llvm::Value* IRBuilderPass::buildIRUnaryOperator(const UnaryOperUnit* unit) {
    llvm::Value* zero = createLLVMInt(0);
    llvm::Value* operand = buildIRExpression(unit->operand());

    switch (unit->getType())
    {
        case GrammarUnitType::UNARY_MINUS:
            return builder_.CreateSub(zero, operand);
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
    return createLLVMInt(unit->num());
}

llvm::Value* IRBuilderPass::buildIRVar(const VarUnit* unit) {
    llvm::AllocaInst* var = named_values_[unit->variable()];
    if (var == nullptr) {
        std::cerr << "buildIRObject: Unknown variable\n";
        return nullptr;
    }

    return builder_.CreateLoad(var->getAllocatedType(), var, unit->name());
}
