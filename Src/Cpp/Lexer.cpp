#include "Lexer.hpp"
#include "Logger.hpp"
#include "Grammar.hpp"
#include "Keywords.hpp"
#include "Operators.hpp"
#include "SpecialSymbols.hpp"
#include "Tokens.hpp"
#include "Utils.hpp"

#include <iostream>
#include <variant>
#include <vector>

// ScopeUnit      ::= '{' {StatementUnit}+ '}'
// StatementUnit  ::= VarAssignUnit | VarDeclUnit | IFUnit | LoopUnit | PrintUnit | 0
// PrintUnit      ::= 'print(' ExprUnit ');'
// LoopUnit       ::= loop '(' ExprUnit ')' ScopeUnit
// IFUnit         ::= 'if' '(' ExprUnit ')' ScopeUnit 'else' ScopeUnit
// VarDeclUnit    ::= 'let' VarAssignUnit
// VarAssignUnit  ::= VarUnit '=' ExprUnit ';'
// ExpressionUnit ::= AddSub
// AddSub         ::= AddExprUnit | SubExprUnit
// AddExprUnit    ::= MulDiv {'+' MulDiv}*
// SubExprUnit    ::= MulDiv {'-' MulDiv}*
// MulDiv         ::= MulExprUnit | DivExprUnit
// MulExprUnit    ::= Brackets {'*' Brackets}* | Brackets
// DivExprUnit    ::= Brackets {'/' Brackets}* | Brackets
// Brackets       ::= '('ExprUnit')' | UnaryMinus
// UnaryMinus     ::= '-' Object | Object
// Object         ::= VarUnit | NumUnit | ArrayUnit | FloatUnit
// VarUnit        ::= {'_', 'a-z', 'A-Z'}{'_', 'a-z', 'A-Z', '0-9'}*
// NumUnit        ::= {'0-9'}+
// FloatUnit      ::= {{'0-9'}+}'.'{{'0-9'}+}
// ArrayUnit      ::= [] | [ExprUnit{, ExprUnit}*]

// using TokenIt = std::vector<Token>::const_iterator;

