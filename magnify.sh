#!/bin/bash

magnify_source="$(dirname $(readlink -f $BASH_SOURCE))"

rootfile="$1" ; shift
if [[ "$rootfile" =~ :// ]] ; then
    echo "Loading URL $rootfile"
else
    rootfile=$(readlink -f "$rootfile")
fi
experiment="${1:-uboone}" ; shift
frame="${1:-decon}"
startdir=$(pwd)
rebin="${2:-1}"

echo "Loading | experiment \"$experiment\" | frame \"$frame\" | rebin \"$rebin\" |"

cd $magnify_source/scripts

# echo $rootfile
# echo $frame
# echo $rebin

root -l loadClasses.C Magnify.C'("'"$rootfile"'", "'"$experiment"'", "'"$frame"'", '$rebin')'
