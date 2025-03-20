#pragma once

#include <string>

enum class SpecialSymbolType {
    LEFT_BRACKET,
    RIGHT_BRACKET,
    START_SCOPE,
    END_SCOPE,
    END_STATEMENT,
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
    {";", SpecialSymbolType::END_STATEMENT}};
