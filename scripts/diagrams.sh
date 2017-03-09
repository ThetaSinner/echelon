#!/bin/bash

rm -rf docs/diagrams && mkdir -p docs/diagrams

build_dir=$(pwd)

diagrams_src_dir=$(dirname $0)/../diagrams
diagrams_build_dir=$build_dir/docs/diagrams

dia --nosplash -t svg $diagrams_src_dir/*.dia -O $diagrams_build_dir
