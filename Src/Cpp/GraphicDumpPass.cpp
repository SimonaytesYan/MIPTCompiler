#include "../Headers/GraphicDumpPass.hpp"

#include <iostream>
#include <fstream>

static void dumpNodeAndEdge(const GrammarUnit* node_, std::ofstream& out_);

GraphicDumpPass::GraphicDumpPass(const std::string file_name_prefix) :
    file_name_prefix_(file_name_prefix) {
}

void GraphicDumpPass::graphicDump(const GrammarUnit* node) {
    node_ = node;

    std::cout << "GraphicDumpPass::graphicDump\n";

    out_ = std::ofstream(fullDumpFileName());

    out_ << "digraph G{\n";
    out_ << "node[shape = record, fontsize = 14];\n";
    out_ << "splines = ortho\n";

    out_ << "info[label = \"root = 0x" << node_ << "\"]\n";

    dumpNodeAndEdge();

    out_ << "}";

    out_.close();
    createPngFromDot();

    dump_counter_++;
}

std::string GraphicDumpPass::fullDumpFileName() {
    return std::string("graphic_dumps/" + file_name_prefix_ + std::to_string(dump_counter_));
}

void GraphicDumpPass::createPngFromDot() {
    std::cout << "GraphicDumpPass::createPngFromDot\n";

    std::string num = std::to_string(dump_counter_);
    std::string command = "dot " + fullDumpFileName() + " -o " + fullDumpFileName() + ".png -T png";

    std::cout << "command = <" << command << ">\n";

    system(command.c_str());
}

void GraphicDumpPass::dumpOperator() {
    if (node_ == nullptr)
        return;

    switch(node_->getType())
    {
        case GrammarUnitType::ADD:
            out_ << "+";
            break;
        case GrammarUnitType::UNARY_MINUS:
        case GrammarUnitType::SUB:
            out_ << "-";
            break;
        case GrammarUnitType::MUL:
            out_ << "*";
            break;
        case GrammarUnitType::DIV:
            out_ << "/";
            break;
        // case OperatorType::EQUATE:
        //     out_ << ":=";
        //     break;
        // case OperatorType::IS_EQ:
        //     out_ << "=";
        //     break;
        // case OperatorType::IS_GE:
        //     out_ << "\\>=";
        //     break;
        // case OperatorType::IS_LE:
        //     out_ << "\\<=";
        //     break;
        // case OperatorType::IS_L:
        //     out_ << "\\<";
        //     break;
        // case OperatorType::IS_G:
        //     out_ << "\\>";
        //     break;
        // case OperatorType::IS_NOT_EQ:
        //     out_ << "\\<\\>";
        //     break;
        // case OperatorType::AND:
        //     out_ << "and";
        //     break;
        // case OperatorType::OR:
        //     out_ << "or";
        //     break;
        // case OperatorType::NOT:
        //     out_ << "not";
        //     break;
        default:
            out_ << "#";
            break;
    }
}

void GraphicDumpPass::dumpEdge(const GrammarUnit* from, const GrammarUnit* to, const char* name) {
    out_ << "GrammarUnit" << from << " -> GrammarUnit" << to
    << "[xlabel = \"" << name << "\"]\n";
}

void GraphicDumpPass::dumpNodeInFormat(const char* color) {
    "[style = \"filled,rounded\", fillcolor = \"%s\", label = \"{{ <v>";

    out_ << "GrammarUnit" << node_ << "[style = \"filled,rounded\","
        << "fillcolor = \"" << color << "\", label = \"{{ <v>";
}

