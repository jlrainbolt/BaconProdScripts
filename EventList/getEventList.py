from __future__ import print_function
import numpy as np
import sys
import time


inFilePref = sys.argv[1]
startIdx = int(sys.argv[2])
nFiles = int(sys.argv[3])

inFileName = inFilePref + "_EvtNum.txt"
outFilePref = inFilePref + "_EventList"


# Load array of (entryIdx, fileIdx, evtNum)
print("Loading", inFileName, "into numpy array...", end='') 
sys.stdout.flush()
stTime = time.clock()
allEvts = np.loadtxt(inFileName, dtype=[('entryIdx', np.uint64), ('fileIdx', np.uint16), ('evtNum', np.uint64)])
elTime = time.clock() - stTime
print(elTime, "seconds")



# Get array indices of unique events (prefers low fileIdx)
print("Processing numpy.unique...", end='')
sys.stdout.flush()
stTime = time.clock()
_, uniqIdx = np.unique(allEvts['evtNum'], return_index=True)
elTime = time.clock() - stTime
print(elTime, "seconds")


# Find number of duplicated events
nDup = len(allEvts) - len(uniqIdx)
if (nDup == 0):
    print("No duplicates found!\n")

# Yikes, now we have to deal with them
else:
    print("Found", nDup, "total duplicated events", end='\n\n') 

    # Make complete array of unique events
    uniqEvts = allEvts[uniqIdx]


    # Get array indices of duplicate events
    isDup = np.ones(len(allEvts), dtype=bool)
    isDup[uniqIdx] = False
    dupIdx = np.where(isDup)


    # Identify which input files contain duplicated events
    for i in range(startIdx, startIdx + nFiles):
        print("Checking", inFilePref + "_" + str(i) + ".root...", end='')
        sys.stdout.flush()

        if np.any(allEvts[dupIdx]['fileIdx'] == i):
            evtList = np.extract(uniqEvts['fileIdx'] == i, uniqEvts['entryIdx'])

            # If a file contains only duplicates
            if (evtList.size == 0):
                print("all duplicates!! (please delete)")

            # Write EventList for each file that has mixed unique and duplicated events
            else:
                outFileName = outFilePref + "_" + str(i) + ".txt"
                np.savetxt(outFileName, np.sort(evtList), fmt='%i', delimiter='\t')
                print("some duplicates;", evtList.size, "unique events (see", outFileName+")")
        else:
            print("all clear!")
