#!/bin/bash

SITEDIR=$1
OUTDIR=$2

if [ ! -d "$OUTDIR" ]; then
    mkdir -p $OUTDIR
fi

CPP_COMPILER=$3
CPP_FLAGS=$4
STD_VERSION=`./benchmark StdVersion`
BOOST_VERSION=`./benchmark BoostVersion`
CPU=`perl -n -e '/^(model name\s+:\s+(.+))$/ && print($2) && exit;\n' "/proc/cpuinfo"`
CPP_COMPILER_VERSION=`$CPP_COMPILER --version | head -1`
OS=`lsb_release -d -s`

perl -p -e "s/BOOST_VERSION/$BOOST_VERSION/" "$SITEDIR/index.md" |
perl -p -e "s/STD_VERSION/$STD_VERSION/" |
perl -p -e "s/CPU_INFO/$CPU/" |
perl -p -e "s/OS_INFO/$OS/" |
perl -p -e "s/CPP_COMPILER_VERSION/$CPP_COMPILER_VERSION/" |
perl -p -e "s|CPP_COMPILER|$CPP_COMPILER|" |
perl -p -e "s/CPP_FLAGS/$CPP_FLAGS/" > "$OUTDIR/index.md"

markdown "$OUTDIR/index.md" > "$OUTDIR/index.html"
rm "$OUTDIR/index.md"

markdown "$SITEDIR/arithmetic.md" > "$OUTDIR/arithmetic.html"
markdown "$SITEDIR/map-find-int.md" > "$OUTDIR/map-find-int.html"
markdown "$SITEDIR/map-find-string.md" > "$OUTDIR/map-find-string.html"
markdown "$SITEDIR/arithmetic.md" > "$OUTDIR/arithmetic.html"
markdown "$SITEDIR/map-insert.md" > "$OUTDIR/map-insert.html"
markdown "$SITEDIR/switch.md" > "$OUTDIR/switch.html"

