#include "../Headers/GraphicDumpPass.hpp"

#include <fstream>

static void writeNodeAndEdge(const GrammarUnit* node, std::ofstream& out);

void graphicDump(const GrammarUnit* node) {
    // log_out << "Graphic dump\n";

    std::ofstream out("GraphicDumps/dump");

    out << "digraph G{\n";
    out << "node[shape = record, fontsize = 14];\n";
    out << "splines = ortho\n";

    out << "info[label = \"root = 0x" << node << "\"]\n";

    writeNodeAndEdge(node, out);

    out << "}";

    system("dot GraphicDumps/dump -o GraphicDumps/Dump.png -T png");
}

void dumpOperator(const GrammarUnit* node, std::ofstream& out) {
    switch(node->getType())
    {
        case GrammarUnitType::ADD:
            out << "+";
            break;
        case GrammarUnitType::SUB:
            out << "-";
            break;
        case GrammarUnitType::MUL:
            out << "*";
            break;
        case GrammarUnitType::DIV:
            out << "/";
            break;
        // case OperatorType::EQUATE:
        //     out << ":=";
        //     break;
        // case OperatorType::IS_EQ:
        //     out << "=";
        //     break;
        // case OperatorType::IS_GE:
        //     out << "\\>=";
        //     break;
        // case OperatorType::IS_LE:
        //     out << "\\<=";
        //     break;
        // case OperatorType::IS_L:
        //     out << "\\<";
        //     break;
        // case OperatorType::IS_G:
        //     out << "\\>";
        //     break;
        // case OperatorType::IS_NOT_EQ:
        //     out << "\\<\\>";
        //     break;
        // case OperatorType::AND:
        //     out << "and";
        //     break;
        // case OperatorType::OR:
        //     out << "or";
        //     break;
        // case OperatorType::NOT:
        //     out << "not";
        //     break;
        default:
            out << "#";
            break;
    }
}

void printNodeInFormat(GrammarUnit* node, const char* color, std::ofstream& out) {
    "[style = \"filled,rounded\", fillcolor = \"%s\", label = \"{{ <v>";

    out << "GrammarUnit" << node << "[style = \"filled,rounded\","
        << "fillcolor = \"" << color << "\", label = \"{{ <v>";
}

static void writeNodeAndEdge(GrammarUnit* node, std::ofstream& out)
{
    const char light_green[]  = "#B1FF9F";
    const char light_red[]    = "#FF4646";
    const char light_blue[]   = "#87A7FF";
    const char light_grey[]   = "#C8C8C8";
    const char light_yellow[] = "#FFDC4B";


    switch (node->getType())
    {
        case GrammarUnitType::NUM:
        {
            printNodeInFormat(node, light_blue, out);
            NumUnit* num_node = dynamic_cast<NumUnit*>(node);
            if (num_node)
                out << "NUM | " << num_node->num();
            break;
        }
        case GrammarUnitType::ADD:
        case GrammarUnitType::SUB:
        case GrammarUnitType::MUL:
        case GrammarUnitType::DIV:
        {
            printNodeInFormat(node, light_yellow, out);
            out << "OPER | ";
            dumpOperator(node, out);
            break;
        }
        case GrammarUnitType::VAR:
        {
            printNodeInFormat(node, light_blue, out);
            VarUnit* var_node = dynamic_cast<VarUnit*>(node);
            if (var_node) {
                out << "VAR | " << var_node->name() << " ";
            }
            break;
        }
        // case GrammarUnitType::KEYWORD:
        // {
        //     printNodeInFormat(node, light_green, out);
        //     out << "KEYWORD | %s ", kKeywords[(int)node->val_.keyword]);
        //     break;
        // }
        // case GrammarUnitType::FICTITIOUS:
        // {
        //     printNodeInFormat(node, light_grey, out);
        //     out << "FICT ";
        //     break;
        // }
        // case GrammarUnitType::STR:
        // {
        //     printNodeInFormat(node, light_red, out);
        //     out << "STR | %s ", node->val_.var);
        //     break;
        // }
        default:
            out << "unknown";
            break;
    }

    out << "} }\"]\n";

    ExprUnit* expr_node = dynamic_cast<ExprUnit*>(node);
    if (expr_node) {
        if (expr_node->left() != nullptr) {
            out << "GrammarUnit" << node << " -> GrammarUnit" << expr_node->left()
                << "[xlabel = \"L\"]\n";
            writeNodeAndEdge(expr_node->left(),out);
        }
        if (expr_node->right() != nullptr) {
            out << "GrammarUnit" << node << " -> GrammarUnit" << expr_node->right()
                << "[xlabel = \"L\"]\n";
            writeNodeAndEdge(expr_node->right(), out);
        }
    }
}
