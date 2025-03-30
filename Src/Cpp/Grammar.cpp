#include "Grammar.hpp"

bool isGrammarUnitOperator(const GrammarUnit* unit) {
  return dynamic_cast<const OperatorUnit*>(unit) != nullptr;
}

bool isGrammarUnitBinaryOperator(const GrammarUnit* unit) {
  return dynamic_cast<const BinaryOperUnit*>(unit) != nullptr;
}

bool isGrammarUnitUnaryOperator(const GrammarUnit* unit) {
  return dynamic_cast<const UnaryOperUnit*>(unit) != nullptr;
}

bool isGrammarUnitObject(const GrammarUnit* unit) {
  return dynamic_cast<const ObjectUnit*>(unit) != nullptr;
}

bool isGrammarUnitExpression(const GrammarUnit* unit) {
  return dynamic_cast<const ExpressionUnit*>(unit) != nullptr;
}

bool isGrammarUnitStatement(const GrammarUnit* unit) {
  return dynamic_cast<const StatementUnit*>(unit) != nullptr;
}
