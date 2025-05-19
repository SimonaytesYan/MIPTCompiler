#pragma once

#include "Grammar.hpp"

class TypeSystem {
  public:
    GrammarUnit* inferTypes(GrammarUnit* root);

  private:
    void inferInScope(ScopeUnit* unit);
    void inferInStatement(StatementUnit* unit);
    void inferInIf(StatementUnit* unit);
    void inferInPrint(StatementUnit* unit);
    void inferInLoop(StatementUnit* unit);
    void inferInVarDecl(StatementUnit* unit);
    void inferInVarAssign(StatementUnit* unit);
    void inferInExpresion(ExpressionUnit* unit);
    void inferInAddSub(ExpressionUnit* unit);
    void inferInMulDiv(ExpressionUnit* unit);
    void inferInBrackets(ExpressionUnit* unit);
    void inferInUnaryMinus(ExpressionUnit* unit);
    void inferInObject(ExpressionUnit* unit);
    void inferInVar(VarUnit* unit);
    void inferInNum(NumUnit* unit);
    void inferInFloat(FloatUnit* unit);
    void inferInArray(ArrayUnit* unit);
};
