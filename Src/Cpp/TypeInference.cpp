#include "Logger.hpp"
#include "TypeInference.hpp"
#include "Utils.hpp"

#include "assert.h"
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

bool TypeSystem::isSuccessful() {
    return finished_successfully_;
}

GrammarUnit* TypeSystem::inferTypes(GrammarUnit* root) {
    if (root == nullptr) {
        finished_successfully_ = false;
        std::cerr << "Root is nullptr\n";
        return nullptr;
    }

    if (root->getType() != GrammarUnitType::START) {
        std::cerr << "Root is not START unit\n";
        finished_successfully_ = false;
        return nullptr;
    }

    LOG << "Start type inference\n";
    inferInStart(static_cast<StartUnit*>(root));

    if (isSuccessful())
        return root;
    return nullptr;
}

void TypeSystem::inferInScope(ScopeUnit* unit) {
    LOG << __func__ << "\n";

    var_table_.startScope();
    for (StatementUnit* statement : *unit) {
        inferInStatement(statement);
    }
    var_table_.endScope();
}

void TypeSystem::inferInStatement(StatementUnit* unit) {
    LOG << __func__ << "\n";

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
    case GrammarUnitType::RET:
        inferInRet(static_cast<RetUnit*>(unit));
        break;

    default:
        assert("Unknown statement unit");
        break;
    }
}

void TypeSystem::inferInIf(IfUnit* unit) {
    LOG << __func__ << "\n";

    inferInExpresion(unit->condition());
    inferInScope(unit->true_branch());
    inferInScope(unit->false_branch());
}

void TypeSystem::inferInRet(RetUnit* unit) {
    LOG << __func__ << "\n";

    inferInExpresion(unit->expr());

    const auto cur_expr_type = unit->expr()->exprType();
    if (function_in_process_.has_value()) {
        const ExpressionType* ret_type = function_in_process_.value()->type()->getRetType();
        if (ret_type == nullptr) {
            function_in_process_.value()->setRetType(cur_expr_type);
        }
        else  {
            TYPE_INF_ASSERT((ExpressionType::isEqual(cur_expr_type, ret_type)), 
                            "Return type mismatch")
        }
    }
}

void TypeSystem::inferInPrint(PrintUnit* unit) {
    LOG << __func__ << "\n";

    inferInExpresion(unit->expression());
}

void TypeSystem::inferInLoop(LoopUnit* unit) {
    LOG << __func__ << "\n";

    inferInExpresion(unit->condition());
    inferInScope(unit->body());
}


void TypeSystem::inferInFunDecl(FunDeclUnit* unit) {
    LOG << __func__ << "\n";

    fun_table_.insertFun(&unit->func(), unit->body());
}

void TypeSystem::inferInVarDecl(VarDeclUnit* unit) {
    LOG << __func__ << "\n";

    inferInExpresion(unit->expr());
    TYPE_INF_ASSERT(isSuccessful(), "");

    unit->updateNameTypeVariable();
    var_table_.insertVar(unit->variable());

    inferInVar(unit->var());
    TYPE_INF_ASSERT(isSuccessful(), "");
}

void TypeSystem::inferInVarAssign(VarAssignUnit* unit) {
    LOG << __func__ << "\n";

    inferInExpresion(unit->expr());
    inferInExpresion(unit->var());
    
    TYPE_INF_ASSERT((ExpressionType::isEqual(unit->var()->exprType(), unit->expr()->exprType())), 
                    "Variable type is not equal with assignment expression")
}

void TypeSystem::inferInExpresion(ExpressionUnit* unit) {
    LOG << __func__ << "\n";

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
    LOG << __func__ << "\n";

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
    LOG << __func__ << "\n";

    inferInExpresion(unit->operand());
    TYPE_INF_ASSERT(isFloat(unit->operand()) || isInteger(unit->operand()),
                    "Not basic type in unary operation expr")
    unit->setExprType(unit->operand()->exprType()->copy());
}

void TypeSystem::inferInObject(ObjectUnit* unit) {
    TYPE_INF_ASSERT(unit != nullptr, "object is nullptr");
    LOG << __func__ << "\n";

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
    case GrammarUnitType::STRING:
        inferInString(static_cast<StringUnit*>(unit));
        break;
    case GrammarUnitType::FUN_CALL:
        inferInFunCall(static_cast<FunCallUnit*>(unit));
        break;
    
    default:
        break;
    }
}

void TypeSystem::inferInVar(VarUnit* unit) {
    Variable* variable = var_table_.findVar(unit->name());
    TYPE_INF_ASSERT(variable != nullptr,
                    "Variable " << variable->name() << " wasn't declare")
                    
    unit->setExprType(variable->type()->copy());
    unit->setVariable(variable);
}

void TypeSystem::inferInNum(NumUnit* unit) {
    unit->setExprType(new IntegerExprType());
}

