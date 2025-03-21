CC = g++
DEBUG_FLAGS = -D _DEBUG -ggdb3 -std=c++2a -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wstack-usage=8192 -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,leak,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
SANITIZER_FLAGS = -g -fcheck-new -fsized-deallocation -fstack-protector \
				  -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer \
				  -pie -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr,leak
CFLAGS = $(SANITIZER_FLAGS)

HEADERS_NAMES = Tokens Keywords SpecialSymbols Operators Grammar Lexer Tokenizer
HEADERS = $(addsuffix .hpp, $(addprefix Src/Headers/, $(HEADERS_NAMES)))

OBJ = obj
BIN = bin
GRAPHIC_DUMPS = GraphicDumps

GREEN_COLOR = \033[0;32m
NO_COLOR = \033[0m

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


$(BIN)/test_lexer_dump_objects: Tests/Lexer/Objects.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o
	$(CC) $(CFLAGS) Tests/Lexer/Objects.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/Grammar.o	\
					$(OBJ)/GraphicDumpPass.o -o $(BIN)/test_lexer_dump_objects

$(BIN)/test_lexer_dump_expr: Tests/Lexer/Expr.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o
	$(CC) $(CFLAGS) Tests/Lexer/Expr.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/Grammar.o	\
					$(OBJ)/GraphicDumpPass.o -o $(BIN)/test_lexer_dump_expr

$(BIN)/test_lexer_dump_scope: Tests/Lexer/Scope.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o
	$(CC) $(CFLAGS) Tests/Lexer/Scope.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/Grammar.o	\
					$(OBJ)/GraphicDumpPass.o -o $(BIN)/test_lexer_dump_scope

$(BIN)/test_tokenizer_spec_symbols: $(OBJ)/Tokenizer.o
	$(CC) $(CFLAGS) Tests/Tokenizer/SpecialSymbols.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_spec_symbols

$(BIN)/test_tokenizer_operators: $(OBJ)/Tokenizer.o
	$(CC) $(CFLAGS) Tests/Tokenizer/Operators.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_operators

$(BIN)/test_tokenizer_keywords: $(OBJ)/Tokenizer.o
	$(CC) $(CFLAGS) Tests/Tokenizer/Keywords.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_keywords

$(BIN)/test_tokenizer_num: $(OBJ)/Tokenizer.o
	$(CC) $(CFLAGS) Tests/Tokenizer/Num.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_num

$(BIN)/test_tokenizer_name: $(OBJ)/Tokenizer.o
	$(CC) $(CFLAGS) Tests/Tokenizer/Name.cpp \
					$(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_name


$(OBJ)/Tokenizer.o: Src/Cpp/Tokenizer.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) Src/Cpp/Tokenizer.cpp -o $(OBJ)/Tokenizer.o

$(OBJ)/Lexer.o: Src/Cpp/Lexer.cpp $(HEADER) $(OBJ)/Grammar.o
	$(CC) -c $(CFLAGS) Src/Cpp/Lexer.cpp \
					   $(OBJ)/Grammar.o -o $(OBJ)/Lexer.o

$(OBJ)/GraphicDumpPass.o: Src/Cpp/GraphicDumpPass.cpp $(HEADER) $(OBJ)/Grammar.o
	$(CC) -c $(CFLAGS) Src/Cpp/GraphicDumpPass.cpp \
					   $(OBJ)/Grammar.o -o $(OBJ)/GraphicDumpPass.o

$(OBJ)/Grammar.o:  Src/Cpp/Grammar.cpp $(HEADER)
	$(CC) -c $(CFLAGS) Src/Cpp/Grammar.cpp -o $(OBJ)/Grammar.o

make_dir:
	-mkdir $(OBJ)
	-mkdir $(BIN)
	-mkdir $(GRAPHIC_DUMPS)

clean:
	-rm $(BIN)/*
	-rm $(OBJ)/*
	-rm $(GRAPHIC_DUMPS)/*
