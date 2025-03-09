#include "../Headers/Tokens.hpp"
#include "../Headers/Tokenizer.hpp"

#include <iostream>

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

std::vector<Token> tokenize(std::istream& in) {
    std::vector<Token> tokens;

    while (in.peek() != EOF)
    {
        skipSpaces(in);

        if (getNum(in, tokens))
            continue;
        if (getKeyword(in, tokens))
            continue;
        if (getOperator(in, tokens))
            continue;
        if (getSpecialSymbol(in, tokens))
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
    if ('0' <= first_sym && first_sym <= '9' || first_sym == '-') {
        in >> num;
        std::cout << "num = " << num << "\n";
        tokens.emplace_back(NumToken(num));
        return true;
    }

    return false;
}

static bool getKeyword(std::istream& in, std::vector<Token>& tokens) {

    std::string potential_keyword = "";
    in >> potential_keyword;

    for (auto& keyword : kKeywords) {
        if (potential_keyword == keyword.name) {
            tokens.emplace_back(KeywordToken(keyword.type));
            return true;
        }
    }

    const size_t get_symbol_num = potential_keyword.size();
    for(size_t i = 0; i < get_symbol_num; i++)
        in.unget();

    return false;
}

static bool getOperator(std::istream& in, std::vector<Token>& tokens) {
    return false;
}

static bool getSpecialSymbol(std::istream& in, std::vector<Token>& tokens) {
    return false;
}

static bool getName(std::istream& in, std::vector<Token>& tokens) {
    std::string name = "";

    in >> name;

    if (in.fail()) {
        std::cout << "fail Name\n";
        return false;
    }

    tokens.emplace_back(NameToken(std::move(name)));
    std::cout << "name = " << name << "\n";

    return true;
}

static void skipSpaces(std::istream& in) {
    in >> std::skipws;
}
