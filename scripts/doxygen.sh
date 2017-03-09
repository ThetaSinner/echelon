#!/bin/bash

build_dir=$(pwd)
scripts_dir=$(dirname $0)

doxyfile=$scripts_dir/../Doxyfile

doxygen_output_dir=$build_dir/docs
doxygen_input_dir=$scripts_dir/../

rm -rf docs && mkdir docs

# This is a rather strange way of specifiying environment specific parameters
# to doxygen by asking doxygen to read from standard input.
# Thanks to http://stackoverflow.com/questions/11032280/specify-doxygen-parameters-through-command-line
( cat $doxyfile ; echo "OUTPUT_DIRECTORY=$doxygen_output_dir"; echo "INPUT=$doxygen_input_dir" ) | doxygen -
