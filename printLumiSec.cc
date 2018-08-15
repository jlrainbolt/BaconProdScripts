#include <fstream>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "BaconAna/DataFormats/interface/TEventInfo.hh"

using namespace baconhep;
using namespace std;



void printLumiSec(const UInt_t badLumiSec, const TString rootFilePref, const UInt_t nFiles, const UInt_t minIdx, const TString inDir)
{
    // Output text file
    TString textFileName;
    textFileName.Form(rootFilePref + "_%i.txt", badLumiSec);
    fstream buff(textFileName, fstream::out);


    // Loop over ROOT files
    TString eosPath = "root://cmsxrootd.fnal.gov/";
    UInt_t nEvents = 0;
    for (unsigned i = minIdx; i < nFiles + minIdx; i++)
    {
        TString rootFileName;
        rootFileName.Form(rootFilePref + "_%i.root", i);

        TFile *rootFile = TFile::Open(eosPath + inDir + "/" + rootFileName);
        TTreeReader reader("Events", rootFile);
        TTreeReaderValue<TEventInfo> fInfo(reader, "Info");


        // Loop over events
        if (i == minIdx)
            cout << endl;
        cout << "Processing " << rootFileName << "..." << flush;

        UInt_t eventCount = 0;
        while (reader.Next())
        {
            eventCount++;
            if (fInfo->lumiSec == badLumiSec)
                buff << fInfo->evtNum << '\t' << i << endl;
        }
        cout << eventCount << " events" << endl;


        // Cleanup
        nEvents += eventCount;
        rootFile->Close();
        delete rootFile;
    }
    buff.close();


    cout << endl;
    cout << "Ran over " << nEvents << " total events" << endl;
    cout << "Wrote output to " << textFileName << endl;
}
