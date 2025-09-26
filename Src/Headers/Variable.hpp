#pragma once

#include "Grammar.hpp"
#include "Types.hpp"

class Variable {

  public:
    Variable(std::string name, ExpressionType* type) :
      name_(name),
      type_(type->copy()) { }
    
    Variable() :
    name_("no named"),
    type_(nullptr) {}

    void setName(const std::string& name) {
        name_ = name;
    }

    void setType(ExpressionType* type) {
        type_ = type->copy();
    }

    const ExpressionType* type() const {
        return type_;
    }

    const std::string& name() const {
        return name_;
    }

    ~Variable() {
        delete type_;
    }

  private:
    std::string name_;
    ExpressionType* type_;
};
