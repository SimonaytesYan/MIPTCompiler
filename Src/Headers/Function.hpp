#pragma once

#include "Grammar.hpp"
#include "Types.hpp"

class Function {

  public:
    Function(std::string name, ExpressionType* return_type) :
      name_(name),
      return_type_(return_type->copy()) { }
    
    Function() :
    name_("no named func"),
    return_type_(nullptr) {}

    void setName(const std::string& name) {
        name_ = name;
    }

    void setRetType(const ExpressionType* ret_type) {
        return_type_ = ret_type->copy();
    }

    void addArg(const Variable& new_arg) {
        args_.push_back(new_arg);
    }

    const ExpressionType* return_type() const {
        return return_type_;
    }

    const std::string& name() const {
        return name_;
    }

    ~Function() {
        delete return_type_;
    }

  private:
    std::string name_;
    ExpressionType* return_type_;
    std::vector<Variable> args_;
};