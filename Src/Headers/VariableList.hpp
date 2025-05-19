#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace execution_pass {

struct IntVar {
  int value_;
  std::string name_;
};

struct ScopeStarter {
};

using Variable = std::variant<IntVar, ScopeStarter>;

class VariableList {
  public:
    void Dump(std::ostream& out);

    void addNewVar(Variable var);
    void setVarValue(std::string name, int value);
    int getVarValue(std::string name);

    void startScope();
    void endScope();

  private:
    std::vector<Variable> vars_;
};

} // end namespace execution_pass