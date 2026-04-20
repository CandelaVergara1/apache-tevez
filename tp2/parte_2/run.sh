#!/bin/bash

set -e

# --- Configuration ---
C_SRC="number_conversor.c"
ASM_SRC="conversor.s"
C_OBJ="c_conversor.o"
ASM_OBJ="conversor.o"
PROGRAM="program"

gcc -g3 -O0 -c -o ${C_OBJ} ${C_SRC}
as --64 -g -o ${ASM_OBJ} ${ASM_SRC}
gcc -g3 -O0 -c -o cJSON.o cJSON.c
gcc -g3 -O0 -o ${PROGRAM} ${C_OBJ} ${ASM_OBJ} cJSON.o -lcurl

gdb ./${PROGRAM}

rm ${PROGRAM} ${C_OBJ} ${ASM_OBJ} cJSON.o

