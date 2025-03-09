#pragma once

enum class Operator {
    ADD,
    SUB,
    MUL,
    DIV,
    OPERATOR_NUM
};

static const char* Operators[(size_t)Operator::OPERATOR_NUM] = {
    "+",
    "-",
    "*",
    "/"};
