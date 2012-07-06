#!/bin/bash
make clean
make distclean
echo "Executando aclocal..."
aclocal
echo "Executando autoheader..."
autoheader
echo "Executando automake..."
automake
echo "Executando autoconf..."
autoconf
echo "Executando ./configure..."
./configure
make

