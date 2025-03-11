#include "../Headers/GraphicDumpPass.hpp"

#include <iostream>
#include <fstream>

static void writeNodeAndEdge(const GrammarUnit* node_, std::ofstream& out_);

GraphicDumpPass::GraphicDumpPass(const std::string file_name_prefix) :
    file_name_prefix_(file_name_prefix) {
}

void GraphicDumpPass::graphicDump(const GrammarUnit* node) {
    std::cout << "GraphicDumpPass::graphicDump\n";

    out_ = std::ofstream(fullDumpFileName());

    out_ << "digraph G{\n";
    out_ << "node[shape = record, fontsize = 14];\n";
    out_ << "splines = ortho\n";

    out_ << "info[label = \"root = 0x" << node_ << "\"]\n";

    node_ = node;
    writeNodeAndEdge();

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

void GraphicDumpPass::printNodeInFormat(const char* color) {
    "[style = \"filled,rounded\", fillcolor = \"%s\", label = \"{{ <v>";

    out_ << "GrammarUnit" << node_ << "[style = \"filled,rounded\","
        << "fillcolor = \"" << color << "\", label = \"{{ <v>";
}

void GraphicDumpPass::writeNodeAndEdge()
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
            printNodeInFormat(light_blue);
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
            printNodeInFormat(light_yellow);
            out_ << "OPER | ";
            dumpOperator();
            break;
        }
        case GrammarUnitType::VAR:
        {
            printNodeInFormat(light_blue);
            const VarUnit* var_node = dynamic_cast<const VarUnit*>(node_);
            if (var_node) {
                out_ << "VAR | " << var_node->name() << " ";
            }
            break;
        }
        // case GrammarUnitType::KEYWORD:
        // {
        //     printNodeInFormat(node_, light_green, out_);
        //     out_ << "KEYWORD | %s ", kKeywords[(int)node_->val_.keyword]);
        //     break;
        // }
        // case GrammarUnitType::FICTITIOUS:
        // {
        //     printNodeInFormat(node_, light_grey, out_);
        //     out_ << "FICT ";
        //     break;
        // }
        // case GrammarUnitType::STR:
        // {
        //     printNodeInFormat(node_, light_red, out_);
        //     out_ << "STR | %s ", node_->val_.var);
        //     break;
        // }
        default:
            out_ << "unknown";
            break;
    }

    out_ << "} }\"]\n";

    const BinaryOperUnit* expr_node = dynamic_cast<const BinaryOperUnit*>(node_);
    if (expr_node) {
        if (expr_node->left() != nullptr) {
            out_ << "GrammarUnit" << expr_node << " -> GrammarUnit" << expr_node->left()
                << "[xlabel = \"L\"]\n";

            node_ = expr_node->left();
            writeNodeAndEdge();
        }
        if (expr_node->right() != nullptr) {
            out_ << "GrammarUnit" << expr_node << " -> GrammarUnit" << expr_node->right()
                << "[xlabel = \"L\"]\n";
            node_ = expr_node->right();
            writeNodeAndEdge();
        }
    }
    else {
        const UnaryOperUnit* unary_op = dynamic_cast<const UnaryOperUnit*>(node_);
        if (unary_op != nullptr) {
            out_ << "GrammarUnit" << unary_op << " -> GrammarUnit" << unary_op->operand()
                << "[xlabel = \"L\"]\n";

            node_ = unary_op->operand();
            writeNodeAndEdge();
        }
    }
}
