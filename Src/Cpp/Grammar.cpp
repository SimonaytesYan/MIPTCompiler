#include "../Headers/Grammar.hpp"

bool isGrammarUnitOperator(GrammarUnit* unit) {
  return dynamic_cast<OperatorUnit*>(unit) != nullptr;
}

bool isGrammarUnitBinaryOperator(GrammarUnit* unit) {
  return dynamic_cast<BinaryOperUnit*>(unit) != nullptr;
}

bool isGrammarUnitUnaryOperator(GrammarUnit* unit) {
  return dynamic_cast<UnaryOperUnit*>(unit) != nullptr;
}

bool isGrammarUnitObject(GrammarUnit* unit) {
  return dynamic_cast<ObjectUnit*>(unit) != nullptr;
}

bool isGrammarUnitExpression(GrammarUnit* unit) {
  return dynamic_cast<ExpressionUnit*>(unit) != nullptr;
}

bool isGrammarUnitStatement(GrammarUnit* unit) {
  return dynamic_cast<StatementUnit*>(unit) != nullptr;
}
