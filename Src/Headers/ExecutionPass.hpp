#pragma once

#include "Grammar.hpp"

#include <iostream>
#include <string>
#include <variant>
#include <vector>

struct IntVar {
  int value_;
  std::string name_;
};

struct ScopeStarter {
};

using Variable = std::variant<IntVar, ScopeStarter>;

class VariableList {
  public:

    void Dump() {
        std::cout << "\nVariable list dump\n";
        for (const Variable& var : vars_) {
            if (std::holds_alternative<ScopeStarter>(var)) {
               std::cout << "{ ";
            }
            else {
                IntVar cur_var = std::get<IntVar>(var);
                std::cout << "'" << cur_var.name_ << "', ";
            }
        }
        std::cout << "\n";
    }

    void addNewVar(Variable var) {
      vars_.push_back(var);
    }

    void setVarValue(std::string name, int value) {
        const int var_num = vars_.size();

        for (int var_ind = var_num - 1; var_ind >= 0; var_ind--) {
            if (std::holds_alternative<ScopeStarter>(vars_[var_ind])) {
              continue;
            }
            IntVar& cur_var = std::get<IntVar>(vars_[var_ind]);
            if (name == cur_var.name_) {
                cur_var.value_ = value;
                return;
            }
        }

        std::cerr << "Error in VariableList set: '" << name << "' unknown variable name\n";
        Dump();
    }

    int getVarValue(std::string name) {
        const int var_num = vars_.size();

        for (int var_ind = var_num - 1; var_ind >= 0; var_ind--) {
            if (std::holds_alternative<ScopeStarter>(vars_[var_ind])) {
              continue;
            }

            const IntVar& cur_var = std::get<IntVar>(vars_[var_ind]);
            if (name == cur_var.name_) {
                return cur_var.value_;
            }
        }

        std::cerr << "Error in VariableList get: '" << name << "' unknown variable name\n";
        Dump();
        return 0;
    }

    void startScope() {
      vars_.push_back(ScopeStarter());
    }

    void endScope() {
      const int var_num = vars_.size();

      for (int var_ind = var_num - 1; var_ind >= 0; var_ind--) {
        if (std::holds_alternative<ScopeStarter>(vars_[var_ind])) {
          vars_.pop_back();
          break;
        }

        vars_.pop_back();
      }
    }
  private:
    std::vector<Variable> vars_;
};

class ExecutionPass {

  public:
    void execute(const GrammarUnit* unit);

  private:
    void executeScope(const ScopeUnit* unit);

    void executeStatement(const StatementUnit* unit);
    void executeIf(const IfUnit* unit);
    void executeLoop(const LoopUnit* unit);
    void executePrint(const PrintUnit* unit);
    void executeVarDecl(const VarDeclUnit* unit);
    void executeVarAssign(const VarAssignUnit* unit);

    int executeExpression(const ExpressionUnit* unit);
    int executeObject(const ObjectUnit* unit);
    int executeOperator(const OperatorUnit* unit);

  private:
    VariableList vars_;
};
