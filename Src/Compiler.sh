FILE=$1

set -x

clang++ $FILE obj/StdLib.o -o bin/a.out
