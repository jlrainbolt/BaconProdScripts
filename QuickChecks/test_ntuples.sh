#!/bin/bash 

set -e

targName=$1
nStart=$2
inputdir=$3

files=($(eval xrdfs root://cmseos.fnal.gov ls -u ${inputdir} \| grep '${targName}' \| sort -V))
nFiles=${#files[@]}

root.exe -q -b "sumEvents.cc($nFiles, $nStart, \"${inputdir}/${targName}\")"
