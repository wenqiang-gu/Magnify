#include "Data.h"
#include "Waveforms.h"

#include "TH2F.h"
#include "TFile.h"

using namespace std;

Data::Data()
{}

Data::Data(const char* filename)
{
    rootFile = new TFile(filename);
    TH2F *hRaw = (TH2F*)rootFile->Get("F");
    wf = new Waveforms(hRaw);
}

Data::~Data()
{
    delete rootFile;
}
