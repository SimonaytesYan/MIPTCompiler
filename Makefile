CXX = clang++
DEBUG_FLAGS = -D _DEBUG -ggdb3 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192 -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,leak,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
SANITIZER_FLAGS = -g -fcheck-new -fsized-deallocation -fstack-protector \
				  -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
				  -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr,leak
RELEASE_FLAGS = -O2
LLVM_FLAGS = llvm-config --cxxflags --ldflags --system-libs --libs core
CXXFLAGS = -ISrc/Headers -std=c++2a # $(LLVM_FLAGS) $(SANITIZER_FLAGS)

HEADERS_NAMES = Tokens Keywords SpecialSymbols Operators Grammar Lexer Tokenizer ExecutionPass Logger
HEADERS = $(addsuffix .hpp, $(addprefix Src/Headers/, $(HEADERS_NAMES)))

OBJ = obj
BIN = bin
GRAPHIC_DUMPS = graphic_dumps

DIRS = $(OBJ) $(BIN) $(GRAPHIC_DUMPS)

GREEN_COLOR = \033[0;32m
NO_COLOR = \033[0m

.PHONY: interpreter test clean

#==================================PHONY TARGETS=================================
interpreter: $(BIN)/interpreter

test: test_execution test_lexer_dump test_tokenizer

clean:
	-rm -r $(BIN)
	-rm -r $(OBJ)
	-rm -r $(GRAPHIC_DUMPS)

#==================================TEST RUNNERS=================================
test_execution: $(BIN)/test_execution_object $(BIN)/test_execution_expression $(BIN)/test_execution_scope
	@echo "${GREEN_COLOR}START EXECUTION TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}EXECUTION OBJECTS${NO_COLOR}"
	-@$(BIN)/test_execution_object

	@echo "\n${GREEN_COLOR}EXECUTION EXPRESSION${NO_COLOR}"
	-@$(BIN)/test_execution_expression

	@echo "\n${GREEN_COLOR}EXECUTION SCOPE${NO_COLOR}"
	-@$(BIN)/test_execution_scope

test_lexer_dump: $(BIN)/test_lexer_dump_objects $(BIN)/test_lexer_dump_expr $(BIN)/test_lexer_dump_scope
	@echo "${GREEN_COLOR}START LEXER TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}LEXER SCOPE${NO_COLOR}"
	-@$(BIN)/test_lexer_dump_scope

	@echo "\n${GREEN_COLOR}LEXER EXPRESSION${NO_COLOR}"
	-@$(BIN)/test_lexer_dump_expr

	@echo "\n${GREEN_COLOR}LEXER OBJECTS${NO_COLOR}"
	-@$(BIN)/test_lexer_dump_objects

test_tokenizer: $(BIN)/test_tokenizer_num $(BIN)/test_tokenizer_name $(BIN)/test_tokenizer_keywords $(BIN)/test_tokenizer_operators $(BIN)/test_tokenizer_spec_symbols
	@echo "${GREEN_COLOR}START TOKENIZER TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}TOKENIZER NUM${NO_COLOR}"
	@$(BIN)/test_tokenizer_num

	@echo "\n${GREEN_COLOR}TOKENIZER NAME${NO_COLOR}"
	@$(BIN)/test_tokenizer_name

	@echo "\n${GREEN_COLOR}TOKENIZER KEYWORD${NO_COLOR}"
	@$(BIN)/test_tokenizer_keywords

	@echo "\n${GREEN_COLOR}TOKENIZER OPERATOR${NO_COLOR}"
	@$(BIN)/test_tokenizer_operators

	@echo "\n${GREEN_COLOR}TOKENIZER SPECIAL SYMBOL${NO_COLOR}"
	@$(BIN)/test_tokenizer_spec_symbols


#---------------------------EXECUTION PASS TESTS--------------------------------
$(BIN)/test_execution_object: Tests/ExecutionPass/Objects.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/ExecutionPass.o $(OBJ)/Grammar.o | $(DIRS) # Tests/ExecutionPass/RunOneTest.hpp
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_execution_expression: Tests/ExecutionPass/Expression.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/ExecutionPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_execution_scope: Tests/ExecutionPass/Scope.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/ExecutionPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

#------------------------------LEXER TESTS--------------------------------------
$(BIN)/test_lexer_dump_objects: Tests/Lexer/Objects.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_lexer_dump_expr: Tests/Lexer/Expr.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_lexer_dump_scope: Tests/Lexer/Scope.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

#----------------------------TOKENIZER TESTS------------------------------------
$(BIN)/test_tokenizer_spec_symbols: Tests/Tokenizer/SpecialSymbols.cpp $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_tokenizer_operators: Tests/Tokenizer/Operators.cpp $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_tokenizer_keywords: Tests/Tokenizer/Keywords.cpp $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_tokenizer_num: Tests/Tokenizer/Num.cpp $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/test_tokenizer_name: Tests/Tokenizer/Name.cpp $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

#====================================SOURCE=====================================
$(BIN)/interpreter: Src/Interpreter.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/ExecutionPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ)/Tokenizer.o: Src/Cpp/Tokenizer.cpp | $(DIRS) #$(HEADERS)
	$(CXX) -c $(CXXFLAGS) $^ -o $@

$(OBJ)/Lexer.o: Src/Cpp/Lexer.cpp | $(DIRS) #$(HEADERS)
	$(CXX) -c $(CXXFLAGS) $^ -o $@

$(OBJ)/GraphicDumpPass.o: Src/Cpp/GraphicDumpPass.cpp $(OBJ)/Grammar.o | $(DIRS) # $(HEADERS)
	$(CXX) -c $(CXXFLAGS) $^ -o $@

$(OBJ)/ExecutionPass.o: Src/Cpp/ExecutionPass.cpp | $(DIRS) # $(HEADERS)
	$(CXX) -c $(CXXFLAGS) $^ -o $@

$(OBJ)/Grammar.o: Src/Cpp/Grammar.cpp | $(DIRS) # $(HEADERS)
	$(CXX) -c $(CXXFLAGS) $^ -o $@

#=================================DIRECTORIES===================================

$(OBJ):
	mkdir $(OBJ)

$(GRAPHIC_DUMPS):
	mkdir $(GRAPHIC_DUMPS)

$(BIN):
	mkdir $(BIN)
