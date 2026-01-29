#pragma once

#include "Grammar.hpp"
#include "Types.hpp"

// TODO emplace ptr to unique_ptr 

class Function {
  public:
    Function() :
      type_(new FunctionType()) { }

    void setName(const std::string& name) { // Set in lexer analyzer
        name_ = name;
    }

    void addArgName(const std::string& new_arg) { // Set in lexer analyzer
        args_name_.push_back(new_arg);
    }

    void setRetType(const ExpressionType* ret_type) {   // Set in type inference when first function call was meet
        type_->setRetType(ret_type);
    }

    void setArgsType(std::vector<ExpressionType*>&& args_type) {    // Set in type inference when first function call was meet
        type_->setArgsType(std::move(args_type));
    }

    const FunctionType* type() const {
        return type_;
    }

    const std::string& name() const {
        return name_;
    }

    const std::vector<std::string>& args_name() const {
        return args_name_;
    }

    ~Function() {
        delete type_;
    }

  private:
    std::string name_; 
    FunctionType* type_;
    std::vector<std::string> args_name_;
};