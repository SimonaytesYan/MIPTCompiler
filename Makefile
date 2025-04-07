#==================================VARIABLES====================================

#------------------------------COMPILER AND FLAGS-------------------------------
ifeq ($(origin CXX), default)
	CXX = clang++
endif

DEBUG_FLAGS = -D _DEBUG -ggdb3 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192 -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,leak,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
SANITIZER_FLAGS = -g -fcheck-new -fsized-deallocation -fstack-protector \
				  -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
				  -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr,leak
RELEASE_FLAGS = -O2 -std=c++2a
LLVM_FLAGS = llvm-config --cxxflags --ldflags --system-libs --libs core
COMMON_INC = -ISrc/Headers

CXXFLAGS = $(RELEASE_FLAGS)
override CXXFLAGS += $(COMMON_INC) # $(LLVM_FLAGS) $(SANITIZER_FLAGS)

#--------------------------------FILES AND DIRS---------------------------------

#-----------------HEADERS----------------
HEADERS_NAMES = Tokens Keywords SpecialSymbols Operators Grammar Lexer Tokenizer ExecutionPass Logger
HEADERS = $(addsuffix .hpp, $(addprefix Src/Headers/, $(HEADERS_NAMES)))

#--------------BASIC_SOURCES-------------
BASIC_SOURCE_NAMES = ExecutionPass GraphicDumpPass Grammar Lexer Tokenizer VariableList
BASIC_CPPS = $(addsuffix .cpp, $(addprefix $(SRC), $(BASIC_SOURCE_NAMES)))

BASIC_OBJS = $(addsuffix .o, $(addprefix $(OBJ)/, $(BASIC_SOURCE_NAMES)))
BASIC_DEPS = $(BASIC_OBJS:.o=.d)

#-------------TOKENIZER TESTS------------
TOKEN_TEST_DIR = Tests/Tokenizer
TOKEN_TEST_BIN_DIR = $(BIN)/$(TOKEN_TEST_DIR)
TOKEN_TEST_SOURCE_NAMES = SpecialSymbols Operators Keywords Num Name

TOKEN_TEST_CPPS = $(addsuffix .cpp, $(addprefix $(TOKEN_TEST_DIR), $(TOKEN_TEST_SOURCE_NAMES)))
TOKEN_TEST_BIN = $(addprefix $(TOKEN_TEST_BIN_DIR)/, $(TOKEN_TEST_SOURCE_NAMES))

#-------------LEXER TESTS------------
LEXER_TEST_DIR = Tests/Lexer
LEXER_TEST_BIN_DIR = $(BIN)/$(LEXER_TEST_DIR)
LEXER_TEST_SOURCE_NAMES = Expr Objects Scope

LEXER_TEST_CPPS = $(addsuffix .cpp, $(addprefix $(LEXER_TEST_DIR), $(LEXER_TEST_SOURCE_NAMES)))
LEXER_TEST_BIN = $(addprefix $(LEXER_TEST_BIN_DIR)/, $(LEXER_TEST_SOURCE_NAMES))

#-------------EXECUTION PASS TESTS------------
EXE_PASS_TEST_DIR = Tests/ExecutionPass
EXE_PASS_TEST_BIN_DIR = $(BIN)/$(EXE_PASS_TEST_DIR)
EXE_PASS_TEST_SOURCE_NAMES = Expression Objects Scope

EXE_PASS_TEST_CPPS = $(addsuffix .cpp, $(addprefix $(EXE_PASS_TEST_DIR), $(EXE_PASS_TEST_SOURCE_NAMES)))
EXE_PASS_TEST_BIN = $(addprefix $(EXE_PASS_TEST_BIN_DIR)/, $(EXE_PASS_TEST_SOURCE_NAMES))

#-----------------DIRS----------------
OBJ = obj
SRC = Src/Cpp
BIN = bin
GRAPHIC_DUMPS = graphic_dumps

DIRS = $(OBJ) $(BIN) $(GRAPHIC_DUMPS)

#----------------------------------------OTHER----------------------------------
NO_DEPS=clean

GREEN_COLOR = "\033[0;32m"
NO_COLOR = "\033[0m"

