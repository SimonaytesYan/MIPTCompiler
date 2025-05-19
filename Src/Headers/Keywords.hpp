#pragma once

#include <cstddef>
#include <string>

enum class KeywordType {
    LET,
    IF,
    ELSE,
    PRINT,
    LOOP,
    ARRAY,
    INT,
    FLOAT,
    STRING,
    KEYWORD_NUM
};

struct Keyword {
    std::string name;
    KeywordType type;
};

const Keyword kKeywords[] = {
    {"let",   KeywordType::LET},
    {"if",    KeywordType::IF},
    {"else",  KeywordType::ELSE},
    {"print", KeywordType::PRINT},
    {"loop",  KeywordType::LOOP},
    {"arr",   KeywordType::ARRAY},
    {"int",   KeywordType::INT},
    {"flt",   KeywordType::FLOAT},
    {"str",   KeywordType::STRING}
};
