#include "VariableList.hpp"

void VariableList::Dump(std::ostream& out) {
    out << "\nVariable list dump\n";
    for (const Variable& var : vars_) {
        if (std::holds_alternative<ScopeStarter>(var)) {
            out << "{ ";
        }
        else {
            IntVar cur_var = std::get<IntVar>(var);
            out << "'" << cur_var.name_ << "', ";
        }
    }
    out << "\n";
}

void VariableList::addNewVar(Variable var) {
    vars_.push_back(var);
}

void VariableList::setVarValue(std::string name, int value) {
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
    Dump(std::cerr);
}

int VariableList::getVarValue(std::string name) {
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
    Dump(std::cerr);
    return 0;
}

void VariableList::startScope() {
    vars_.push_back(ScopeStarter());
}

void VariableList::endScope() {
    const int var_num = vars_.size();

    for (int var_ind = var_num - 1; var_ind >= 0; var_ind--) {
        if (std::holds_alternative<ScopeStarter>(vars_[var_ind])) {
            vars_.pop_back();
            break;
        }

        vars_.pop_back();
    }
}