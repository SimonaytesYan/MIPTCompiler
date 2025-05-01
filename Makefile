#==================================VARIABLES====================================

#------------------------------COMPILER AND FLAGS-------------------------------
ifeq ($(origin CXX), default)
	CXX = clang++
endif

LLVM_CONFIG ?= llvm-config

DEBUG_FLAGS = -D _DEBUG -ggdb3 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192 -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,leak,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
SANITIZER_FLAGS = -g -fcheck-new -fsized-deallocation -fstack-protector \
				  -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
				  -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr,leak
RELEASE_FLAGS = -O2 -std=c++2a
LLVM_FLAGS = `$(LLVM_CONFIG) --cxxflags --ldflags --libs`
COMMON_INC = -ISrc/Headers -ISrc/StdLib

CXXFLAGS =
override CXXFLAGS += $(COMMON_INC) $(RELEASE_FLAGS)

# LDFLAGS = $(LLVM_FLAGS)

#--------------------------------FILES AND DIRS---------------------------------

#-----------------HEADERS----------------
HEADERS_NAMES = Tokens Keywords SpecialSymbols Operators Grammar Lexer Tokenizer ExecutionPass Logger
HEADERS = $(addsuffix .hpp, $(addprefix Src/Headers/, $(HEADERS_NAMES)))

#--------------BASIC_SOURCES-------------
BASIC_SRC_NAMES = ExecutionPass GraphicDumpPass Grammar Lexer Tokenizer VariableList # IRBuilderPass
# BASIC_CPPS = $(addsuffix .cpp, $(addprefix $(SRC), $(BASIC_SRC_NAMES)))

BASIC_OBJS = $(addsuffix .o, $(addprefix $(OBJ)/, $(BASIC_SRC_NAMES)))
BASIC_DEPS = $(BASIC_OBJS:.o=.d)

#-----------LLVM DEPENDED SOURCES-------
LLVM_DEP_SRC_NAMES = IRBuilderPass
LLVM_DEP_CPPS = $(addsuffix .cpp, $(addprefix $(SRC), $(LLVM_DEP_SRC_NAMES)))

LLVM_DEP_OBJS = $(addsuffix .o, $(addprefix $(OBJ)/, $(LLVM_DEP_SRC_NAMES)))
LLVM_DEP_DEPS = $(LLVM_DEP_OBJS:.o=.d)

#-------------TOKENIZER TESTS------------
TOKEN_T_DIR = Tests/Tokenizer
TOKEN_T_BIN_DIR = $(BIN)/$(TOKEN_T_DIR)
TOKEN_T_SRC_NAMES = SpecialSymbols Operators Keywords Num Name

TOKEN_T_CPPS = $(addsuffix .cpp, $(addprefix $(TOKEN_T_DIR), $(TOKEN_T_SRC_NAMES)))
TOKEN_T_BIN = $(addprefix $(TOKEN_T_BIN_DIR)/, $(TOKEN_T_SRC_NAMES))

#-------------LEXER TESTS------------
LEXER_T_DIR = Tests/Lexer
LEXER_T_BIN_DIR = $(BIN)/$(LEXER_T_DIR)
LEXER_T_SRC_NAMES = Expr Objects Scope

LEXER_T_CPPS = $(addsuffix .cpp, $(addprefix $(LEXER_T_DIR), $(LEXER_T_SRC_NAMES)))
LEXER_T_BIN = $(addprefix $(LEXER_T_BIN_DIR)/, $(LEXER_T_SRC_NAMES))

#-------------EXECUTION PASS TESTS------------
INTER_T_DIR = Tests/ExecutionPass
INTER_T_BIN_DIR = $(BIN)/$(INTER_T_DIR)
INTER_T_SRC_NAMES = Expression Objects Scope

INTER_T_CPPS = $(addsuffix .cpp, $(addprefix $(INTER_T_DIR), $(INTER_T_SRC_NAMES)))
INTER_T_BIN = $(addprefix $(INTER_T_BIN_DIR)/, $(INTER_T_SRC_NAMES))

#-------------IR BUILDER PASS TESTS-----------
IR_BUILD_T_DIR = Tests/IRBuilderPass
IR_BUILD_T_BIN_DIR = $(BIN)/$(IR_BUILD_T_DIR)
IR_BUILD_T_SRC_NAMES = NumExpressions Vars Ifs Loops

IR_BUILD_T_CPPS = $(addsuffix .cpp, $(addprefix $(IR_BUILD_T_DIR), $(IR_BUILD_T_SRC_NAMES)))
IR_BUILD_T_BIN = $(addprefix $(IR_BUILD_T_BIN_DIR)/, $(IR_BUILD_T_SRC_NAMES))

#-----------------DIRS----------------
OBJ = obj
BASIC_SRC = Src
SRC = $(BASIC_SRC)/Cpp
BIN = bin
GRAPHIC_DUMPS = graphic_dumps

DIRS = $(OBJ) $(BIN) $(GRAPHIC_DUMPS)

#----------------------------------------OTHER----------------------------------
NO_DEPS=clean

