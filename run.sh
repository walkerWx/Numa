#!/bin/bash
input=$1
output=${input##*\/}
make && ./bin/matcher -extra-arg=-I/Users/walker/.local/share/iRRAM_2013_01/include/ $1 -- > ./output/$output
diff $1 ./output/$output
clang++ ./output/$output src/fixup.cpp -Isrc -o ./output/bin/${output%%.cc}

