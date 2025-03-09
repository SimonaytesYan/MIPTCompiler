#pragma once

#include <cstddef>
#include <string>

enum class KeywordType {
    LET,
    IF,
    ELSE,
    OUT,
    KEYWORD_NUM
};

struct Keyword {
    std::string name;
    KeywordType type;
};

const Keyword kKeywords[(size_t)KeywordType::KEYWORD_NUM] = {
    {"let", KeywordType::LET},
    {"if", KeywordType::IF},
    {"else", KeywordType::ELSE},
    {"out", KeywordType::OUT}
};
