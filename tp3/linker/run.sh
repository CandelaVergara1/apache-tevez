#!/bin/bash

set -e

as -g -o main.o main.S

ld --oformat binary -o main.img -T link.ld main.o

objdump -d -j .text main.o > output.txt

printf '\n\n' >> output.txt

hexdump main.img >> output.txt

cat output.txt

rm main.o output.txt