static ScopeUnit*     getScope(TokenIt& cur_token, TokenIt end);
static StatementUnit* getStatement(TokenIt& cur_token, TokenIt end);
static StatementUnit* getIf(TokenIt& cur_token, TokenIt end);
static StatementUnit* getPrint(TokenIt& cur_token, TokenIt end);
static StatementUnit* getLoop(TokenIt& cur_token, TokenIt end);
static StatementUnit* getVarDecl(TokenIt& cur_token, TokenIt end);
static StatementUnit* getVarAssign(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getExpresion(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getAddSub(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getMulDiv(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getBrackets(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getUnaryMinus(TokenIt& cur_token, TokenIt end);
static ExpressionUnit* getObject(TokenIt& cur_token, TokenIt end);
static VarUnit* getVar(TokenIt& cur_token, TokenIt end);
static NumUnit* getNum(TokenIt& cur_token, TokenIt end);
static FloatUnit* getFloat(TokenIt& cur_token, TokenIt end);
static ArrayUnit* getArrayUnit(TokenIt& cur_token, TokenIt end);

template<class TokenType, class TokenValueType>
static bool CheckTokenValue(const TokenIt token, TokenValueType necessary_value) {
    if (!std::holds_alternative<TokenType>(*token))
        return false;
    if (GetTokenVal<TokenType, TokenValueType>(token) != necessary_value)
        return false;
    return true;
}

template<class TokenType>
static bool CheckTokenType(const TokenIt token) {
    return std::holds_alternative<TokenType>(*token);
}

void recursiveUnitDelete(GrammarUnit* unit);

GrammarUnit* parse(const std::vector<Token>& tokens) {
    TokenIt cur_token = tokens.begin();
    GrammarUnit* result = getScope(cur_token, tokens.end());

    if (cur_token != tokens.end()) {
        std::cerr << "Error during parsing\n";
        return nullptr;
    }

    return result;
}

static ScopeUnit* getScope(TokenIt& cur_token, TokenIt end) {
    LOG << "getScope: Start func\n";
    if (cur_token == end) {
        std::cerr << "getScope: cur_token == end\n";
        return nullptr;
    }

    if (!CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::START_SCOPE)) {
        std::cerr << "getScope: Scope not start from {\n";
        return nullptr;
    }

    ++cur_token;

    ScopeUnit* scope = new ScopeUnit();
    while (cur_token != end &&
           !CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::END_SCOPE))
    {
        LOG << "getScope: new_step\n";

        StatementUnit* next_statement = getStatement(cur_token, end);
        if (next_statement == nullptr) {
            std::cerr << "getScope: next statement = null\n";
            return nullptr;
        }

        scope->addStatements(next_statement);
    }

    if (cur_token == end) {
        std::cerr << "getScope: Scope not end with }\n";
        return nullptr;
    }

    ++cur_token;
    LOG << "getScope: end function\n";
    return scope;
}

static StatementUnit* getStatement(TokenIt& cur_token, TokenIt end) {
    LOG << "getStatement: start func\n";
    if (cur_token == end) {
        std::cerr << "getStatement: cur_token == end\n";
        return nullptr;
    }

    if (CheckTokenValue<KeywordToken, KeywordType>(cur_token, KeywordType::LET)) {
        return getVarDecl(cur_token, end);
    }

    if (CheckTokenValue<KeywordToken, KeywordType>(cur_token, KeywordType::IF)) {
        return getIf(cur_token, end);
    }

    if (CheckTokenValue<KeywordToken, KeywordType>(cur_token, KeywordType::LOOP)) {
        return getLoop(cur_token, end);
    }

    if (CheckTokenValue<KeywordToken, KeywordType>(cur_token, KeywordType::PRINT)) {
        return getPrint(cur_token, end);
    }

    if (CheckTokenType<NameToken>(cur_token)) {
        return getVarAssign(cur_token, end);
    }

    std::cerr << "getStatement: Unexpected token type = " << cur_token->index() << "\n";
    return nullptr;
}

static StatementUnit* getPrint(TokenIt& cur_token, TokenIt end) {
    LOG << "getPrint: start print\n";

    if (cur_token == end) {
        std::cerr << "getPrint: cur_token == end\n";
        return nullptr;
    }

    if (!CheckTokenValue<KeywordToken, KeywordType>(cur_token, KeywordType::PRINT)) {
        std::cerr << "getPrint: not start from print\n";
        return nullptr;
    }
    ++cur_token;

    if (!CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::LEFT_BRACKET)) {
        std::cerr << "getPrint: there not open bracket before expression\n";
        return nullptr;
    }
    ++cur_token;

    ExpressionUnit* expression = getExpresion(cur_token, end);

    if (!CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::RIGHT_BRACKET)) {
        std::cerr << "getPrint: there not close bracket after expression\n";
        return nullptr;
    }
    ++cur_token;

    if (!CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::END_STATEMENT)) {
        std::cerr << "getPrint: there is no a ';' at the end of print statement\n";
        return nullptr;
    }
    ++cur_token;

    return new PrintUnit(expression);
}

