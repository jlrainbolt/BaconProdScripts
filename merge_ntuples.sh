#!/bin/bash 

set -e

nTarget=$1
inName="Output"
outName=$2
inDir=$3
outDir=$4



nFiles=$(eos root://cmseos.fnal.gov ls -1 ${inDir}/${inName}_*.root | wc -l)
nSource=$((nFiles / nTarget))
remainder=$((nFiles - (nTarget * nSource)))

echo "Directory ${inDir} contains ${nFiles} input \"${inName}\" files"
if [ "$remainder" -ne 0 ]
then
    nSource1=$nSource
    nSource2=$((nSource + 1))
    echo "${nTarget} output \"${outName}\" files => ${nSource1} or ${nSource2} input \"${inName}\" files each"
else
    nSource1=$nSource
    echo "${nTarget} output \"${outName}\" files => ${nSource1} input \"${inName}\" files each"
fi


files=($(eos root://cmseos.fnal.gov ls -1 ${inDir}/${inName}_*.root))
startIndex=0



for ((i=0; i<nTarget; i+=1))
do
    echo ""

    # Pick nSource to ensure most even file splitting
    if [ "$remainder" -ne 0 ]
    then
        nSource=$nSource2
        remainder=$((remainder - 1))
    else
        nSource=$nSource1
    fi


    for filename in ${files[@]:startIndex:nSource}
    do
        echo "Making local copy of ${filename}"
        xrdcp root://cmseos.fnal.gov/${inDir}/${filename} .
    done
    hadd ${outName}_${i}.root ${files[@]:startIndex:nSource}
    echo "Copying ${outName}_${i}.root to ${outDir}"
    if xrdcp ${outName}_${i}.root root://cmseos.fnal.gov/${outDir}/${outName}_${i}.root
    then
        echo "Deleting local copy of ${outName}_${i}.root"
        rm ${outName}_${i}.root

        for filename in ${files[@]:startIndex:nSource}
        do
            echo "Deleting local copy of ${filename}"
            rm ${filename}
        done
    fi


    startIndex=$((startIndex + nSource))
done


if [ $startIndex == $nFiles ]
then
    echo "Done!  Don't forget to check the output :)"
else
    echo "There was a problem :("
fi
