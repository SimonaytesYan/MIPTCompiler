## Сборка и использование

### Компиляция
Чтобы собрать компилятор, запустите команду
```
make compiler
```

Исполняемый файл можно будет найти по пути `bin/compiler`.

Для работы с компилятором, передайте ему единственным аргументом путь до файла с исходным кодом
```
bin/compiler Examples/print.sym
```
По завершении исполнения будет создан исполняемый файл `a.out`.

### Интерпретация
Чтобы собрать интерпретатор, запустите команду
```
make interpreter
```

Исполняемый файл можно будет найти по пути `bin/interpreter`.

Для работы с интерпретатором, передайте ему единственным аргументом путь до файла с исходным кодом
```
bin/interpreter Examples/print.sym
```

### Примеры кода
Примеры для программ можно найти в папке `Examples`.

## Синтаксис языка

Динамическая типизация.

Имена переменных содержат маленькие и большие буквы, знак '_', а так же цифры, но не на первой позиции.

f() ?
f() !

## TODO

Тесты для loop

Добавить header-ы в зависимости тестов
Унифицировать сборку(и запуск) тестов в Makefile

Добавить позицию в токены и узлы AST. Сделать с помощью этого индикацию ошибок

Заменить тесты на google tests

Унифицировать пассы по дереву

Make => Makefile
