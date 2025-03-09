#pragma once

enum class OperatorType {
    ADD,
    SUB,
    MUL,
    DIV,
    OPERATOR_NUM
};

struct Operator {
    std::string name;
    OperatorType type;
};

const Operator kOperators[] = {
    {"+", OperatorType::ADD},
    {"-", OperatorType::SUB},
    {"*", OperatorType::MUL},
    {"/", OperatorType::DIV}
};
