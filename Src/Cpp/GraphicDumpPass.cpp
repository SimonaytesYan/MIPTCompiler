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
    return std::string("GraphicDumps/" + file_name_prefix_ + std::to_string(dump_counter_));
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
            break;
        }
        case GrammarUnitType::ADD:
        case GrammarUnitType::SUB:
        case GrammarUnitType::MUL:
        case GrammarUnitType::DIV:
        case GrammarUnitType::UNARY_MINUS:
        {
            dumpNodeInFormat(light_yellow);
            out_ << "OPER | ";
            dumpOperator();
            break;
        }
        case GrammarUnitType::VAR:
        {
            dumpNodeInFormat(light_blue);
            const VarUnit* var_node = dynamic_cast<const VarUnit*>(node_);
            if (var_node) {
                out_ << "VAR | " << var_node->name() << " ";
            }
            break;
        }
        case GrammarUnitType::VAR_DECL: {
            dumpNodeInFormat(light_red);
            out_ << "VAR DECL ";
            break;
        }
        // case GrammarUnitType::KEYWORD:
        // {
        //     dumpNodeInFormat(node_, light_green, out_);
        //     out_ << "KEYWORD | %s ", kKeywords[(int)node_->val_.keyword]);
        //     break;
        // }
        // case GrammarUnitType::FICTITIOUS:
        // {
        //     dumpNodeInFormat(node_, light_grey, out_);
        //     out_ << "FICT ";
        //     break;
        // }
        // case GrammarUnitType::STR:
        // {
        //     dumpNodeInFormat(node_, light_red, out_);
        //     out_ << "STR | %s ", node_->val_.var);
        //     break;
        // }
        default:
            out_ << "unknown";
            break;
    }

    out_ << "} }\"]\n";

    switch (node_->getType())
    {
    case GrammarUnitType::ADD:
    case GrammarUnitType::SUB:
    case GrammarUnitType::MUL:
    case GrammarUnitType::DIV: {
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
        break;
    }

    case GrammarUnitType::UNARY_MINUS: {
        const UnaryOperUnit* unary_op = reinterpret_cast<const UnaryOperUnit*>(node_);
        if (unary_op->operand() != nullptr) {
            dumpEdge(unary_op, unary_op->operand(), "");
            node_ = unary_op->operand();
            dumpNodeAndEdge();
        }
        break;
    }

    case GrammarUnitType::VAR_DECL: {
        const VarDeclUnit* var_decl = reinterpret_cast<const VarDeclUnit*>(node_);
        if (var_decl != nullptr) {
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
        }
    }
    case GrammarUnitType::SCOPE: {
        const ScopeUnit* scope = reinterpret_cast<const ScopeUnit*>(node_);
        size_t statement_num = 0;
        for (auto statement : *scope) {
            statement_num++;
            dumpEdge(scope, statement, std::to_string(statement_num).c_str());
            node_ = statement;
            dumpNodeAndEdge();
        }
    }
    default:
        break;
    }
}