void GraphicDumpPass::dumpNodeAndEdge()
{
    if (node_ == nullptr)
        return;

    const char light_green[]  = "#B1FF9F";
    const char light_red[]    = "#FF4646";
    const char light_blue[]   = "#87A7FF";
    const char light_grey[]   = "#C8C8C8";
    const char light_yellow[] = "#FFDC4B";


    switch (node_->getType())
    {
        case GrammarUnitType::NUM:
        {
            dumpNodeInFormat(light_blue);
            const NumUnit* num_node = dynamic_cast<const NumUnit*>(node_);
            if (num_node)
                out_ << "NUM | " << num_node->num();
            out_ << "} }\"]\n";
            return;
        }
        case GrammarUnitType::ADD:
        case GrammarUnitType::SUB:
        case GrammarUnitType::MUL:
        case GrammarUnitType::DIV:
        {
            dumpNodeInFormat(light_yellow);
            out_ << "OPER | ";
            dumpOperator();
            out_ << "} }\"]\n";

            const BinaryOperUnit* bin_oper_node = dynamic_cast<const BinaryOperUnit*>(node_);
            if (bin_oper_node->left() != nullptr) {
                dumpEdge(bin_oper_node, bin_oper_node->left(), "L");
                node_ = bin_oper_node->left();
                dumpNodeAndEdge();
            }
            if (bin_oper_node->right() != nullptr) {
                dumpEdge(bin_oper_node, bin_oper_node->right(), "R");
                node_ = bin_oper_node->right();
                dumpNodeAndEdge();
            }
            return;
        }
        case GrammarUnitType::UNARY_MINUS:
        {
            dumpNodeInFormat(light_yellow);
            out_ << "UNARY_OPER | ";
            dumpOperator();
            out_ << "} }\"]\n";

            const UnaryOperUnit* unary_op = reinterpret_cast<const UnaryOperUnit*>(node_);
            if (unary_op->operand() != nullptr) {
                dumpEdge(unary_op, unary_op->operand(), "");
                node_ = unary_op->operand();
                dumpNodeAndEdge();
            }
            return;
        }
        case GrammarUnitType::VAR:
        {
            dumpNodeInFormat(light_blue);
            const VarUnit* var_node = reinterpret_cast<const VarUnit*>(node_);
            out_ << "VAR | " << var_node->name() << " ";
            out_ << "} }\"]\n";
            return;
        }
        case GrammarUnitType::VAR_DECL: {
            dumpNodeInFormat(light_green);
            out_ << "VAR DECL ";
            out_ << "} }\"]\n";

            const VarDeclUnit* var_decl = reinterpret_cast<const VarDeclUnit*>(node_);
            if (var_decl->var() != nullptr) {
                dumpEdge(var_decl, var_decl->var(), "Var");
                node_ = var_decl->var();
                dumpNodeAndEdge();
            }
            if (var_decl->expr() != nullptr) {
                dumpEdge(var_decl, var_decl->expr(), "Expr");
                node_ = var_decl->expr();
                dumpNodeAndEdge();
            }
            return;
        }
        case GrammarUnitType::SCOPE: {
            dumpNodeInFormat(light_green);
            out_ << "SCOPE ";
            out_ << "} }\"]\n";

            const ScopeUnit* scope = reinterpret_cast<const ScopeUnit*>(node_);
            size_t statement_num = 0;
            for (auto statement : *scope) {
                statement_num++;
                dumpEdge(scope, statement, std::to_string(statement_num).c_str());
                node_ = statement;
                dumpNodeAndEdge();
            }
            return;
        }
        case GrammarUnitType::IF: {
            dumpNodeInFormat(light_green);

            out_ << "IF ";
            out_ << "} }\"]\n";

            const IfUnit* if_decl = reinterpret_cast<const IfUnit*>(node_);
            if (if_decl->condition() != nullptr) {
                dumpEdge(if_decl, if_decl->condition(), "Cond");
                node_ = if_decl->condition();
                dumpNodeAndEdge();
            }
            if (if_decl->true_branch() != nullptr) {
                dumpEdge(if_decl, if_decl->true_branch(), "True");
                node_ = if_decl->true_branch();
                dumpNodeAndEdge();
            }
            if (if_decl->false_branch() != nullptr) {
                dumpEdge(if_decl, if_decl->false_branch(), "False");
                node_ = if_decl->false_branch();
                dumpNodeAndEdge();
            }
            return;
        }
        case GrammarUnitType::LOOP: {
            dumpNodeInFormat(light_green);

            out_ << "LOOP ";
            out_ << "} }\"]\n";

            const LoopUnit* loop_decl = reinterpret_cast<const LoopUnit*>(node_);
            if (loop_decl->condition() != nullptr) {
                dumpEdge(loop_decl, loop_decl->condition(), "Cond");
                node_ = loop_decl->condition();
                dumpNodeAndEdge();
            }
            if (loop_decl->body() != nullptr) {
                dumpEdge(loop_decl, loop_decl->body(), "Body");
                node_ = loop_decl->body();
                dumpNodeAndEdge();
            }
            return;
        }
        case GrammarUnitType::PRINT: {
            dumpNodeInFormat(light_green);
            out_ << "PRINT ";
            out_ << "} }\"]\n";

            const PrintUnit* print_decl = reinterpret_cast<const PrintUnit*>(node_);
            if (print_decl->expression() != nullptr) {
                dumpEdge(print_decl, print_decl->expression(), "Expr");
                node_ = print_decl->expression();
                dumpNodeAndEdge();
            }
            return;
        }
        case GrammarUnitType::VAR_ASSIGN: {
            dumpNodeInFormat(light_green);

            out_ << "VAR ASSIGN ";
            out_ << "} }\"]\n";

            const VarAssignUnit* var_decl = reinterpret_cast<const VarAssignUnit*>(node_);
            if (var_decl->var() != nullptr) {
                dumpEdge(var_decl, var_decl->var(), "Var");
                node_ = var_decl->var();
                dumpNodeAndEdge();
            }
            if (var_decl->expr() != nullptr) {
                dumpEdge(var_decl, var_decl->expr(), "Expr");
                node_ = var_decl->expr();
                dumpNodeAndEdge();
            }
            return;
        }
        default: {
            out_ << "unknown";
            return;
        }
    }
}
