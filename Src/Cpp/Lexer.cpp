#include "../Headers/Lexer.hpp"

#include <iostream>

// ExprUnit    ::= AddSub
// AddSub      ::= AddExprUnit | SubExprUnit
// AddExprUnit ::= AddSub '+' AddSub | MulDiv
// SubExprUnit ::= AddSub '-' AddSub | MulDiv
// MulDiv      ::= MulExprUnit | DivExprUnit
// MulExprUnit ::= MulDiv '*' MulDiv | Brackets
// DivExprUnit ::= MulDiv '/' MulDiv | Brackets
// Brackets    ::= '('Brackets')' | ExprUnit | Var | Num
// Var         ::= {'_', 'a-z', 'A-Z'}{'_', 'a-z', 'A-Z', '0-9'}*
// Num         ::= '-'{'0-9'}+ | {'0-9'}

using token_it = std::vector<Token>::const_iterator;

static ExprUnit* getExpresion(token_it& cur_token, token_it end);
static ExprUnit* getAddSub(token_it& cur_token, token_it end);
static ExprUnit* getMulDiv(token_it& cur_token, token_it end);
static ExprUnit* getBrackets(token_it& cur_token, token_it end);
static ExprUnit* getVar(token_it& cur_token, token_it end);
static ExprUnit* getNum(token_it& cur_token, token_it end);

GrammarUnit* parse(const std::vector<Token>& tokens) {
    token_it cur_token = tokens.begin();
    GrammarUnit* result = getExpresion(cur_token, tokens.end());

    if (cur_token != tokens.end()) {
        std::cerr << "Error during parsing\n";
        return nullptr;
    }

    return result;
}

static ExprUnit* getExpresion(token_it& cur_token, token_it end) {
    return getAddSub(cur_token, end);
}

static ExprUnit* getAddSub(token_it& cur_token, token_it end) {
    // ExprUnit* result = nullptr;

    // ExprUnit* left_op = getAddSub(cur_token, end);
    // if (left_op == nullptr) {
    //     std::cerr << "getAddSub: left operator is null\n";
    //     return nullptr;
    // }

    // if (!std::holds_alternative<OperatorToken>(*cur_token)) {
    //     std::cout << "getAddSub: middle token is not add/sub operator\n";
    //     return left_op;
    // }

    // OperatorType oper_type = std::get<OperatorToken>(*cur_token).oper();
    // ++cur_token;

    // ExprUnit* right_op = getAddSub(cur_token, end);
    // if (right_op == nullptr) {
    //     std::cerr << "getAddSub: right token is null\n";
    //     return nullptr;
    // }

    // if (oper_type == OperatorType::ADD) {
    //     result = new AddExprUnit(left_op, right_op);
    // }
    // else if (oper_type == OperatorType::SUB) {
    //     result = new SubExprUnit(left_op, right_op);
    // }
    // else {
    //     return nullptr;
    // }

    return nullptr;
}

static ExprUnit* getMulDiv(token_it& cur_token, token_it end) {
    return nullptr;
}

static ExprUnit* getBrackets(token_it& cur_token, token_it end) {
    return nullptr;
}

static ExprUnit* getVar(token_it& cur_token, token_it end) {
    return nullptr;
}

static ExprUnit* getNum(token_it& cur_token, token_it end) {
    return nullptr;
}

