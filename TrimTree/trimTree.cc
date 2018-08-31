#include <fstream>

#include "TError.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TEventList.h"
#include "TH1.h"

// #include "BaconAna/DataFormats/interface/TEventInfo.hh"

//using namespace baconhep;
using namespace std;



void trimTree(const TString filePref, const UInt_t fileIdx)
{
//  gErrorIgnoreLevel = kError;


    //--- MAKE EVENT LIST ---//

    TEventList *list = new TEventList();


    // Open input text file
    TString textFileName;
    textFileName.Form(filePref + "_EventList_%i.txt", fileIdx);
    ifstream file(textFileName); 


    // Loop over list of entry indices
    cout << "Loading " << textFileName << " into TEventList..." << flush;
    Long64_t entry;
    while (file)
    {
        file >> entry;
        list->Enter(entry);
    }
    file.close();
    Double_t nEvts = list->GetN();
    cout << "contains " << nEvts << " events" << endl;



    //--- CREATE NEW FILE ---//

    TString inFileName, outFileName;
    inFileName.Form(filePref + "_%i.root", fileIdx);
    outFileName.Form(filePref + "_trimmed_%i.root", fileIdx);


    // Grab everything from input file
    cout << "Loading contents from " << inFileName << "..." << endl;
    TFile *inFile = new TFile(inFileName);
    TTree *inTree, *inXS;
    inFile->GetObject("Events", inTree);
    inFile->GetObject("xs", inXS);


    // Apply event list
    inTree->SetEventList(list);


    // Create new file and copy everything over
    cout << "Copying trimmed contents to " << outFileName << "..." << endl;
    TFile *outFile = new TFile(outFileName, "RECREATE");
    gROOT->cd();
    TTree *outTree = inTree->CopyTree("");
    TTree *outXS = inXS->CopyTree("");


    // Correct TotalEvents histogram
    cout << "Adjusting TotalEvents histogram..." << endl;
    TH1D *outHist;
    inFile->GetObject("TotalEvents", outHist);
    outHist->SetDirectory(0);
    outHist->SetBinContent(1, nEvts);
    outHist->SetEntries(nEvts);


    // Write to file
    cout << "Writing and closing " << outFileName << "..." << endl;
    outHist->Write();
    outTree->Write();
    outXS->Write();
//  outFile->Purge();
    outFile->Close();
//  inFile->Close();
    cout << "Done!" << endl << endl << endl;
}
