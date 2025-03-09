#pragma once

enum class SpecialSymbol {
    LEFT_BRACKET,
    RIGHT_BRACKET,
    START_SCOPE,
    END_SCOPE,
    SPECIAL_SYMBOLS_NUM
};

static const char* SpecialSymbols[(size_t)SpecialSymbol::SPECIAL_SYMBOLS_NUM] = {
    "(",
    ")",
    "{",
    "}"};