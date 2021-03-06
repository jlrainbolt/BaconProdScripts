#include <fstream>

#include "TError.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TEventList.h"
#include "TH1.h"

#include "BLT/BLTAnalysis/interface/BLTHelper.hh"
// #include "BaconAna/DataFormats/interface/TEventInfo.hh"

//using namespace baconhep;
using namespace std;



void trimLumi(const TString filePref, const UInt_t fileIdx)
{
//  gErrorIgnoreLevel = kError;


    TString inFileName, outFileName;
    inFileName.Form(filePref + "_%i.root", fileIdx);
    outFileName.Form(filePref + "_trimmed_%i.root", fileIdx);


    // Grab everything from input file
    cout << "Loading contents from " << inFileName << "..." << endl;
    TFile *inFile = TFile::Open(inFileName);
    TTree *inTree, *inXS;
    inFile->GetObject("Events", inTree);
    inFile->GetObject("xs", inXS);


    // Create new file and copy everything over
    cout << "Copying trimmed contents to " << outFileName << "..." << endl;
    TFile *outFile = new TFile(outFileName, "RECREATE");
    TTree *outTree = inTree->CopyTree("Entry$>=139");
    TTree *outXS = inXS->CopyTree("");


    // Correct TotalEvents histogram
    TH1D *outHist;
    inFile->GetObject("TotalEvents", outHist);
    outHist->SetDirectory(0);
    Double_t nEvts = (Double_t) outTree->GetEntries();
    outHist->SetBinContent(1, nEvts);
    outHist->SetEntries(nEvts);

    outHist->Write();
    outTree->Write();
    outXS->Write();
//  outFile->Purge();
    outFile->Close();
//  inFile->Close();
    cout << "Done!" << endl << endl << endl;
}
