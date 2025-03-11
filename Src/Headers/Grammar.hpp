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

// class Statement : public GrammarUnit {
//   public:

//   private:
//     VarUnit* variable_;
// };

class ExpressionUnit : public GrammarUnit {
  public:
    ExpressionUnit(GrammarUnitType type) :
      GrammarUnit(type) { }
};

class ObjectUnit : public ExpressionUnit {
  public:
    ObjectUnit(GrammarUnitType type) :
      ExpressionUnit(type) { }
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

class UnaryOperUnit : public ExpressionUnit {
  public:
    UnaryOperUnit(ExpressionUnit* operand, GrammarUnitType type) :
      ExpressionUnit(type),
      operand_(operand) { }

  ExpressionUnit* operand() const {
    return operand_;
  }

  protected:
    ExpressionUnit* operand_;
};

class UnaryOperMinus : public UnaryOperUnit {
  public:
    UnaryOperMinus(ExpressionUnit* operand) :
      UnaryOperUnit(operand, GrammarUnitType::UNARY_MINUS),
      operand_(operand) { }

    int executeUnit() {
        return -operand_->executeUnit();
    }

  private:
    GrammarUnit* operand_;
};

class BinaryOperUnit : public ExpressionUnit {
  public:
    BinaryOperUnit(ExpressionUnit* left_op, ExpressionUnit* right_op, GrammarUnitType type) :
      ExpressionUnit(type),
      left_op_(left_op),
      right_op_(right_op) { }

    const ExpressionUnit* left() const {
        return left_op_;
    }

    const ExpressionUnit* right() const {
        return right_op_;
    }

  public:
    ExpressionUnit* left_op_;
    ExpressionUnit* right_op_;
};

class AddExprUnit : public BinaryOperUnit {
  public:
    AddExprUnit(ExpressionUnit* left_op, ExpressionUnit* right_op) :
      BinaryOperUnit(left_op, right_op, GrammarUnitType::ADD) { }

    int executeUnit() {
        return left_op_->executeUnit() + right_op_->executeUnit();
    }
};

class MulExprUnit : public BinaryOperUnit {
  public:
    MulExprUnit(ExpressionUnit* left_op, ExpressionUnit* right_op) :
      BinaryOperUnit(left_op, right_op, GrammarUnitType::MUL) { }

    int executeUnit() {
        return left_op_->executeUnit() * right_op_->executeUnit();
    }
};

class SubExprUnit : public BinaryOperUnit {
  public:
    SubExprUnit(ExpressionUnit* left_op, ExpressionUnit* right_op) :
      BinaryOperUnit(left_op, right_op, GrammarUnitType::SUB) { }

    int executeUnit() {
        return left_op_->executeUnit() - right_op_->executeUnit();
    }
};

class DivExprUnit : public BinaryOperUnit {
  public:
    DivExprUnit(ExpressionUnit* left_op, ExpressionUnit* right_op) :
      BinaryOperUnit(left_op, right_op, GrammarUnitType::DIV) { }

    int executeUnit() {
        // TODO add check right_op result to zero
        return left_op_->executeUnit() / right_op_->executeUnit();
    }
};
