#pragma once

#include "Grammar.hpp"

#include <map>
#include <optional>
#include <vector>

class TypeSystem {
    class VarTable {
      public:
        Variable* findVar(const std::string& name);
        void startScope();
        void endScope();
        void insertVar(Variable* var);

      private:
        std::vector<std::map<std::string, Variable*>> named_expr_types_;
    };

    class FunTable {
      public:
        Function* findFun(const std::string& name);
        void insertFun(Function* fun);

        bool isFunInstantiated(const std::string& name);
        void funWasInstantiated(const std::string& name);

      private:
        std::map<std::string, Function*> fun_types_;
        std::map<Function*, bool> is_fun_instantiated;
    };

  public:
    // TODO start from startUnit
    GrammarUnit* inferTypes(GrammarUnit* root);
    bool isSuccessful();

  private:
    void inferInScope(ScopeUnit* unit);
    void inferInStatement(StatementUnit* unit);
    void inferInRet(RetUnit* unit);
    void inferInFunDecl(FunDeclUnit* unit);
    void inferInFunCall(FunCallUnit* unit);
    void inferInIf(IfUnit* unit);
    void inferInPrint(PrintUnit* unit);
    void inferInLoop(LoopUnit* unit);
    void inferInVarDecl(VarDeclUnit* unit);
    void inferInVarAssign(VarAssignUnit* unit);

    void inferInExpresion(ExpressionUnit* unit);
    void inferInBinaryOp(BinaryOperUnit* unit);
    void inferInUnaryOp(UnaryOperUnit* unit);

    void inferInObject(ObjectUnit* unit);
    void inferInVar(VarUnit* unit);
    void inferInNum(NumUnit* unit);
    void inferInFloat(FloatUnit* unit);
    void inferInArray(ArrayUnit* unit);
    void inferInString(StringUnit* unit);

    void instantiateFunction(Function* fun, FunCallUnit* unit);

  private:
    VarTable var_table_;
    FunTable fun_table_;
    bool finished_successfully_ = true;

    std::optional<Function*> function_in_process_;
};
