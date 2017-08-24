#!/bin/bash

magnify_source="$(dirname $(readlink -f $BASH_SOURCE))"

rootfile="$1" ; shift
if [[ "$rootfile" =~ :// ]] ; then
    echo "Loading URL $rootfile"
else
    rootfile=$(readlink -f "$rootfile")
fi
frame="${1:-decon}"
startdir=$(pwd)

echo "Loading frame \"$frame\""

cd $magnify_source/scripts

echo $rootfile
echo $frame


root -l loadClasses.C Magnify.C'("'"$rootfile"'","'"$frame"'")'