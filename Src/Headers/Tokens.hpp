#pragma once

#include "Keywords.hpp"
#include "SpecialSymbols.hpp"
#include "Operators.hpp"

#include <cstddef>
#include <string>
#include <variant>
#include <vector>

struct TokenInfo {
    size_t line_num;
    size_t sym_num;
};

class NumToken {
  public:
    NumToken(int num) :
      num_(num) { }

  int num() const {
    return num_;
  }

  private:
    int num_;
    TokenInfo info_;
};

class NameToken {
  public:
    NameToken(std::string&& name) :
      name_(name) { }

    const std::string& name() const {
      return name_;
    }

  private:
    std::string name_;
    TokenInfo info_;
};

class KeywordToken {
  public:
    KeywordToken(KeywordType keyword) :
      keyword_(keyword) { }

    KeywordType keyword() const {
      return keyword_;
    }
  private:
    KeywordType keyword_;
    TokenInfo info_;
};

class SpecialSymbolToken {
  public:
    SpecialSymbolToken(SpecialSymbolType sym_type) :
      sym_type_(sym_type) { }

    SpecialSymbolType specSym() const {
      return sym_type_;
    }

  private:
    SpecialSymbolType sym_type_;
    TokenInfo info_;
};

class OperatorToken {
  public:
    OperatorToken(OperatorType oper) :
      operator_(oper) { }

   OperatorType oper() const {
    return operator_;
   }

  private:
    OperatorType operator_;
    TokenInfo info_;
};

using Token = std::variant<NumToken, NameToken, KeywordToken,
                           SpecialSymbolToken, OperatorToken>;

using TokenIt = std::vector<Token>::const_iterator;

template<class TokenType, class TokenValueType>
static TokenValueType GetTokenVal(TokenIt token);


template<>
const std::string& GetTokenVal<NameToken, const std::string&>(TokenIt token) {
    return std::get<NameToken>(*token).name();
}

template<>
int GetTokenVal<NumToken, int>(const TokenIt token) {
    return std::get<NumToken>(*token).num();
}

template<>
KeywordType GetTokenVal<KeywordToken, KeywordType>(TokenIt token) {
    return std::get<KeywordToken>(*token).keyword();
}

template<>
SpecialSymbolType GetTokenVal<SpecialSymbolToken, SpecialSymbolType>(TokenIt token) {
    return std::get<SpecialSymbolToken>(*token).specSym();
}

template<>
OperatorType GetTokenVal<OperatorToken, OperatorType>(TokenIt token) {
    return std::get<OperatorToken>(*token).oper();
}

