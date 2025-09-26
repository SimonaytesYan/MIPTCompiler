#pragma once

#include <iostream>
#include <sstream>
#include <string_view>

#include "Keywords.hpp"
#include "Operators.hpp"
#include "SpecialSymbols.hpp"
#include "Tokens.hpp"
#include "Tokenizer.hpp"


void printTokens(const std::vector<Token>& tokens) {
    std::cout << "{";
    for (const Token& token : tokens) {
        if (std::holds_alternative<NumToken>(token)) {
            int val = std::get<NumToken>(token).num();
            std::cout << "num `" << val << "`";
        }
        if (std::holds_alternative<NameToken>(token)) {
            const std::string& val = std::get<NameToken>(token).name();
            std::cout << "name `" << val << "`";
        }
        if (std::holds_alternative<KeywordToken>(token)) {
            KeywordType val = std::get<KeywordToken>(token).keyword();
            std::cout << "keyword `" << kKeywords[(size_t)val].name << "`";
        }
        if (std::holds_alternative<SpecialSymbolToken>(token)) {
            SpecialSymbolType val = std::get<SpecialSymbolToken>(token).specSym();
            std::cout << "spec `" << kSpecialSymbols[(size_t)val].name << "`";
        }
        if (std::holds_alternative<OperatorToken>(token)) {
            OperatorType val = std::get<OperatorToken>(token).oper();
            std::cout << "oper `" << kOperators[(size_t)val].name << "`";
        }
        if (std::holds_alternative<FloatToken>(token)) {
            float val = std::get<FloatToken>(token).num();
            std::cout << "float `" << val << "`";
        }
        std::cout << ", ";
    }
    std::cout << "}\n";
}

void runOneTest(std::string test, std::string_view name) {
    std::istringstream in(test);
    std::cout << "TEST `" << name << "`:\n";
    std::cout << "origin = `" << test << "`\n";

    auto res = tokenize(in);
    std::cout << "tokens = ";
    printTokens(res);
    std::cout << "\n";
}