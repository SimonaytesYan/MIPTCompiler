#include "../Headers/Lexer.hpp"

#include <iostream>

// ScopeUnit      ::= {StatementUnit}+
// StatementUnit  ::= VarDeclUnit
// VarDeclUnit    ::= 'let' Var '=' ExprUnit ';'
// ExpressionUnit ::= AddSub
// AddSub         ::= AddExprUnit | SubExprUnit
// AddExprUnit    ::= MulDiv {'+' MulDiv}*
// SubExprUnit    ::= MulDiv {'-' MulDiv}*
// MulDiv         ::= MulExprUnit | DivExprUnit
// MulExprUnit    ::= Brackets {'*' Brackets}* | Brackets
// DivExprUnit    ::= Brackets {'/' Brackets}* | Brackets
// Brackets       ::= '('ExprUnit')' | UnaryMinus
// UnaryMinus     ::= '-' Object | Object
// Object         ::= Var | Num
// Var            ::= {'_', 'a-z', 'A-Z'}{'_', 'a-z', 'A-Z', '0-9'}*
// Num            ::= '-'{'0-9'}+ | {'0-9'}

using TokenIt = std::vector<Token>::const_iterator;

static ScopeUnit*     getScope(TokenIt& cur_token, TokenIt end);
static StatementUnit* getStatement(TokenIt& cur_token, TokenIt end);
static StatementUnit* getVarDecl(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getExpresion(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getAddSub(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getMulDiv(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getBrackets(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getUnaryMinus(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getObject(TokenIt& cur_token, TokenIt end);
static VarUnit* getVar(TokenIt& cur_token, TokenIt end);
static NumUnit* getNum(TokenIt& cur_token, TokenIt end);

void recursiveUnitDelete(GrammarUnit* unit);

GrammarUnit* parse(const std::vector<Token>& tokens) {
    TokenIt cur_token = tokens.begin();
    GrammarUnit* result = getStatement(cur_token, tokens.end());

    if (cur_token != tokens.end()) {
        std::cerr << "Error during parsing\n";
        return nullptr;
    }

    return result;
}

// static ScopeUnit* getScope(TokenIt& cur_token, TokenIt end) {
//     if (cur_token == end) {
//         std::cerr << "getStatement: cur_token == end\n";
//         return nullptr;
//     }

//     if (!std::holds_alternative<SpecialSymbolToken>(*cur_token)) {
//         std::cerr << "getStatement: Scope not start from {\n";
//         return nullptr;
//     }
//     std::get<>

//     ScopeUnit* scope = new ScopeUnit();
//     do {
//         if (std::holds_alternative<SpecialSymbolToken>(*cur_token)) {
//             scope->addStatements();
//         }

//     } while (cur_token != end);

//     return scope;
// }

static StatementUnit* getStatement(TokenIt& cur_token, TokenIt end) {
    std::cout << "getStatement: start func\n";
    if (cur_token == end) {
        std::cerr << "getStatement: cur_token == end\n";
        return nullptr;
    }
    if (std::holds_alternative<KeywordToken>(*cur_token)) {
        if (std::get<KeywordToken>(*cur_token).keyword() == KeywordType::LET) {
            return getVarDecl(cur_token, end);
        }
    }

    std::cerr << "getStatement: Unexpected token type\n";
    return nullptr;
}

static StatementUnit* getVarDecl(TokenIt& cur_token, TokenIt end) {
    std::cout << "getVarDecl: start func\n";

    if (cur_token == end) {
        std::cerr << "getVarDecl: cur_token == end\n";
        return nullptr;
    }

    if (!std::holds_alternative<KeywordToken>(*cur_token)) {
        std::cerr << "getVarDecl: not keyword\n";
        return nullptr;
    }
    if (std::get<KeywordToken>(*cur_token).keyword() != KeywordType::LET) {
        std::cerr << "getVarDecl: not let\n";
        return nullptr;
    }

    ++cur_token;

    VarUnit* var = getVar(cur_token, end);

    if (var == nullptr) {
        std::cerr << "getVarDecl: var in null";
        return nullptr;
    }

    if (!std::holds_alternative<OperatorToken>(*cur_token)) {
        std::cerr << "getVarDecl: not operator in the middle\n";
        return nullptr;
    }

    if (std::get<OperatorToken>(*cur_token).oper() != OperatorType::EQUAL) {
        std::cerr << "getVarDecl: not operator = in the middle\n";
        return nullptr;
    }

    ++cur_token;

    ExpressionUnit* expression = getExpresion(cur_token, end);
    if (expression == nullptr) {
        std::cerr << "getVarDecl: expression in null";
        return nullptr;
    }

    if (!std::holds_alternative<SpecialSymbolToken>(*cur_token)) {
        std::cerr << "getVarDecl: not operator in the middle\n";
        return nullptr;
    }

    if (std::get<SpecialSymbolToken>(*cur_token).specSym() != SpecialSymbolType::END_STATEMENT) {
        std::cerr << "getVarDecl: not operator = in the middle\n";
        return nullptr;
    }

    ++cur_token;

    return new VarDeclUnit(var, expression);
}

static ExpressionUnit* getExpresion(TokenIt& cur_token, TokenIt end) {
    std::cout << "getExpresion: start func\n";
    return getAddSub(cur_token, end);
}

static ExpressionUnit* getAddSub(TokenIt& cur_token, TokenIt end) {
    std::cout << "getAddSub: start func\n";
    ExpressionUnit* result = getMulDiv(cur_token, end);

    if (result == nullptr) {
        std::cerr << "getAddSub: left operand is null\n";
        return nullptr;
    }

    while (cur_token != end) {
        if (!std::holds_alternative<OperatorToken>(*cur_token) ||
            (std::get<OperatorToken>(*cur_token).oper() != OperatorType::SUB &&
            std::get<OperatorToken>(*cur_token).oper() != OperatorType::ADD)) {
            std::cout << "getAddSub: middle token is not operator\n";
            break;
        }

        ExpressionUnit* left_op = result;

        OperatorType oper_type = std::get<OperatorToken>(*cur_token).oper();
        ++cur_token;

        ExpressionUnit* right_op = getMulDiv(cur_token, end);
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

static ExpressionUnit* getMulDiv(TokenIt& cur_token, TokenIt end) {
    std::cout << "getMulDiv: start func\n";
    ExpressionUnit* result = getBrackets(cur_token, end);

    if (result == nullptr) {
        std::cerr << "getMulDiv: left operand is null\n";
        return nullptr;
    }

    while (cur_token != end) {
        if (!std::holds_alternative<OperatorToken>(*cur_token) ||
            (std::get<OperatorToken>(*cur_token).oper() != OperatorType::MUL &&
            std::get<OperatorToken>(*cur_token).oper() != OperatorType::DIV)) {
            std::cout << "getMulDiv: middle token is not operator\n";
            break;
        }

        ExpressionUnit* left_op = result;

        OperatorType oper_type = std::get<OperatorToken>(*cur_token).oper();
        ++cur_token;

        ExpressionUnit* right_op = getBrackets(cur_token, end);
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

static ExpressionUnit* getBrackets(TokenIt& cur_token, TokenIt end) {
    std::cout << "getBrackets: start func\n";

    if (!std::holds_alternative<SpecialSymbolToken>(*cur_token)) {
        return getUnaryMinus(cur_token, end);
    }

    SpecialSymbolType sym_type = std::get<SpecialSymbolToken>(*cur_token).specSym();

    if (sym_type != SpecialSymbolType::LEFT_BRACKET) {
        std::cerr << "getBrackets: is not open bracket\n";
        return nullptr;
    }
    ++cur_token;

    ExpressionUnit* result = getExpresion(cur_token, end);
    if (result == nullptr) {
        std::cerr << "getBrackets: middle expresion is null\n";
        return nullptr;
    }

    if (!std::holds_alternative<SpecialSymbolToken>(*cur_token)) {
        std::cerr << "getBrackets: is not close bracket\n";
        recursiveUnitDelete(result);
        return nullptr;
    }

    sym_type = std::get<SpecialSymbolToken>(*cur_token).specSym();
    if (sym_type != SpecialSymbolType::RIGHT_BRACKET) {
        recursiveUnitDelete(result);
        std::cerr << "getBrackets: is not close bracket(incorrect symbol)\n";
        return nullptr;
    }
    ++cur_token;

    return result;
}

static ExpressionUnit* getUnaryMinus(TokenIt& cur_token, TokenIt end) {
    std::cout << "getUnaryMinus:: start func\n";
    if (std::holds_alternative<OperatorToken>(*cur_token)) {
        if (std::get<OperatorToken>(*cur_token).oper() != OperatorType::SUB) {
            std::cerr << "getUnaryMinus:: not minus operator\n";
            return nullptr;
        }
        cur_token++;

        ExpressionUnit* operand = getObject(cur_token, end);
        if (operand == nullptr) {
            std::cerr << "getUnaryMinus: operand is null\n";
            return nullptr;
        }
        return new UnaryOperMinus(operand);
    }

    return getObject(cur_token, end);
}

static ExpressionUnit* getObject(TokenIt& cur_token, TokenIt end) {
    if (std::holds_alternative<NumToken>(*cur_token)) {
        return getNum(cur_token, end);
    }

    if (std::holds_alternative<NameToken>(*cur_token)) {
        return getVar(cur_token, end);
    }

    return nullptr;
}


static VarUnit* getVar(TokenIt& cur_token, TokenIt end) {
    std::cout << "getVar: start func\n";
    if (!std::holds_alternative<NameToken>(*cur_token)) {
        std::cerr << "getVar: is not NameToken\n";
        return nullptr;
    }

    VarUnit* result = new VarUnit(std::get<NameToken>(*cur_token).name());
    ++cur_token;
    std::cout << "getVar: end\n";
    return result;
}

static NumUnit* getNum(TokenIt& cur_token, TokenIt end) {
    std::cout << "getNum: start func\n";

    if (!std::holds_alternative<NumToken>(*cur_token)) {
        std::cerr << "getVar: is not NumToken\n";
        return nullptr;
    }

    int num = std::get<NumToken>(*cur_token).num();
    ++cur_token;

    std::cout << "getNum: end\n";
    return new NumUnit(num);
}

void recursiveUnitDelete(GrammarUnit* unit) {
    if (!unit)
        return;

    BinaryOperUnit* binary_op = dynamic_cast<BinaryOperUnit*>(unit);

    if (binary_op) {
        recursiveUnitDelete(binary_op->left_op_);
        recursiveUnitDelete(binary_op->right_op_);
    }

    UnaryOperUnit* unary_unit = dynamic_cast<UnaryOperUnit*>(unit);
    if (unary_unit) {
        recursiveUnitDelete(unary_unit->operand());
    }

    VarDeclUnit* var_decl_unit = dynamic_cast<VarDeclUnit*>(unit);
    if (var_decl_unit) {
        recursiveUnitDelete(var_decl_unit->var());
        recursiveUnitDelete(var_decl_unit->expr());
    }

    delete unit;
}
