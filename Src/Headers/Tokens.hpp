#pragma once

#include "Keywords.hpp"
#include "SpecialSymbols.hpp"
#include "Operators.hpp"

#include <cstddef>
#include <string>
#include <variant>

struct TokenInfo {
    size_t line_num;
    size_t sym_num;
};

class NumToken {
  public:
    NumToken(int num) :
      num_(num) { }

  private:
    int num_;
    TokenInfo info_;
};

class NameToken {
  public:
    NameToken(std::string&& str) :
      name(str) { }

  private:
    std::string name;
    TokenInfo info_;
};

class KeywordToken {
  public:
    KeywordToken(KeywordType keyword) :
      keyword_(keyword) { }

  private:
    KeywordType keyword_;
    TokenInfo info_;
};

class SymbolToken {

  private:
    SpecialSymbol sym_;
    TokenInfo info_;
};

class OperatorToken {
  public:
    OperatorToken(OperatorType oper) :
      operator_(oper) { }

  private:
    OperatorType operator_;
    TokenInfo info_;
};

using Token = std::variant<NumToken, NameToken, KeywordToken,
                           SymbolToken, OperatorToken>;
