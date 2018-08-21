#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TString.h"

using namespace std;

void sumEvents(const unsigned nFiles, const unsigned nStart, const TString fileName)
{
    Long64_t nEvents = 0, nTotal = 0;
    TString filePath, num;

    for (unsigned i = nStart; i < nStart + nFiles; i++)
    {
        num.Form("%i", i);
//      if (i == 313)
//          num.Form("%i", nStart + nFiles);
        nEvents = 0;
        TString fullName = fileName + "_" + num + ".root";
        
        filePath = "root://cmsxrootd.fnal.gov/" + fullName;
        TFile *file = TFile::Open(filePath);
        TH1D *hist;

        file->GetObject("TotalEvents", hist);
        nEvents = hist->GetBinContent(1);

        cout << fullName << ": \t" << nEvents << " events" << endl;

        nTotal += nEvents;

        file->Close();
    }

    cout << "All " << nFiles << " files:\t" << nTotal << " events" << endl;
}
