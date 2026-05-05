#!/bin/bash

set -e

as -g -o main.o main.S

ld --oformat binary -o main.img -T link.ld main.o

objdump -D -b binary -mi8086 main.img | head -n 15

printf '\n'

hexdump main.img

rm main.o 
