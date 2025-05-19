#pragma once

#include "Tokens.hpp"
#include "Types.hpp"

#include <string>
#include <vector>

class GrammarUnit;

  class ScopeUnit;
  class StatementUnit;
    class IfUnit;
    class LoopUnit;
    class PrintUnit;
    class VarDeclUnit;
    class VarAssignUnit;

  class ExpressionUnit;
    class ObjectUnit;
      class NumUnit;
      class VarUnit;
      class ArrayUnit;

    class OperatorUnit;
      class UnaryOperUnit;
        class UnaryOperMinus;

      class BinaryOperUnit;
        class AddExprUnit;
        class MulExprUnit;
        class DivExprUnit;
        class SubExprUnit;

bool isGrammarUnitOperator(const GrammarUnit* unit);
bool isGrammarUnitBinaryOperator(const GrammarUnit* unit);
bool isGrammarUnitUnaryOperator(const GrammarUnit* unit);
bool isGrammarUnitObject(const GrammarUnit* unit);
bool isGrammarUnitExpression(const GrammarUnit* unit);
bool isGrammarUnitStatement(const GrammarUnit* unit);

enum class GrammarUnitType {
    VAR,
    NUM,
    FLOAT,
    ARRAY,
    NAME,
    ADD,
    SUB,
    MUL,
    DIV,
    UNARY_MINUS,
    SCOPE,
    VAR_DECL,
    VAR_ASSIGN,
    IF,
    LOOP,
    PRINT,
};

class GrammarUnit {
  public:

    GrammarUnit(GrammarUnitType type) :
      type_(type) { }

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

    virtual ~StatementUnit() = default;
};

class PrintUnit : public StatementUnit {
  public:
    PrintUnit(ExpressionUnit* expression) :
      StatementUnit(GrammarUnitType::PRINT),
      expression_(expression) { }

    ExpressionUnit* expression() {
      return expression_;
    }

    const ExpressionUnit* expression() const {
      return expression_;
    }

  private:
    ExpressionUnit* expression_;
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

class VarAssignUnit : public StatementUnit {
  public:
    VarAssignUnit(VarUnit* variable, ExpressionUnit* expression) :
      StatementUnit(GrammarUnitType::VAR_ASSIGN),
      variable_(variable),
      expression_(expression) { }

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

    ExpressionUnit* condition() {
      return condition_;
    }

    ScopeUnit* body() {
      return body_;
    }

    const ExpressionUnit* condition() const {
      return condition_;
    }

    const ScopeUnit* body() const {
      return body_;
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

    int num() const {
        return value_;
    }

  private:
    int value_;
};

class FloatUnit : public ObjectUnit {
  public:

    FloatUnit(float value) :
      ObjectUnit(GrammarUnitType::FLOAT),
      value_(value) { }

    float num() const {
        return value_;
    }

  private:
    float value_;
};

class VarUnit : public ObjectUnit {
  public:
    VarUnit(const std::string& str, VarType* var_type = nullptr) :
      ObjectUnit(GrammarUnitType::VAR),
      name_(str),
      var_type_(var_type) { }

    const std::string& name() const {
        return name_;
    }

    VarType* var_type() const {
        return var_type_;
    }

  private:
    std::string name_;
    VarType* var_type_;
};

class ArrayUnit : public ObjectUnit {
  public:
    ArrayUnit(std::vector<ExpressionUnit*>&& array_elements) :
      ObjectUnit(GrammarUnitType::ARRAY),
      array_elements_(array_elements) { }

    const std::vector<ExpressionUnit*>& array_elements() const {
      return array_elements_;
    }

  private:
    std::vector<ExpressionUnit*> array_elements_;
};

class OperatorUnit : public ExpressionUnit {
  public:
    OperatorUnit(GrammarUnitType type) :
    ExpressionUnit(type) { }
};

class UnaryOperUnit : public OperatorUnit {
  public:
    UnaryOperUnit(ExpressionUnit* operand, GrammarUnitType type) :
      OperatorUnit(type),
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

  private:
    GrammarUnit* operand_;
};

class BinaryOperUnit : public OperatorUnit {
  public:
    virtual ~BinaryOperUnit() = default;

    BinaryOperUnit(ExpressionUnit* left_op, ExpressionUnit* right_op, GrammarUnitType type) :
      OperatorUnit(type),
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
};

class MulExprUnit : public BinaryOperUnit {
  public:
    MulExprUnit(ExpressionUnit* left_op, ExpressionUnit* right_op) :
      BinaryOperUnit(left_op, right_op, GrammarUnitType::MUL) { }
};

class SubExprUnit : public BinaryOperUnit {
  public:
    SubExprUnit(ExpressionUnit* left_op, ExpressionUnit* right_op) :
      BinaryOperUnit(left_op, right_op, GrammarUnitType::SUB) { }
};

class DivExprUnit : public BinaryOperUnit {
  public:
    DivExprUnit(ExpressionUnit* left_op, ExpressionUnit* right_op) :
      BinaryOperUnit(left_op, right_op, GrammarUnitType::DIV) { }
};