void TypeSystem::inferInFloat(FloatUnit* unit) {
    unit->setExprType(new FloatExprType());
}

void TypeSystem::inferInString(StringUnit* unit) {
    unit->setExprType(new StringExprType());
}

void TypeSystem::inferInArray(ArrayUnit* unit) {
    ExpressionType* expr_type = nullptr;
    for (auto elem : unit->arrayElements()) {
        inferInExpresion(elem);
        if (expr_type == nullptr) {
            expr_type = elem->exprType();
        }
        else {
            TYPE_INF_ASSERT(ExpressionType::isEqual(expr_type, elem->exprType()),
                            "Elements in array has different types")
        }
    }

    unit->setExprType(new ArrayVarType(unit->arrayElements().size(), expr_type));
}

void TypeSystem::inferInFunCall(FunCallUnit* unit) {
    LOG << __func__ << "\n";

    Function* fun = fun_table_.findFun(unit->name());
    TYPE_INF_ASSERT(fun != nullptr,
                    "Function " << fun->name() << " wasn't declare")
    
    if (!fun_table_.isFunInstantiated(unit->name())) {
        instantiateFunction(fun, unit);
    }
    unit->setFunction(fun);
    unit->setExprType(fun->type()->getRetType()->copy());

    const auto& correct_args_type = fun->type()->getArgTypes();

    bool args_type_correct = true;
    for (size_t arg_ind = 0; arg_ind < unit->args().size(); ++arg_ind) {
        auto arg = unit->args()[arg_ind];
        inferInExpresion(arg);

        TYPE_INF_ASSERT(ExpressionType::isEqual(arg->exprType(), correct_args_type[arg_ind]), 
                        std::string(std::string("argument of function '") + fun->name() + std::string("' type is incorrect")));
    }
}

void TypeSystem::instantiateFunction(Function* fun, FunCallUnit* unit) {
    // Infer args type
    std::vector<ExpressionType*> args_type;
    for (size_t arg_ind = 0; arg_ind < unit->args().size(); ++arg_ind) {
        auto arg = unit->args()[arg_ind];
        inferInExpresion(arg);
        args_type.push_back(arg->exprType()->copy());
    }

    fun->setArgsType(std::move(args_type));

    // Infer type in function body 
    std::vector<Variable> args;
    auto args_name = fun->args_name();
    auto args_type = fun->type()->getArgTypes();

    assert(args_name.size() == args_type.size());

    var_table_.startFunction();
    for (size_t ind = 0; ind < args_name.size(); ind++) {
        args.emplace_back(args_name[ind], args_type[ind]);
        var_table_.insertVar(&args[ind]);
    }
    function_in_process_ = fun;

    inferInScope(fun_table_.findFunBody(fun));

    function_in_process_.reset();
    var_table_.endFunction();

    fun_table_.funWasInstantiated(fun->name());
}

Variable* TypeSystem::VarTable::findVar(const std::string& name) {
    // Go from top of the 'stack' to the end
    auto begin_it = named_expr_types_.rbegin();
    auto end_it = named_expr_types_.rend();

    for (auto scope_it = begin_it; scope_it != end_it; ++scope_it) {
        if (begin_it->second != scope_it->second) // Do not go out from scopes of function
            break;

        Variable* var_in_cur_scope = (*scope_it).first[name];
        if (var_in_cur_scope != nullptr) {
            return var_in_cur_scope;
        }
    }

    return nullptr;
}

void TypeSystem::VarTable::startScope() {
    if (named_expr_types_.size() == 0 || named_expr_types_.back().second == MAIN_SCOPE)
        named_expr_types_.push_back(std::make_pair(ScopeType(), MAIN_SCOPE));
    else
        named_expr_types_.push_back(std::make_pair(ScopeType(), USER_FUNCTION_SCOPE));
}

void TypeSystem::VarTable::endScope() {
    named_expr_types_.pop_back();
}

void TypeSystem::VarTable::startFunction() {
    named_expr_types_.push_back(std::make_pair(ScopeType(), USER_FUNCTION_SCOPE));
}

void TypeSystem::VarTable::endFunction() {
    endScope();
}

void TypeSystem::VarTable::insertVar(Variable* var) {
    named_expr_types_.back().first[var->name()] = var;
}

Function* TypeSystem::FunTable::findFun(const std::string& name) {
    return fun_types_[name];
}

void TypeSystem::FunTable::insertFun(Function* func, ScopeUnit* body) {
    fun_types_[func->name()] = func;
    is_fun_instantiated[func] = false;
    fun_body_[func] = body;
}

ScopeUnit* TypeSystem::FunTable::findFunBody(Function* fun) {
    return fun_body_[fun];
}

bool TypeSystem::FunTable::isFunInstantiated(const std::string& name) {
    return is_fun_instantiated[findFun(name)];
}

void TypeSystem::FunTable::funWasInstantiated(const std::string& name) {
    is_fun_instantiated[findFun(name)] = true;
}
