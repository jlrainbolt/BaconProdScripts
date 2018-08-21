from __future__ import print_function
import numpy as np
import sys


inFilePref = sys.argv[1]

inFileName = inFilePref + "_EvtNum.txt"
outFileName = inFilePref + "_dupes.txt"


# Load array of (evtNum, file index)
print("Loading", inFileName, "into numpy array...") 
sys.stdout.flush()
allEvts = np.loadtxt(inFileName, dtype=[('entryIdx', np.uint64), ('fileIdx', np.uint16), ('evtNum', np.uint64)])


# Get array of duplicates 
print("Processing numpy.unique...")
sys.stdout.flush()
_, uniqIdx = np.unique(allEvts['evtNum'], return_index=True)
dupEvts = np.delete(allEvts, uniqIdx)


# Print 
print("Found", len(dupEvts), "duplicated events") 
np.savetxt(outFileName, dupEvts, fmt='%i', delimiter='\t')
print("Wrote duplicates to", outFileName)