.PHONY: interpreter test clean test_execution test_lexer_dump test_tokenizer
#==================================PHONY TARGETS=================================
interpreter: $(BIN)/interpreter

test: test_execution test_lexer_dump test_tokenizer

clean:
	-rm -r $(BIN)
	-rm -r $(OBJ)
	-rm -r $(GRAPHIC_DUMPS)

#==================================TEST RUNNERS=================================
test_execution: $(EXE_PASS_TEST_BIN)
	@echo "${GREEN_COLOR}START EXECUTION TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}EXECUTION OBJECTS${NO_COLOR}"
	-@$(EXE_PASS_TEST_BIN_DIR)/Objects

	@echo "\n${GREEN_COLOR}EXECUTION EXPRESSION${NO_COLOR}"
	-@$(EXE_PASS_TEST_BIN_DIR)/Expression

	@echo "\n${GREEN_COLOR}EXECUTION SCOPE${NO_COLOR}"
	-@$(EXE_PASS_TEST_BIN_DIR)/Scope

test_lexer_dump: $(LEXER_TEST_BIN)
	@echo "${GREEN_COLOR}START LEXER TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}LEXER SCOPE${NO_COLOR}"
	-@$(LEXER_TEST_BIN_DIR)/Scope

	@echo "\n${GREEN_COLOR}LEXER EXPRESSION${NO_COLOR}"
	-@$(LEXER_TEST_BIN_DIR)/Expr

	@echo "\n${GREEN_COLOR}LEXER OBJECTS${NO_COLOR}"
	-@$(LEXER_TEST_BIN_DIR)/Objects

test_tokenizer: $(TOKEN_TEST_BIN)
	@echo "${GREEN_COLOR}START TOKENIZER TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}TOKENIZER NUM${NO_COLOR}"
	@$(TOKEN_TEST_BIN_DIR)/Num

	@echo "\n${GREEN_COLOR}TOKENIZER NAME${NO_COLOR}"
	@$(TOKEN_TEST_BIN_DIR)/Name

	@echo "\n${GREEN_COLOR}TOKENIZER KEYWORD${NO_COLOR}"
	@$(TOKEN_TEST_BIN_DIR)/Keywords

	@echo "\n${GREEN_COLOR}TOKENIZER OPERATOR${NO_COLOR}"
	@$(TOKEN_TEST_BIN_DIR)/Operators

	@echo "\n${GREEN_COLOR}TOKENIZER SPECIAL SYMBOL${NO_COLOR}"
	@$(TOKEN_TEST_BIN_DIR)/SpecialSymbols

#---------------------------EXECUTION PASS TESTS--------------------------------
$(EXE_PASS_TEST_BIN) : $(EXE_PASS_TEST_BIN_DIR)/% : $(EXE_PASS_TEST_DIR)/%.cpp $(BASIC_OBJS) | $(DIRS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

#------------------------------LEXER TESTS--------------------------------------
$(LEXER_TEST_BIN) : $(LEXER_TEST_BIN_DIR)/% : $(LEXER_TEST_DIR)/%.cpp $(BASIC_OBJS) | $(DIRS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

#----------------------------TOKENIZER TESTS------------------------------------
$(TOKEN_TEST_BIN) : $(TOKEN_TEST_BIN_DIR)/% : $(TOKEN_TEST_DIR)/%.cpp $(OBJ)/Tokenizer.o | $(DIRS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

#====================================SOURCE=====================================
$(BIN)/interpreter: Src/Interpreter.cpp $(BASIC_OBJS) | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BASIC_OBJS) : $(OBJ)/%.o : $(SRC)/%.cpp | $(DIRS)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BASIC_DEPS) : $(OBJ)/%.d : $(SRC)/%.cpp | $(DIRS)
	$(CXX) -E $(CXXFLAGS) $^ -MM -MT $(@:.d=.o) > $@

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NO_DEPS))))
include $(BASIC_DEPS)
endif

#=================================DIRECTORIES===================================

$(OBJ):
	mkdir $(OBJ)

$(GRAPHIC_DUMPS):
	mkdir $(GRAPHIC_DUMPS)

$(BIN):
	mkdir $(BIN)
