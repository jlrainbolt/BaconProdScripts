#!/bin/bash 

set -e

lumiSec=$1
targName=$2
nStart=$3
inputdir=$4

nFiles=$(eos root://cmseos.fnal.gov ls -1 ${inputdir}/${targName}_*.root | wc -l)

root.exe -q -b "printLumiSec.cc($lumiSec, \"$targName\", $nFiles, $nStart, \"$inputdir\")"