static StatementUnit* getIf(TokenIt& cur_token, TokenIt end) {
    LOG << "getIf: start func\n";

    if (cur_token == end) {
        std::cerr << "getIf: cur_token == end\n";
        return nullptr;
    }

    if (!CheckTokenValue<KeywordToken, KeywordType>(cur_token, KeywordType::IF)) {
        std::cerr << "getIf: not start from if\n";
        return nullptr;
    }
    ++cur_token;

    if (!CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::LEFT_BRACKET)) {
        std::cerr << "getIf: there not open bracket before condition\n";
        return nullptr;
    }
    ++cur_token;

    ExpressionUnit* condition = getExpresion(cur_token, end);

    if (!CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::RIGHT_BRACKET)) {
        std::cerr << "getIf: there not close bracket after condition\n";
        return nullptr;
    }
    ++cur_token;

    ScopeUnit* true_branch = getScope(cur_token, end);

    if (!CheckTokenValue<KeywordToken, KeywordType>(cur_token, KeywordType::ELSE)) {
        std::cerr << "getIf: there not else";
        return nullptr;
    }
    ++cur_token;

    ScopeUnit* false_branch = getScope(cur_token, end);

    return new IfUnit(condition, true_branch, false_branch);
}

static StatementUnit* getLoop(TokenIt& cur_token, TokenIt end) {
    LOG << "getLoop: start func\n";

    if (cur_token == end) {
        std::cerr << "getLoop: cur_token == end\n";
        return nullptr;
    }

    if (!CheckTokenValue<KeywordToken, KeywordType>(cur_token, KeywordType::LOOP)) {
        std::cerr << "getLoop: not start from loop\n";
        return nullptr;
    }
    ++cur_token;

    if (!CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::LEFT_BRACKET)) {
        std::cerr << "getLoop: there not open bracket before condition\n";
        return nullptr;
    }
    ++cur_token;

    ExpressionUnit* condition = getExpresion(cur_token, end);

    if (!CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::RIGHT_BRACKET)) {
        std::cerr << "getLoop: there not close bracket after condition\n";
        return nullptr;
    }
    ++cur_token;

    ScopeUnit* body = getScope(cur_token, end);
    if (body == nullptr) {
        std::cerr << "getLoop: scope is nullptr\n";
    }

    return new LoopUnit(condition, body);
}

static StatementUnit* getVarDecl(TokenIt& cur_token, TokenIt end) {
    LOG << "start getVarDecl(\n";
    COMPILER_ASSERT(cur_token != end, "cur_token == end")

    COMPILER_ASSERT((CheckTokenValue<KeywordToken, KeywordType>(cur_token, KeywordType::LET)), 
                    "do not start with 'let'")
    ++cur_token;

    VarAssignUnit* var_assign = static_cast<VarAssignUnit*>(getVarAssign(cur_token, end));
    COMPILER_ASSERT(var_assign != nullptr, "var assign unit is null")

    VarUnit* var = var_assign->var();
    ExpressionUnit* expr = var_assign->expr();
    delete var_assign;

    return new VarDeclUnit(var, expr);
}

static StatementUnit* getVarAssign(TokenIt& cur_token, TokenIt end) {
    LOG << "getVarAssign: start func\n";

    COMPILER_ASSERT(cur_token != end, "cur_token == end");

    VarUnit* variable = getVar(cur_token, end);
    COMPILER_ASSERT(variable != nullptr, "var is null");

    COMPILER_ASSERT((CheckTokenValue<OperatorToken, OperatorType>(cur_token, OperatorType::EQUAL)),
                    "there is not operator = in var assign")

    ++cur_token;

    ExpressionUnit* expression = getExpresion(cur_token, end);
    COMPILER_ASSERT(variable != nullptr, "expression is null");

    COMPILER_ASSERT((CheckTokenValue<SpecialSymbolToken,
                         SpecialSymbolType>(cur_token,
                                           SpecialSymbolType::END_STATEMENT)),
                    "there is no a ';' at the end of var assignment")

    ++cur_token;

    return new VarAssignUnit(variable, expression);
}

static ExpressionUnit* getExpresion(TokenIt& cur_token, TokenIt end) {
    LOG << "getExpresion: start func\n";
    return getAddSub(cur_token, end);
}

