#pragma once

#include "Grammar.hpp"
#include "Types.hpp"

#include "llvm/IR/Instructions.h"

class Variable {

  public:
    Variable(std::string name, ExpressionType* type) :
      name_(name)
      type_(type->copy()),
      alloca_inst_(nullptr) { }

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
        return name;
    }

    ExpressionType

    ~Variable() {
        delete type_;
    }

  private:
    std::string name_;              // set by TypeInference
    ExpressionType* type_;          // set by TypeInference
    llvm::AllocaInst* alloca_inst_; // set and use by IRBuilderPass
};
