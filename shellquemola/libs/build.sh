#!/usr/bin/env bash
rm *.so
for i in *.c
do
	gcc -Wall -shared -fPIC "$i" -o "${i%.c}.so"
done
