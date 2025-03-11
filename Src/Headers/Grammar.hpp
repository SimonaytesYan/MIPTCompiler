#pragma once

#include "Tokens.hpp"

#include <vector>

enum class GrammarUnitType {
    VAR,
    NUM,
    ADD,
    SUB,
    MUL,
    DIV,
    UNARY_MINUS,
    KEYWORD,
    FICTITIOUS,
};

class GrammarUnit {
  public:

    GrammarUnit(GrammarUnitType type) :
      type_(type) { }

    virtual int executeUnit() = 0;

    virtual GrammarUnitType getType() const {
        return type_;
    }

  private:
    GrammarUnitType type_;
};

class ObjectUnit : public GrammarUnit {
  public:
    ObjectUnit(GrammarUnitType type) :
      GrammarUnit(type) { }
};

class NumUnit : public ObjectUnit {
  public:

    NumUnit(int value) :
      ObjectUnit(GrammarUnitType::NUM),
      value_(value) { }

    int executeUnit() {
        return value_;
    }

    int num() const {
        return value_;
    }

  private:
    int value_;
};

class VarUnit : public ObjectUnit {
  public:
    VarUnit(const std::string& str) :
      ObjectUnit(GrammarUnitType::VAR),
      name_(str) {
    }

    int executeUnit() {
        return 0;
    }

    const std::string& name() const {
        return name_;
    }

  private:
    std::string name_;
};

class UnaryOperUnit : public GrammarUnit {
  public:
    UnaryOperUnit(GrammarUnit* operand, GrammarUnitType type) :
      GrammarUnit(type),
      operand_(operand) { }

  GrammarUnit* operand() const {
    return operand_;
  }

  protected:
    GrammarUnit* operand_;
};

class UnaryOperMinus : public UnaryOperUnit {
  public:
    UnaryOperMinus(GrammarUnit* operand) :
      UnaryOperUnit(operand, GrammarUnitType::UNARY_MINUS),
      operand_(operand) { }

    int executeUnit() {
        return -operand_->executeUnit();
    }

  private:
    GrammarUnit* operand_;
};

class ExprUnit : public GrammarUnit {
  public:
    ExprUnit(GrammarUnit* left_op, GrammarUnit* right_op, GrammarUnitType type) :
      GrammarUnit(type),
      left_op_(left_op),
      right_op_(right_op) { }

    const GrammarUnit* left() const {
        return left_op_;
    }

    const GrammarUnit* right() const {
        return right_op_;
    }

  public:
    GrammarUnit* left_op_;
    GrammarUnit* right_op_;
};

class AddExprUnit : public ExprUnit {
  public:
    AddExprUnit(GrammarUnit* left_op, GrammarUnit* right_op) :
      ExprUnit(left_op, right_op, GrammarUnitType::ADD) { }

    int executeUnit() {
        return left_op_->executeUnit() + right_op_->executeUnit();
    }
};

class MulExprUnit : public ExprUnit {
  public:
    MulExprUnit(GrammarUnit* left_op, GrammarUnit* right_op) :
      ExprUnit(left_op, right_op, GrammarUnitType::MUL) { }

    int executeUnit() {
        return left_op_->executeUnit() * right_op_->executeUnit();
    }
};

class SubExprUnit : public ExprUnit {
  public:
    SubExprUnit(GrammarUnit* left_op, GrammarUnit* right_op) :
      ExprUnit(left_op, right_op, GrammarUnitType::SUB) { }

    int executeUnit() {
        return left_op_->executeUnit() - right_op_->executeUnit();
    }
};

class DivExprUnit : public ExprUnit {
  public:
    DivExprUnit(GrammarUnit* left_op, GrammarUnit* right_op) :
      ExprUnit(left_op, right_op, GrammarUnitType::DIV) { }

    int executeUnit() {
        // TODO add check right_op result to zero
        return left_op_->executeUnit() / right_op_->executeUnit();
    }
};
