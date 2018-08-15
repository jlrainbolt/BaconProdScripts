#!/bin/bash 

set -e

targName=$1
inputdir=$2

echo "Deleting $targName Ntuples from input directory ${inputdir}"

nFiles=$(eos root://cmseos.fnal.gov ls -1 ${inputdir}/${targName}_*.root | wc -l)
echo "Directory contains ${nFiles} root files"

for filename in `eos root://cmseos.fnal.gov ls -1 ${inputdir}/${targName}_*.root`
do
    echo "Deleting file ""$filename"
    eos root://cmseos.fnal.gov rm "$inputdir""/""$filename"
done
