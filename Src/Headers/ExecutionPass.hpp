#pragma once

#include "Grammar.hpp"
#include <string>
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
    void addNewVar(Variable var) {
      vars_.push_back(var);
    }

    int getVarValue(std::string name) {
        const size_t var_num = vars_.size();

        for (size_t var_ind = var_num - 1; var_ind >= 0; var_ind--) {
            if (std::holds_alternative<ScopeStarter>(vars_[var_ind])) {
              continue;
            }

          const IntVar& cur_var = std::get<IntVar>(vars_[var_ind]);
          if (name == cur_var.name_)
            return cur_var.value_;
        }

        std::cerr << "Error in VariableList: unknown variable name\n";
        return 0;
    }

    void startScope() {
      vars_.push_back(ScopeStarter());
    }

    void endScope() {
      const size_t var_num = vars_.size();

      for (size_t var_ind = var_num - 1; var_ind >= 0; var_ind--) {

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
    void execute(GrammarUnit* unit);

  private:
    void executeScope(ScopeUnit* unit);

    void executeStatement(StatementUnit* unit);
    void executeIf(IfUnit* unit);
    void executeLoop(LoopUnit* unit);
    void executePrint(PrintUnit* unit);
    void executeVarDecl(VarDeclUnit* unit);
    void executeVarAssign(VarAssignUnit* unit);

    int executeExpression(ExpressionUnit* unit);
    int executeObject(ObjectUnit* unit);
    int executeOperator(OperatorUnit* unit);

  private:
    VariableList vars_;
};
