#!/usr/bin/env bash

rm -rf ../coverage

mkdir ../coverage
# Point this script to the build folder.
lcov -c --directory ../$1/src\
 --directory ../src\
 --output-file ../coverage/echelon-coverage.info --no-external

mkdir ../coverage/html
genhtml --output-directory ../coverage/html  ../coverage/echelon-coverage.info
