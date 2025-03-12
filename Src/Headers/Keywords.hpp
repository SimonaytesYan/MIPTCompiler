#pragma once

#include <cstddef>
#include <string>

enum class KeywordType {
    LET,
    IF,
    ELSE,
    OUT,
    LOOP,
    KEYWORD_NUM
};

struct Keyword {
    std::string name;
    KeywordType type;
};

const Keyword kKeywords[] = {
    {"let",  KeywordType::LET},
    {"if",   KeywordType::IF},
    {"else", KeywordType::ELSE},
    {"out",  KeywordType::OUT},
    {"loop",  KeywordType::LOOP}
};
