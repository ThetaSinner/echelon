#!/bin/bash

scripts_dir=$(dirname $0)

$scripts_dir/doxygen.sh
$scripts_dir/diagrams.sh
