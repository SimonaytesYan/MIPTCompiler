#pragma once

#include "Tokens.hpp"
#include "Grammar.hpp"

#include <vector>

GrammarUnit* parse(const std::vector<Token>& tokens);

void recursiveUnitDelete(GrammarUnit* unit);