static ExpressionUnit* getAddSub(TokenIt& cur_token, TokenIt end) {
    LOG << "getAddSub: start func\n";
    ExpressionUnit* result = getMulDiv(cur_token, end);

    COMPILER_ASSERT(result != nullptr, "left operand is null");

    while (cur_token != end) {
        if (!std::holds_alternative<OperatorToken>(*cur_token) ||
            (std::get<OperatorToken>(*cur_token).oper() != OperatorType::SUB &&
            std::get<OperatorToken>(*cur_token).oper() != OperatorType::ADD)) {
            LOG << "getAddSub: middle token is not operator\n";
            break;
        }

        ExpressionUnit* left_op = result;

        OperatorType oper_type = std::get<OperatorToken>(*cur_token).oper();
        ++cur_token;

        ExpressionUnit* right_op = getMulDiv(cur_token, end);
        COMPILER_ASSERT(right_op != nullptr, "right operand is null");

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

    LOG << "getAddSub: end func\n";
    return result;
}

static ExpressionUnit* getMulDiv(TokenIt& cur_token, TokenIt end) {
    LOG << "getMulDiv: start func\n";
    ExpressionUnit* result = getBrackets(cur_token, end);

    if (result == nullptr) {
        std::cerr << "getMulDiv: left operand is null\n";
        return nullptr;
    }

    while (cur_token != end) {
        if (!std::holds_alternative<OperatorToken>(*cur_token) ||
            (std::get<OperatorToken>(*cur_token).oper() != OperatorType::MUL &&
            std::get<OperatorToken>(*cur_token).oper() != OperatorType::DIV)) {
            LOG << "getMulDiv: middle token is not operator\n";
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

    LOG << "getMulDiv: end func\n";
    return result;
}

static ExpressionUnit* getBrackets(TokenIt& cur_token, TokenIt end) {
    LOG << "getBrackets: start func\n";

    if (!CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::LEFT_BRACKET)) {
        return getUnaryMinus(cur_token, end);
    }
    ++cur_token;

    ExpressionUnit* result = getExpresion(cur_token, end);
    COMPILER_ASSERT(result != nullptr, "middle expresion is null");

    COMPILER_ASSERT((CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::RIGHT_BRACKET)),
                    "is not close bracket");
    ++cur_token;

    return result;
}

static ExpressionUnit* getUnaryMinus(TokenIt& cur_token, TokenIt end) {
    LOG << "getUnaryMinus: start func\n";
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
    LOG << "getObject: start func\n";
    if (std::holds_alternative<NumToken>(*cur_token)) {
        return getNum(cur_token, end);
    }

    if (std::holds_alternative<FloatToken>(*cur_token)) {
        return getFloat(cur_token, end);
    }

    if (std::holds_alternative<NameToken>(*cur_token)) {
        return getVar(cur_token, end);
    }

    if (CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::LEFT_SQUARE_BRACKET)) {
        return getArrayUnit(cur_token, end);
    }

    return nullptr;
}

static VarUnit* getVar(TokenIt& cur_token, TokenIt end) {
    LOG << "getVar: start func\n";
    COMPILER_ASSERT((std::holds_alternative<NameToken>(*cur_token)), "Is not NameToken");

    VarUnit* result = new VarUnit(std::get<NameToken>(*cur_token).name());
    ++cur_token;
    LOG << "getVar: end\n";
    return result;
}

static NumUnit* getNum(TokenIt& cur_token, TokenIt end) {
    LOG << "getNum: start func\n";

    COMPILER_ASSERT(std::holds_alternative<NumToken>(*cur_token), "is not NumToken");

    int num = std::get<NumToken>(*cur_token).num();
    ++cur_token;

    LOG << "getNum: end\n";
    return new NumUnit(num);
}

static FloatUnit* getFloat(TokenIt& cur_token, TokenIt end) {
    LOG << "getNum: start func\n";

    COMPILER_ASSERT(std::holds_alternative<FloatToken>(*cur_token), "is not FloatToken");
    float num = std::get<FloatToken>(*cur_token).num();
    ++cur_token;

    LOG << "getNum: end\n";
    return new FloatUnit(num);
}

