#include "../Headers/Lexer.hpp"

#include <iostream>

// ExprUnit    ::= AddSub
// AddSub      ::= AddExprUnit | SubExprUnit
// AddExprUnit ::= MulDiv ('+' MulDiv)*
// SubExprUnit ::= MulDiv ('-' MulDiv)*
// MulDiv      ::= MulExprUnit | DivExprUnit
// MulExprUnit ::= Brackets {'*' Brackets}* | Brackets
// DivExprUnit ::= Brackets {'/' Brackets}* | Brackets
// Brackets    ::= '('ExprUnit')' | Var | Num
// Var         ::= {'_', 'a-z', 'A-Z'}{'_', 'a-z', 'A-Z', '0-9'}*
// Num         ::= '-'{'0-9'}+ | {'0-9'}

using token_it = std::vector<Token>::const_iterator;

static GrammarUnit* getExpresion(token_it& cur_token, token_it end);
static GrammarUnit* getAddSub(token_it& cur_token, token_it end);
static GrammarUnit* getMulDiv(token_it& cur_token, token_it end);
static GrammarUnit* getBrackets(token_it& cur_token, token_it end);
static GrammarUnit* getVar(token_it& cur_token, token_it end);
static GrammarUnit* getNum(token_it& cur_token, token_it end);

static void recursiveUnitDelete(GrammarUnit* unit);

GrammarUnit* parse(const std::vector<Token>& tokens) {
    token_it cur_token = tokens.begin();
    GrammarUnit* result = getExpresion(cur_token, tokens.end());

    if (cur_token != tokens.end()) {
        std::cerr << "Error during parsing\n";
        return nullptr;
    }

    return result;
}

static GrammarUnit* getExpresion(token_it& cur_token, token_it end) {
    return getAddSub(cur_token, end);
}

static GrammarUnit* getAddSub(token_it& cur_token, token_it end) {
    GrammarUnit* result = getMulDiv(cur_token, end);

    if (result == nullptr) {
        std::cerr << "getAddSub: left operand is null\n";
        return nullptr;
    }

    while (cur_token != end) {
        if (!std::holds_alternative<OperatorToken>(*cur_token)) {
            std::cerr << "getAddSub: middle token is not operator\n";
            return nullptr;
        }

        GrammarUnit* left_op = result;

        OperatorType oper_type = std::get<OperatorToken>(*cur_token).oper();
        ++cur_token;

        GrammarUnit* right_op = getMulDiv(cur_token, end);
        if (right_op == nullptr) {
            std::cerr << "getAddSub: right operand is null\n";
            return nullptr;
        }

        if (oper_type == OperatorType::ADD) {
            result = new AddExprUnit(left_op, right_op);
        }
        else if (oper_type == OperatorType::SUB) {
            result = new SubExprUnit(left_op, right_op);
        }
        else {
            std::cerr << "getAddSub: operator is not add or sub\n";
            return nullptr;
        }
    }

    std::cout << "getAddSub: end func\n";
    return result;
}

static GrammarUnit* getMulDiv(token_it& cur_token, token_it end) {
    GrammarUnit* result = getBrackets(cur_token, end);

    if (result == nullptr) {
        std::cerr << "getMulDiv: left operand is null\n";
        return nullptr;
    }

    while (cur_token != end) {
        if (!std::holds_alternative<OperatorToken>(*cur_token)) {
            std::cerr << "getMulDiv: middle token is not operator\n";
            return nullptr;
        }

        GrammarUnit* left_op = result;

        OperatorType oper_type = std::get<OperatorToken>(*cur_token).oper();
        ++cur_token;

        GrammarUnit* right_op = getBrackets(cur_token, end);
        if (right_op == nullptr) {
            recursiveUnitDelete(left_op);
            std::cerr << "getMulDiv: right operand is null\n";
            return nullptr;
        }

        if (oper_type == OperatorType::MUL) {
            result = new MulExprUnit(left_op, right_op);
        }
        else if (oper_type == OperatorType::DIV) {
            result = new DivExprUnit(left_op, right_op);
        }
        else {
            recursiveUnitDelete(left_op);
            recursiveUnitDelete(right_op);
            std::cerr << "getMulDiv: operator is not mul or div\n";
            return nullptr;
        }
    }

    std::cout << "getMulDiv: end func\n";
    return result;
}

static GrammarUnit* getBrackets(token_it& cur_token, token_it end) {
    if (std::holds_alternative<NumToken>(*cur_token)) {
        return getNum(cur_token, end);
    }

    if (std::holds_alternative<NameToken>(*cur_token)) {
        return getVar(cur_token, end);
    }

    if (std::holds_alternative<SymbolToken>(*cur_token)) {
        std::cerr << "getBrackets: is not symbol token\n";
        return nullptr;
    }

    SpecialSymbolType sym_type = std::get<SymbolToken>(*cur_token).specSym();

    if (sym_type != SpecialSymbolType::LEFT_BRACKET) {
        std::cerr << "getBrackets: is not open bracket\n";
        return nullptr;
    }

    GrammarUnit* result = getExpresion(cur_token, end);
    if (result == nullptr) {
        std::cerr << "getBrackets: middle expresion is null\n";
        return nullptr;
    }

    if (sym_type != SpecialSymbolType::RIGHT_BRACKET) {
        recursiveUnitDelete(result);
        std::cerr << "getBrackets: is not close bracket\n";
        return nullptr;
    }

    return result;
}

static GrammarUnit* getVar(token_it& cur_token, token_it end) {

    return nullptr;
}

static GrammarUnit* getNum(token_it& cur_token, token_it end) {
    return nullptr;
}

static void recursiveUnitDelete(GrammarUnit* unit) {
    if (!unit)
        return;

    ExprUnit* expr_unit = dynamic_cast<ExprUnit*>(unit);

    if (expr_unit) {
        recursiveUnitDelete(expr_unit->left_op_);
        recursiveUnitDelete(expr_unit->right_op_);
    }

    delete expr_unit;
}