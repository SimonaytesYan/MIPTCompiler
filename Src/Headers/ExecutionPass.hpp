#pragma once

#include "Grammar.hpp"
#include <string>
#include <vector>

struct IntVar {
  int value_;
  std::string name_;
};

struct StartScope {
};

using Variable = std::variant<IntVar, StartScope>;

class VariableList {
  public:
    void AddNewVar(Variable var) {
      vars_.push_back(var);
    }

    int GetVarValue(std::string name) {
        const size_t var_num = vars_.size();

        for (size_t var_ind = var_num - 1; var_ind >= 0; var_ind--) {
            if (std::holds_alternative<StartScope>(vars_[var_ind])) {
              continue;
            }

          const IntVar& cur_var = std::get<IntVar>(vars_[var_ind]);
          if (name == cur_var.name_)
            return cur_var.value_;
        }

        std::cerr << "Error in VariableList: unknown variable name\n";
        return 0;
    }

    void EndScope() {
      const size_t var_num = vars_.size();

      for (size_t var_ind = var_num - 1; var_ind >= 0; var_ind--) {

        if (std::holds_alternative<StartScope>(vars_[var_ind])) {
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
    void Execute(GrammarUnit* unit);

  private:
    void ExecuteScope(ScopeUnit* unit);

    void ExecuteStatement(StatementUnit* unit);
    void ExecuteIf(IfUnit* unit);
    void ExecuteLoop(LoopUnit* unit);
    void ExecutePrint(PrintUnit* unit);
    void ExecuteVarDecl(VarDeclUnit* unit);
    void ExecuteVarAssign(VarAssignUnit* unit);

    int ExecuteExpression(ExpressionUnit* unit);
    int ExecuteObject(ObjectUnit* unit);
    int ExecuteOperator(OperatorUnit* unit);

  private:
    VariableList vars_;
};