static ArrayUnit* getArrayUnit(TokenIt& cur_token, TokenIt end) {
    LOG << "getArratUnit: start func\n";
    
    COMPILER_ASSERT((CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::LEFT_SQUARE_BRACKET)), 
        "left square bracket was not found")
    ++cur_token;

    std::vector<ExpressionUnit*> array_elements;
    if (CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::RIGHT_SQUARE_BRACKET)) {
        ++cur_token;
    }
    else {
        while(true) {
            ExpressionUnit* expression = getExpresion(cur_token, end);
            COMPILER_ASSERT(expression != nullptr, "fail to get one of expressions in array")

            array_elements.push_back(expression);

            if (CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::RIGHT_SQUARE_BRACKET)) {
                ++cur_token;
                break;
            }

            COMPILER_ASSERT((CheckTokenValue<SpecialSymbolToken, SpecialSymbolType>(cur_token, SpecialSymbolType::COMMA)), \
                            "comma expected")
            ++cur_token;
        }
    }

    return new ArrayUnit(std::move(array_elements));
}

void recursiveUnitDelete(GrammarUnit* unit) {
    if (!unit)
        return;

    switch (unit->getType())
    {
    case GrammarUnitType::ADD:
    case GrammarUnitType::MUL:
    case GrammarUnitType::DIV:
    case GrammarUnitType::SUB: {
        BinaryOperUnit* binary_op = reinterpret_cast<BinaryOperUnit*>(unit);
        recursiveUnitDelete(binary_op->left_op_);
        recursiveUnitDelete(binary_op->right_op_);
        break;
    }
    case GrammarUnitType::UNARY_MINUS: {
        UnaryOperUnit* unary_unit = reinterpret_cast<UnaryOperUnit*>(unit);
        recursiveUnitDelete(unary_unit->operand());
        break;
    }
    case GrammarUnitType::VAR_DECL: {
        VarDeclUnit* var_decl_unit = reinterpret_cast<VarDeclUnit*>(unit);
        recursiveUnitDelete(var_decl_unit->var());
        recursiveUnitDelete(var_decl_unit->expr());
        break;
    }
    case GrammarUnitType::VAR_ASSIGN: {
        VarAssignUnit* var_assign_unit = reinterpret_cast<VarAssignUnit*>(unit);
        recursiveUnitDelete(var_assign_unit->var());
        recursiveUnitDelete(var_assign_unit->expr());
        break;
    }
    case GrammarUnitType::SCOPE: {
        ScopeUnit* scope_unit = reinterpret_cast<ScopeUnit*>(unit);
        for (auto statement_unit : *scope_unit) {
            recursiveUnitDelete(statement_unit);
        }
        break;
    }
    case GrammarUnitType::IF: {
        IfUnit* scope_unit = reinterpret_cast<IfUnit*>(unit);

        recursiveUnitDelete(scope_unit->condition());
        recursiveUnitDelete(scope_unit->true_branch());
        recursiveUnitDelete(scope_unit->false_branch());
        break;
    }
    case GrammarUnitType::LOOP: {
        LoopUnit* loop_unit = reinterpret_cast<LoopUnit*>(unit);

        recursiveUnitDelete(loop_unit->condition());
        recursiveUnitDelete(loop_unit->body());
        break;
    }
    case GrammarUnitType::PRINT : {
        PrintUnit* print_unit = reinterpret_cast<PrintUnit*>(unit);

        recursiveUnitDelete(print_unit->expression());
        break;
    }
    case GrammarUnitType::ARRAY: {
        ArrayUnit* array_unit = reinterpret_cast<ArrayUnit*>(unit);
        
        for (ExpressionUnit* element : array_unit->array_elements()) {
            recursiveUnitDelete(element);
        }
    }
    default:
        break;
    }

    delete unit;
}
