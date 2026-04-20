#!/bin/bash

set -e

PYTHON_SRC="gini_index.py"
GINILIB="libgini.so"
C_SRC="number_conversor.c"

gcc -shared -W -fPIC -o ${GINILIB} ${C_SRC}
python ${PYTHON_SRC}

rm ${GINILIB}

