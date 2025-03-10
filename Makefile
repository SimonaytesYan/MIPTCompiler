CC = g++
CFLAGS = -O2

HEADERS_NAMES = Tokens Keywords SpecialSymbols Operators
HEADERS = $(addsuffix .hpp, $(addprefix Src/Headers/, $(HEADERS_NAMES)))

OBJ = obj
BIN = bin
GRAPHIC_DUMPS = GraphicDumps

GREEN_COLOR = \033[0;32m
NO_COLOR = \033[0m

test_lexer_dump: $(BIN)/test_lexer_dump_num $(BIN)/test_lexer_dump_var $(BIN)/test_lexer_dump_expr
	@echo "${GREEN_COLOR}START LEXER TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}LEXER NUM${NO_COLOR}"
	@$(BIN)/test_lexer_dump_num
	@echo "\n${GREEN_COLOR}LEXER VAR${NO_COLOR}"
	@$(BIN)/test_lexer_dump_var
	@echo "\n${GREEN_COLOR}LEXER EXPRESSION${NO_COLOR}"
	@$(BIN)/test_lexer_dump_expr

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


$(BIN)/test_lexer_dump_num: Tests/Lexer/Num.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o
	$(CC) $(CFLAGS) Tests/Lexer/Num.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/GraphicDumpPass.o -o $(BIN)/test_lexer_dump_num

$(BIN)/test_lexer_dump_var: Tests/Lexer/Var.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o
	$(CC) $(CFLAGS) Tests/Lexer/Var.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/GraphicDumpPass.o -o $(BIN)/test_lexer_dump_var

$(BIN)/test_lexer_dump_expr: Tests/Lexer/Expr.cpp $(OBJ)/Tokenizer.o $(OBJ)/Lexer.o $(OBJ)/GraphicDumpPass.o
	$(CC) $(CFLAGS) Tests/Lexer/Expr.cpp \
					$(OBJ)/Tokenizer.o 	\
					$(OBJ)/Lexer.o		\
					$(OBJ)/GraphicDumpPass.o -o $(BIN)/test_lexer_dump_expr


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

$(OBJ)/Lexer.o: Src/Cpp/Lexer.cpp $(HEADER)
	$(CC) -c $(CFLAGS) Src/Cpp/Lexer.cpp -o $(OBJ)/Lexer.o

$(OBJ)/GraphicDumpPass.o: Src/Cpp/GraphicDumpPass.cpp $(HEADER)
	$(CC) -c $(CFLAGS) Src/Cpp/GraphicDumpPass.cpp -o $(OBJ)/GraphicDumpPass.o


make_dir:
	-mkdir $(OBJ)
	-mkdir $(BIN)
	-mkdir $(GRAPHIC_DUMPS)

clean:
	rm $(BIN)/*
	rm $(OBJ)/*