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
        void startFunction();
        void endFunction();
        void insertVar(Variable* var);

      private:
        using ScopeType = std::map<std::string, Variable*>;

        enum ScopeInFunction {
          USER_FUNCTION_SCOPE,
          MAIN_SCOPE
        };

        std::vector<std::pair<ScopeType, ScopeInFunction>> named_expr_types_;
    };

    class FunTable {
      public:
        Function* findFun(const std::string& name);
        void insertFun(Function* fun, ScopeUnit* body);

        ScopeUnit* findFunBody(Function* fun);
        bool isFunInstantiated(const std::string& name);
        void funWasInstantiated(const std::string& name);

      private:
        std::map<std::string, Function*> fun_types_;
        std::map<Function*, bool> is_fun_instantiated;
        std::map<Function*, ScopeUnit*> fun_body_;
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
