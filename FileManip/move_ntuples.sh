#!/bin/bash 

set -e

srcName=$1
targName=$2
startIdx=$3
srcDir=$4
targDir=$5

nFiles=$(eos root://cmseos.fnal.gov ls -1 ${srcDir}/${srcName}_*.root | wc -l)

echo "Moving ${nFiles} \"${srcName}\" files in ${srcDir} to ${targDir} as \"${targName}\""

i=$startIdx
for filename in `eos root://cmseos.fnal.gov ls -1 ${srcDir}/${srcName}_*.root`
do
    echo "Moved ${filename} to ${targName}_${i}.root"
    xrdfs root://cmseos.fnal.gov mv ${srcDir}/${filename} ${targDir}/${targName}_${i}.root
    i=$((i + 1))
done

nFiles=$((i - startIdx))
echo "Moved ${nFiles} files to ${targDir}"
