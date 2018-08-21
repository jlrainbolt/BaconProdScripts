#!/bin/bash 

set -e

targName=$1
nStart=$2
inputdir=$3

nFiles=$(eos root://cmseos.fnal.gov ls -1 ${inputdir}/${targName}_*.root | wc -l)

root.exe -q -b "sumEvents.cc($nFiles, $nStart, \"$inputdir/$targName\")"
