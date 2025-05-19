#pragma once

#include "Grammar.hpp"

#include <vector>
#include <map>

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

  public:
    GrammarUnit* inferTypes(GrammarUnit* root);

  private:
    void inferInScope(ScopeUnit* unit);
    void inferInStatement(StatementUnit* unit);
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

    Variable* findVar(const std::string& name);
    void startScope();
    void endScope();

  private:
  // TODO 
  // TODO Добавить функции для работы с named_expr_types в TypeSystem и использовать их
  // TODO 
    VarTable var_table_;
};
