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
    SCOPE,
    VAR_DECL,
    IF,
    LOOP,
};

bool isGrammarUnitOperator(GrammarUnitType type) {
  return type == GrammarUnitType::ADD ||
         type == GrammarUnitType::SUB ||
         type == GrammarUnitType::MUL ||
         type == GrammarUnitType::DIV;
}

class GrammarUnit;

class ScopeUnit;
class StatementUnit;
class ExpressionUnit;
class IfUnit;
class LoopUnit;

class ObjectUnit;
class NumUnit;
class VarUnit;

class UnaryOperUnit;
class UnaryOperMinus;

class BinaryOperUnit;
class AddExprUnit;
class MulExprUnit;
class DivExprUnit;
class SubExprUnit;
class VarDeclUnit;

class GrammarUnit {
  public:

    GrammarUnit(GrammarUnitType type) :
      type_(type) { }

    virtual int executeUnit() = 0;

    virtual GrammarUnitType getType() const {
        return type_;
    }

    virtual ~GrammarUnit() = default;

  private:
    GrammarUnitType type_;
};

class ScopeUnit : public GrammarUnit {
  public:
    ScopeUnit() :
      GrammarUnit(GrammarUnitType::SCOPE) { }

    int executeUnit() {
      // for (const auto& statement : statements) {
      //   statement->executeUnit();
      // }

      return 0;
    }

    std::vector<StatementUnit*>::iterator begin() {
      return statements.begin();
    }

    std::vector<StatementUnit*>::iterator end() {
      return statements.end();
    }

    std::vector<StatementUnit*>::const_iterator begin() const {
      return statements.cbegin();
    }

    std::vector<StatementUnit*>::const_iterator end() const {
      return statements.cend();
    }

    void addStatements(StatementUnit* statement) {
      statements.push_back(statement);
    }

    ~ScopeUnit() = default;

  private:
    std::vector<StatementUnit*> statements;
};

class StatementUnit : public GrammarUnit {
  public:
    StatementUnit(GrammarUnitType type) :
      GrammarUnit(type) { }

    virtual int executeUnit() = 0;

    virtual ~StatementUnit() = default;
};

class IfUnit : public StatementUnit {
  public:
    IfUnit(ExpressionUnit* condition,
           ScopeUnit* true_branch,
           ScopeUnit* false_branch) :
      StatementUnit(GrammarUnitType::IF),
      condition_(condition),
      true_branch_(true_branch),
      false_branch_(false_branch) { }

    ExpressionUnit* condition() {
      return condition_;
    }
    ScopeUnit* true_branch() {
      return true_branch_;
    }
    ScopeUnit* false_branch() {
      return false_branch_;
    }

    const ExpressionUnit* condition() const {
      return condition_;
    }
    const ScopeUnit* true_branch() const {
      return true_branch_;
    }
    const ScopeUnit* false_branch() const {
      return false_branch_;
    }

    virtual int executeUnit() {
      return 0;
    }

  private:
    ExpressionUnit* condition_;
    ScopeUnit* true_branch_;
    ScopeUnit* false_branch_;
};

class VarDeclUnit : public StatementUnit {
  public:
    VarDeclUnit(VarUnit* variable, ExpressionUnit* expression) :
      StatementUnit(GrammarUnitType::VAR_DECL),
      variable_(variable),
      expression_(expression) { }

    int executeUnit() {
      // variable = executeUnit(expression);
      return 0;
    }

    VarUnit* var() {
      return variable_;
    }

    ExpressionUnit* expr() {
      return expression_;
    }

    const VarUnit* var() const {
      return variable_;
    }

    const ExpressionUnit* expr() const {
      return expression_;
    }

  private:
    VarUnit* variable_;
    ExpressionUnit* expression_;
};

class LoopUnit : public StatementUnit {
  public:
    LoopUnit(ExpressionUnit* condition, ScopeUnit* body) :
      StatementUnit(GrammarUnitType::LOOP),
      condition_(condition),
      body_(body) {}

    virtual int executeUnit() {
      return 0;
    }

  public:
    ExpressionUnit* condition_;
    ScopeUnit* body_;
};

class ExpressionUnit : public GrammarUnit {
  public:
    ExpressionUnit(GrammarUnitType type) :
      GrammarUnit(type) { }

    virtual ~ExpressionUnit() { }
};

class ObjectUnit : public ExpressionUnit {
  public:
    ObjectUnit(GrammarUnitType type) :
      ExpressionUnit(type) { }

    virtual ~ObjectUnit() = default;
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

  virtual ~UnaryOperUnit() = default;

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
    virtual ~BinaryOperUnit() = default;

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
