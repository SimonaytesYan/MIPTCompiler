## Сборка и использование

Чтобы собрать интерпретатор, запустите команду
```
make interpreter
```

Исполняемый файл можно будет найти по пути `bin/Interpreter`.

Для работы с интерпретатором, передайте ему единственным аргументом путь до файла с исходным кодом
```
bin/Interpreter Examples/print.sym
```

Примеры для программ можно найти в папке `Examples`.

## Синтаксис языка

Динамическая типизация.

Имена переменных содержат маленькие и большие буквы, знак '_', а так же цифры, но не на первой позиции.

f() ?
f() !

## TODO
Заменить тесты на google tests

Унифицировать пассы по дереву

Make => Makefile
