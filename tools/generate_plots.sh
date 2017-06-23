#!/bin/bash

INDIR=$1
OUTDIR=$2
TOOLDIR=$3
TEMPDIR="$OUTDIR/temp"

if [ -d "$OUTDIR" ]; then
    rm -r $OUTDIR
fi
mkdir -p $OUTDIR

if [ -d "$TEMPDIR" ]; then
    rm -r $TEMPDIR
fi
mkdir -p $TEMPDIR

function generate_maps {

    CURTEMP=$TEMPDIR/map_find
    mkdir $CURTEMP
    
    cp "$INDIR/map find/$1/std map.txt" $CURTEMP
    cp "$INDIR/map find/$1/boost map.txt" $CURTEMP
    cp "$INDIR/map find/$1/boost intrusive set.txt" $CURTEMP
    cp "$INDIR/map find/$1/boost intrusive avl_set.txt" $CURTEMP
    
    python $TOOLDIR/show_plot.py "$CURTEMP" "$OUTDIR/map_find_${1}_tree.png"
    
    rm -r $CURTEMP
    
    
    
    CURTEMP="$TEMPDIR/map_find"
    mkdir "$CURTEMP"
    
    cp "$INDIR/map find/$1/boost unordered_map.txt" $CURTEMP
    cp "$INDIR/map find/$1/boost unordered_map no hash.txt" $CURTEMP
    cp "$INDIR/map find/$1/std unordered_map.txt" $CURTEMP
    cp "$INDIR/map find/$1/std unordered_map no hash.txt" $CURTEMP
    
    python $TOOLDIR/show_plot.py "$CURTEMP" "$OUTDIR/map_find_${1}_hash.png"
    
    rm -r $CURTEMP
    
    
    
    CURTEMP="$TEMPDIR/map_find"
    mkdir $CURTEMP
    
    cp "$INDIR/map find/$1/boost flat_map.txt" $CURTEMP
    cp "$INDIR/map find/$1/std deque.txt" $CURTEMP
    cp "$INDIR/map find/$1/std vector.txt" $CURTEMP
    
    python $TOOLDIR/show_plot.py "$CURTEMP" "$OUTDIR/map_find_${1}_bin_search.png"
    
    rm -r $CURTEMP
    
    
    CURTEMP="$TEMPDIR/map_find"
    mkdir $CURTEMP
    
    cp "$INDIR/map find/$1/$2.txt" $CURTEMP
    cp "$INDIR/map find/$1/$3.txt" $CURTEMP
    cp "$INDIR/map find/$1/$4.txt" $CURTEMP
    
    if [ ! -z "$5" ]
    then
        cp "$INDIR/map find/$1/$5.txt" $CURTEMP
    fi    
    
    python $TOOLDIR/show_plot.py "$CURTEMP" "$OUTDIR/map_find_${1}_compare.png"
    
    rm -r $CURTEMP
    
}

function generate_string_maps {
    CURTEMP=$TEMPDIR/temp
    mkdir $CURTEMP
    
    cp "$INDIR/string map find/$1/$2/std map char p.txt" "$CURTEMP"
    cp "$INDIR/string map find/$1/$2/std map string.txt" "$CURTEMP"    
    cp "$INDIR/string map find/$1/$2/boost map char p.txt" "$CURTEMP"
    cp "$INDIR/string map find/$1/$2/boost map string.txt" "$CURTEMP"
    
    python $TOOLDIR/show_plot.py "$CURTEMP" "$OUTDIR/string_map_find_${1}_${2}.png"
    
    rm -r $CURTEMP
    
    CURTEMP=$TEMPDIR/temp
    mkdir $CURTEMP
    
    cp "$INDIR/string map find/$1/$2/std unordered_map char p.txt" "$CURTEMP"
    cp "$INDIR/string map find/$1/$2/std unordered_map string.txt" "$CURTEMP"    
    cp "$INDIR/string map find/$1/$2/boost unordered_map char p.txt" "$CURTEMP"
    cp "$INDIR/string map find/$1/$2/boost unordered_map string.txt" "$CURTEMP"
    
    python $TOOLDIR/show_plot.py "$CURTEMP" "$OUTDIR/string_unordered_map_find_${1}_${2}.png"
    
    rm -r $CURTEMP
    
    
    mkdir $CURTEMP
    
    cp "$INDIR/string map find/$1/$2/$3.txt" "$CURTEMP"
    cp "$INDIR/string map find/$1/$2/$4.txt" "$CURTEMP"    
    cp "$INDIR/string map find/$1/$2/$5.txt" "$CURTEMP"
    cp "$INDIR/string map find/$1/$2/$6.txt" "$CURTEMP"    
    
    python $TOOLDIR/show_plot.py "$CURTEMP" "$OUTDIR/string_map_compare_${1}_${2}.png"
    
    rm -r $CURTEMP    
}

#===============================================================================

echo "plots generation - maps..."

generate_maps 350 "std map" "std unordered_map" "std vector"

generate_maps 3500 "boost map" "std map" "std unordered_map" "std vector"

generate_maps 100000 "boost map" "std unordered_map" "std vector"

#===============================================================================

echo "plots generation - string maps..."

generate_string_maps 16 100 "std map string" "std map char p" "std unordered_map string" "std unordered_map char p"
generate_string_maps 16 1000 "std map string" "std map char p" "std unordered_map string" "std unordered_map char p"

generate_string_maps 4 100 "std map string" "std map char p" "std unordered_map string" "std unordered_map char p"
generate_string_maps 4 1000 "std map string" "std map char p" "std unordered_map string" "std unordered_map char p"

#===============================================================================

echo "plots generation - switch..."

python "$TOOLDIR/show_plot.py" "$INDIR/switch" "$OUTDIR/switch.png"

#===============================================================================

echo "plots generation - arithmetic..."

python "$TOOLDIR/show_plot_bar.py" "$INDIR/arithmetic" "$OUTDIR/arithmetic.png"

#===============================================================================

echo "plots generation - map insert..."

python "$TOOLDIR/show_plot.py" "$INDIR/map insert/10000" "$OUTDIR/map_insert_10000.png"

#===============================================================================

rm -r $TEMPDIR

