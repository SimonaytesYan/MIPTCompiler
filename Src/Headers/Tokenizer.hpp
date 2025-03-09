#pragma once

#include "Tokens.hpp"

#include <istream>
#include <vector>

std::vector<Token> tokenize(std::istream& in);