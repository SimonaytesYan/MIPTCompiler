#pragma once

#include "Tokens.hpp"

#include <vector>

ASTTree parse(const std::vector<Token>& tokens);
