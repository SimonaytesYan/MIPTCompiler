CXX = g++
DEBUG_FLAGS = -D _DEBUG -ggdb3 -std=c++2a -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192 -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,leak,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
SANITIZER_FLAGS = -g -fcheck-new -fsized-deallocation -fstack-protector \
				  -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
				  -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr,leak
RELEASE_FLAGS = -O2
CFLAGS = $(SANITIZER_FLAGS) -ISrc/Headers
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

test: test_execution test_lexer_dump tests_tokenizer

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

tests_tokenizer: $(BIN)/test_tokenizer_num $(BIN)/test_tokenizer_name $(BIN)/test_tokenizer_keywords $(BIN)/test_tokenizer_operators $(BIN)/test_tokenizer_spec_symbols
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
$(BIN)/test_execution_object: Tests/ExecutionPass/Objects.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/ExecutionPass.o $(OBJ)/Grammar.o Tests/ExecutionPass/RunOneTest.hpp | $(DIRS)
	$(CXX) $(CFLAGS) Tests/ExecutionPass/Objects.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/Grammar.o	\
					$(OBJ)/ExecutionPass.o -o $(BIN)/test_execution_object

$(BIN)/test_execution_expression: Tests/ExecutionPass/Expression.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/ExecutionPass.o $(OBJ)/Grammar.o Tests/ExecutionPass/RunOneTest.hpp | $(DIRS)
	$(CXX) $(CFLAGS) Tests/ExecutionPass/Expression.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/Grammar.o	\
					$(OBJ)/ExecutionPass.o -o $(BIN)/test_execution_expression

$(BIN)/test_execution_scope: Tests/ExecutionPass/Scope.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/ExecutionPass.o $(OBJ)/Grammar.o Tests/ExecutionPass/RunOneTest.hpp| $(DIRS)
	$(CXX) $(CFLAGS) Tests/ExecutionPass/Scope.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/Grammar.o	\
					$(OBJ)/ExecutionPass.o -o $(BIN)/test_execution_scope

#------------------------------LEXER TESTS--------------------------------------
$(BIN)/test_lexer_dump_objects: Tests/Lexer/Objects.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CFLAGS) Tests/Lexer/Objects.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/Grammar.o	\
					$(OBJ)/GraphicDumpPass.o -o $(BIN)/test_lexer_dump_objects

$(BIN)/test_lexer_dump_expr: Tests/Lexer/Expr.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CFLAGS) Tests/Lexer/Expr.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/Grammar.o	\
					$(OBJ)/GraphicDumpPass.o -o $(BIN)/test_lexer_dump_expr

$(BIN)/test_lexer_dump_scope: Tests/Lexer/Scope.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CFLAGS) Tests/Lexer/Scope.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/Grammar.o	\
					$(OBJ)/GraphicDumpPass.o -o $(BIN)/test_lexer_dump_scope

#----------------------------TOKENIZER TESTS------------------------------------
$(BIN)/test_tokenizer_spec_symbols: $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CFLAGS) Tests/Tokenizer/SpecialSymbols.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_spec_symbols

$(BIN)/test_tokenizer_operators: $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CFLAGS) Tests/Tokenizer/Operators.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_operators

$(BIN)/test_tokenizer_keywords: $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CFLAGS) Tests/Tokenizer/Keywords.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_keywords

$(BIN)/test_tokenizer_num: $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CFLAGS) Tests/Tokenizer/Num.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_num

$(BIN)/test_tokenizer_name: $(OBJ)/Tokenizer.o | $(DIRS)
	$(CXX) $(CFLAGS) Tests/Tokenizer/Name.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_name


#====================================SOURCE=====================================
$(BIN)/interpreter: Src/Interpreter.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/ExecutionPass.o $(OBJ)/Grammar.o | $(DIRS)
	$(CXX) $(CFLAGS) Src/Interpreter.cpp \
					$(OBJ)/Tokenizer.o 	 \
					$(OBJ)/Lexer.o		 \
					$(OBJ)/Grammar.o	 \
					$(OBJ)/ExecutionPass.o -o $(BIN)/Interpreter

$(OBJ)/Tokenizer.o: Src/Cpp/Tokenizer.cpp $(HEADERS) | $(DIRS)
	$(CXX) -c $(CFLAGS) Src/Cpp/Tokenizer.cpp -o $(OBJ)/Tokenizer.o

$(OBJ)/Lexer.o: Src/Cpp/Lexer.cpp $(HEADERS) | $(DIRS)
	$(CXX) -c $(CFLAGS) Src/Cpp/Lexer.cpp -o $(OBJ)/Lexer.o

$(OBJ)/GraphicDumpPass.o: Src/Cpp/GraphicDumpPass.cpp $(OBJ)/Grammar.o $(HEADERS) | $(DIRS)
	$(CXX) -c $(CFLAGS) Src/Cpp/GraphicDumpPass.cpp \
					   $(OBJ)/Grammar.o -o $(OBJ)/GraphicDumpPass.o

$(OBJ)/ExecutionPass.o: Src/Cpp/ExecutionPass.cpp $(HEADERS) | $(DIRS)
	$(CXX) -c $(CFLAGS) Src/Cpp/ExecutionPass.cpp -o $(OBJ)/ExecutionPass.o

$(OBJ)/Grammar.o:  Src/Cpp/Grammar.cpp $(HEADERS) | $(DIRS)
	$(CXX) -c $(CFLAGS) Src/Cpp/Grammar.cpp -o $(OBJ)/Grammar.o

#=================================DIRECTORIES===================================

$(OBJ):
	mkdir $(OBJ)

$(GRAPHIC_DUMPS):
	mkdir $(GRAPHIC_DUMPS)

$(BIN):
	mkdir $(BIN)
