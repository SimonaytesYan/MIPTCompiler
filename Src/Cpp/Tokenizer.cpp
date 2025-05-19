#include "Keywords.hpp"
#include "Logger.hpp"
#include "Operators.hpp"
#include "SpecialSymbols.hpp"
#include "Tokenizer.hpp"
#include "Tokens.hpp"

#include <algorithm>
#include <cstdio>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

// return true, if number get correctly
// return false otherwise
static bool getNum(std::istream& in, std::vector<Token>& tokens);

// return true, if keyword(see Keyword.hpp) get correctly
// return false otherwise
static bool getKeyword(std::istream& in, std::vector<Token>& tokens);

// return true, if operator(see Operators.hpp) get correctly
// return false otherwise
static bool getOperator(std::istream& in, std::vector<Token>& tokens);

// return true, if special symbol(see SpecialSymbols.hpp) get correctly
// return false otherwise
static bool getSpecialSymbol(std::istream& in, std::vector<Token>& tokens);

// return true, if name get correctly
// return false otherwise
static bool getName(std::istream& in, std::vector<Token>& tokens);

// skip all space symbols aka spaces, tabs, \n, \r, etc.
static void skipSpaces(std::istream& in);

// run in.unet() n times
static void ungetSymbols(std::istream& in, size_t n);

std::vector<Token> tokenize(std::istream& in) {
    std::vector<Token> tokens;

    while (in.peek() != EOF)
    {
        skipSpaces(in);

        if (getKeyword(in, tokens))
            continue;
        if (getOperator(in, tokens))
            continue;
        if (getSpecialSymbol(in, tokens))
            continue;
        if (getNum(in, tokens))
            continue;
        if (getName(in, tokens))
            continue;

        std::cerr << "ERROR unknown token\n";
    }

    return tokens;
}

static bool getNum(std::istream& in, std::vector<Token>& tokens) {
    int num = 0;

    int first_sym = in.get();
    in.unget();

    // Do not wait '-', will work with it during syntax analysis
    if (!('0' <= first_sym && first_sym <= '9')) {
        return false;
    }

    in >> num;
    log << "num = " << num << "\n";

    int sym = in.get();
    if (sym == '.') {
        int fractional_part = 0;

        first_sym = in.get();
        in.unget();
        if (!('0' <= first_sym && first_sym <= '9')) {
            return false;
        }
        in >> fractional_part;
    }
    else {
        in.unget();
    }

    tokens.emplace_back(NumToken(num));
    return true;
}

static bool getKeyword(std::istream& in, std::vector<Token>& tokens) {

    std::string potential_keyword = "";
    in >> potential_keyword;

    for (const auto& keyword : kKeywords) {
        // check that keyword is a prefix of potential_keyword
        auto res = std::mismatch(keyword.name.begin(),
                                keyword.name.end(),
                                potential_keyword.begin());
        if (res.first == keyword.name.end()) {
            tokens.emplace_back(KeywordToken(keyword.type));

            log << "keyword = " << keyword.name << "\n";

            ungetSymbols(in, potential_keyword.size() - keyword.name.size());
            return true;
        }
    }

    ungetSymbols(in, potential_keyword.size());
    return false;
}

static bool getOperator(std::istream& in, std::vector<Token>& tokens) {
    std::string potential_operator = "";
    in >> potential_operator;

    for (const Operator& oper : kOperators) {
        // check that oper is a prefix of potential_operator
        auto res = std::mismatch(oper.name.begin(),
                                oper.name.end(),
                                potential_operator.begin());
        if (res.first == oper.name.end()) {
            tokens.emplace_back(OperatorToken(oper.type));

            log << "operator = " << oper.name << "\n";

            ungetSymbols(in, potential_operator.size() - oper.name.size());
            return true;
        }
    }

    ungetSymbols(in, potential_operator.size());
    return false;
}

static bool getString(std::istream& in, std::vector<Token>& tokens, SpecialSymbol start_symbol) {
    std::string str = "";
    while (in.peek() != EOF) {
        char symbol = in.get();
        if (symbol == start_symbol.name[0]) {
            tokens.push_back(NameToken(std::move(str)));
            tokens.push_back(SpecialSymbolToken(start_symbol.type));
            return true;
        }
        str.push_back(symbol);
    }

    return false;
}

static bool getSpecialSymbol(std::istream& in, std::vector<Token>& tokens) {
    std::string potential_spec_sym = "";
    in >> potential_spec_sym;

    for (const SpecialSymbol& spec_sym : kSpecialSymbols) {
        // check that spec_sym is a prefix of potential_spec_sym
        auto res = std::mismatch(spec_sym.name.begin(),
                                 spec_sym.name.end(),
                                 potential_spec_sym.begin());
        if (res.first == spec_sym.name.end()) {
            tokens.emplace_back(SpecialSymbolToken(spec_sym.type));

            log << "spec_symbol = " << spec_sym.name << "\n";

            ungetSymbols(in, potential_spec_sym.size() - spec_sym.name.size());

            if (spec_sym.type == SpecialSymbolType::DOUBLE_QUOTES ||
                spec_sym.type == SpecialSymbolType::SINGLE_QUOTES)
                return getString(in, tokens, spec_sym);

            return true;
        }
    }

    ungetSymbols(in, potential_spec_sym.size());
    return false;
}

static bool getName(std::istream& in, std::vector<Token>& tokens) {
    std::string name = "";
    int next_sym = 0;

    next_sym = in.get();

    if (isalpha(next_sym) || next_sym == '_') {
        do {
            name += next_sym;
            next_sym = in.get();
        }
        while (isalnum(next_sym) || next_sym == '_');

        log << "name = " << name << "\n";
        tokens.emplace_back(NameToken(std::move(name)));

        in.unget();
        return true;
    }

    in.unget();
    return true;
}

static void skipSpaces(std::istream& in) {
    in >> std::skipws;
}

static void ungetSymbols(std::istream& in, size_t n) {
    for (size_t i = 0; i < n; i++)
        in.unget();
}