GREEN_COLOR = \033[0;32m
NO_COLOR = \033[0m

.PHONY: interpreter test clean test_execution test_lexer_dump test_tokenizer test_ir_builder
#==================================PHONY TARGETS=================================
interpreter: $(BIN)/interpreter

compiler: $(BIN)/compiler

test: test_execution test_ir_builder test_lexer_dump test_tokenizer

clean:
	-rm -r $(BIN)
	-rm -r $(OBJ)
	-rm -r $(GRAPHIC_DUMPS)

#==================================TEST RUNNERS=================================
test_ir_builder: $(IR_BUILD_T_BIN) $(OBJ)/StdLib.o
	@echo "${GREEN_COLOR}START IR BUILDER TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}BUILD NUM EXPRESSIONS${NO_COLOR}"
	-@$(IR_BUILD_T_BIN_DIR)/NumExpressions

	@echo "\n${GREEN_COLOR}BUILD VARS${NO_COLOR}"
	-@$(IR_BUILD_T_BIN_DIR)/Vars

	@echo "\n${GREEN_COLOR}BUILD IFS${NO_COLOR}"
	-@$(IR_BUILD_T_BIN_DIR)/Ifs

	@echo "\n${GREEN_COLOR}BUILD LOOPS${NO_COLOR}"
	-@$(IR_BUILD_T_BIN_DIR)/Loops

test_execution: $(INTER_T_BIN)
	@echo "${GREEN_COLOR}START EXECUTION TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}EXECUTION OBJECTS${NO_COLOR}"
	-@$(INTER_T_BIN_DIR)/Objects

	@echo "\n${GREEN_COLOR}EXECUTION EXPRESSION${NO_COLOR}"
	-@$(INTER_T_BIN_DIR)/Expression

	@echo "\n${GREEN_COLOR}EXECUTION SCOPE${NO_COLOR}"
	-@$(INTER_T_BIN_DIR)/Scope

test_lexer_dump: $(LEXER_T_BIN)
	@echo "${GREEN_COLOR}START LEXER TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}LEXER SCOPE${NO_COLOR}"
	-@$(LEXER_T_BIN_DIR)/Scope

	@echo "\n${GREEN_COLOR}LEXER EXPRESSION${NO_COLOR}"
	-@$(LEXER_T_BIN_DIR)/Expr

	@echo "\n${GREEN_COLOR}LEXER OBJECTS${NO_COLOR}"
	-@$(LEXER_T_BIN_DIR)/Objects

test_tokenizer: $(TOKEN_T_BIN)
	@echo "${GREEN_COLOR}START TOKENIZER TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}TOKENIZER NUM${NO_COLOR}"
	@$(TOKEN_T_BIN_DIR)/Num

	@echo "\n${GREEN_COLOR}TOKENIZER NAME${NO_COLOR}"
	@$(TOKEN_T_BIN_DIR)/Name

	@echo "\n${GREEN_COLOR}TOKENIZER KEYWORD${NO_COLOR}"
	@$(TOKEN_T_BIN_DIR)/Keywords

	@echo "\n${GREEN_COLOR}TOKENIZER OPERATOR${NO_COLOR}"
	@$(TOKEN_T_BIN_DIR)/Operators

	@echo "\n${GREEN_COLOR}TOKENIZER SPECIAL SYMBOL${NO_COLOR}"
	@$(TOKEN_T_BIN_DIR)/SpecialSymbols

#---------------------------BUILD IR PASS TESTS--------------------------------
$(IR_BUILD_T_BIN) : $(IR_BUILD_T_BIN_DIR)/% : $(IR_BUILD_T_DIR)/%.cpp $(BASIC_OBJS) $(LLVM_DEP_OBJS)
	@mkdir -p $(@D)
	echo "Hui"
	$(CXX) $(CXXFLAGS) $(LLVM_FLAGS) $^ -o $@

#---------------------------EXECUTION PASS TESTS--------------------------------
$(INTER_T_BIN) : $(INTER_T_BIN_DIR)/% : $(INTER_T_DIR)/%.cpp $(BASIC_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

#------------------------------LEXER TESTS--------------------------------------
$(LEXER_T_BIN) : $(LEXER_T_BIN_DIR)/% : $(LEXER_T_DIR)/%.cpp $(BASIC_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

#----------------------------TOKENIZER TESTS------------------------------------
$(TOKEN_T_BIN) : $(TOKEN_T_BIN_DIR)/% : $(TOKEN_T_DIR)/%.cpp $(OBJ)/Tokenizer.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

#====================================SOURCE=====================================
$(BIN)/interpreter: Src/Interpreter.cpp $(BASIC_OBJS) | $(DIRS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/compiler: Src/Compiler.cpp $(BASIC_OBJS) $(OBJ)/StdLib.o $(LLVM_DEP_OBJS) | $(DIRS)
	$(CXX) $(CXXFLAGS) $(LLVM_FLAGS) $^ -o $@

$(OBJ)/StdLib.o: $(BASIC_SRC)/StdLib/StdLib.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

$(LLVM_DEP_OBJS) : $(OBJ)/%.o : $(SRC)/%.cpp | $(DIRS)
	$(CXX) -c $(CXXFLAGS) $(LLVM_FLAGS) $< -o $@

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
