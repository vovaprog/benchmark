#!/bin/bash

TOOLDIR=$1
OUTDIR=$2

if [ ! -d "$OUTDIR" ]; then
    mkdir -p $OUTDIR
fi

CPP_COMPILER=$3
CPP_FLAGS=$4
BOOST_VERSION=`./benchmark GetBoostVersion`
CPU=`perl -n -e '/^(model name\s+:\s+(.+))$/ && print($2) && exit;\n' "/proc/cpuinfo"`
CPP_COMPILER_VERSION=`$CPP_COMPILER --version | head -1`
OS=`lsb_release -d -s`

perl -p -e "s/BOOST_VERSION/$BOOST_VERSION/" "$TOOLDIR/README_template.md" |
perl -p -e "s/CPU_INFO/$CPU/" |
perl -p -e "s/OS_INFO/$OS/" |
perl -p -e "s/CPP_COMPILER_VERSION/$CPP_COMPILER_VERSION/" |
perl -p -e "s|CPP_COMPILER|$CPP_COMPILER|" |
perl -p -e "s/CPP_FLAGS/$CPP_FLAGS/" > "$OUTDIR/README.md"

markdown "$OUTDIR/README.md" > "$OUTDIR/README.html"
