#pragma once

#include "Tokens.hpp"

#include <vector>

enum class GrammarUnitType {
    VAR,
    NUM,
    ADD,
    SUB,
    MUL,
    DIV
};

class GrammarUnit {
  public:
    virtual int executeUnit() = 0;
    virtual GrammarUnitType getType() = 0;
    virtual ~GrammarUnit() = 0;
};

class NumUnit : public GrammarUnit {
  public:

    NumUnit(int value) :
      value_(value) { }

    int executeUnit() {
        return value_;
    }

    GrammarUnitType getType() {
        return GrammarUnitType::NUM;
    }

  private:
    int value_;
};

class VarUnit : public GrammarUnit {
  public:
    int executeUnit() {
        return ;
    }

    GrammarUnitType getType() {
        return GrammarUnitType::VAR;
    }

  private:
    std::string str;
};

class ExprUnit : public GrammarUnit {
  public:
    ExprUnit(GrammarUnit* left_op, GrammarUnit* right_op) :
      left_op_(left_op),
      right_op_(right_op) { }

  public:
    GrammarUnit* left_op_;
    GrammarUnit* right_op_;
};

class AddExprUnit : public ExprUnit {
  public:
    AddExprUnit(GrammarUnit* left_op, GrammarUnit* right_op) :
      ExprUnit(left_op, right_op) { }

    int executeUnit() {
        return left_op_->executeUnit() + right_op_->executeUnit();
    }

    GrammarUnitType getType() {
        return GrammarUnitType::ADD;
    }
};

class MulExprUnit : public ExprUnit {
  public:
    MulExprUnit(GrammarUnit* left_op, GrammarUnit* right_op) :
      ExprUnit(left_op, right_op) { }

    int executeUnit() {
        return left_op_->executeUnit() * right_op_->executeUnit();
    }

    GrammarUnitType getType() {
        return GrammarUnitType::MUL;
    }
};

class SubExprUnit : public ExprUnit {
  public:
    SubExprUnit(GrammarUnit* left_op, GrammarUnit* right_op) :
      ExprUnit(left_op, right_op) { }

    int executeUnit() {
        return left_op_->executeUnit() - right_op_->executeUnit();
    }

    GrammarUnitType getType() {
        return GrammarUnitType::SUB;
    }
};

class DivExprUnit : public ExprUnit {
  public:
    DivExprUnit(GrammarUnit* left_op, GrammarUnit* right_op) :
      ExprUnit(left_op, right_op) { }

    int executeUnit() {
        // TODO add check right_op result to zero
        return left_op_->executeUnit() / right_op_->executeUnit();
    }

    GrammarUnitType getType() {
        return GrammarUnitType::DIV;
    }
};
