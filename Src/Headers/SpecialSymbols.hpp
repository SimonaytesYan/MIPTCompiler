#pragma once

#include <string>

enum class SpecialSymbolType {
    LEFT_BRACKET,
    RIGHT_BRACKET,
    START_SCOPE,
    END_SCOPE,
    END_STATEMENT,
    SINGLE_QUOTES,
    DOUBLE_QUOTES,
    LEFT_SQUARE_BRACKET,
    RIGHT_SQUARE_BRACKET,
    COMMA,
    SPECIAL_SYMBOLS_NUM
};

struct SpecialSymbol {
    std::string name;
    SpecialSymbolType type;
};

const SpecialSymbol kSpecialSymbols[] = {
    {"(", SpecialSymbolType::LEFT_BRACKET},
    {")", SpecialSymbolType::RIGHT_BRACKET},
    {"{", SpecialSymbolType::START_SCOPE},
    {"}", SpecialSymbolType::END_SCOPE},
    {";", SpecialSymbolType::END_STATEMENT},
    {"'", SpecialSymbolType::SINGLE_QUOTES},
    {"\"", SpecialSymbolType::DOUBLE_QUOTES},
    {"[", SpecialSymbolType::LEFT_SQUARE_BRACKET},
    {"]", SpecialSymbolType::RIGHT_SQUARE_BRACKET},
    {",", SpecialSymbolType::COMMA}
    };
