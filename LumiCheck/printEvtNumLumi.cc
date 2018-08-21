#include <fstream>

#include "TError.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"

#include "BaconAna/DataFormats/interface/TEventInfo.hh"

using namespace baconhep;
using namespace std;



void printEvtNumLumi(const TString rootFilePref, const UInt_t lumiSec, const UInt_t startIdx, const UInt_t nFiles, const TString inDir)
{
    gErrorIgnoreLevel = kFatal;

    // Output text file
    TString textFileName;
    textFileName.Form(rootFilePref + "_lumi%i.txt", lumiSec);
    fstream buff(textFileName, fstream::out);


    // Loop over ROOT files
    TString eosPath = "root://cmsxrootd.fnal.gov/";
    UInt_t nEvents = 0, nLumiEvents = 0;
    for (unsigned i = startIdx; i < nFiles + startIdx; i++)
    {
        TString rootFileName;
        rootFileName.Form(rootFilePref + "_%i.root", i);

        TFile *rootFile = TFile::Open(eosPath + inDir + "/" + rootFileName);
        TTreeReader reader("Events", rootFile);
        TTreeReaderValue<TEventInfo> fInfo(reader, "Info");


        // Loop over events
        // Print event index (for current tree), file index (for current file), and event number (global)
        cout << "Reading " << rootFileName << "..." << flush;

        UInt_t eventCount = 0, lumiCount = 0;
        while (reader.Next())
        {
            eventCount++;
            if (fInfo->lumiSec == lumiSec)
            {
                lumiCount++;
                buff << reader.GetCurrentEntry() << '\t' << i << '\t' << fInfo->evtNum << endl;
            }
        }
        cout << eventCount << " events, " << lumiCount << " in lumisec " << lumiSec << endl;


        // Cleanup
        nEvents += eventCount;
        nLumiEvents += lumiCount;
        rootFile->Close();
        delete rootFile;
    }
    buff.close();


    cout << endl;
    cout << "Ran over " << nEvents << " total events" << endl;
    cout << "Found " << nLumiEvents << " in lumisec " << lumiSec << endl;
    cout << "Wrote output to " << textFileName << endl << endl << endl;
}
