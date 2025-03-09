CC = g++
CFLAGS = -O2

HEADERS_NAMES = Tokens Keywords SpecialSymbols Operators
HEADERS = $(addsuffix .hpp, $(addprefix Src/Headers/, $(HEADERS_NAMES)))

OBJ = obj
BIN = bin

GREEN_COLOR = \033[0;32m
NO_COLOR = \033[0m

tests_tokenizer: $(BIN)/test_tokenizer_num $(BIN)/test_tokenizer_name $(BIN)/test_tokenizer_keywords
	@echo "${GREEN_COLOR}START TESTS${NO_COLOR}\n"

	@echo "\n${GREEN_COLOR}TOKENIZER NUM${NO_COLOR}"
	@$(BIN)/test_tokenizer_num

	@echo "\n${GREEN_COLOR}TOKENIZER NAME${NO_COLOR}"
	@$(BIN)/test_tokenizer_name

	@echo "\n${GREEN_COLOR}TOKENIZER KEYWORD${NO_COLOR}"
	@$(BIN)/test_tokenizer_keywords

$(BIN)/test_tokenizer_keywords: $(OBJ)/Tokenizer.o
	$(CC) Tests/Tokenizer/Keywords.cpp $(CFLAGS) $(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_keywords

$(BIN)/test_tokenizer_num: $(OBJ)/Tokenizer.o
	$(CC) Tests/Tokenizer/Num.cpp $(CFLAGS) $(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_num

$(BIN)/test_tokenizer_name: $(OBJ)/Tokenizer.o
	$(CC) Tests/Tokenizer/Name.cpp $(CFLAGS) $(OBJ)/Tokenizer.o -o $(BIN)/test_tokenizer_name

$(OBJ)/Tokenizer.o: Src/Cpp/Tokenizer.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) Src/Cpp/Tokenizer.cpp -o $(OBJ)/Tokenizer.o

make_dir:
	-mkdir $(OBJ)
	-mkdir $(BIN)