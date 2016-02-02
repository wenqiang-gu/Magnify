#include "Data.h"
#include "Waveforms.h"

#include "TH2F.h"
#include "TFile.h"

#include <vector>
using namespace std;

Data::Data()
{}

Data::Data(const char* filename)
{
    rootFile = new TFile(filename);

    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hu_raw")) );
    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hv_raw")) );
    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hw_raw")) );

    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hu_decon"),"","",1./500) );
    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hv_decon"),"","",1./500) );
    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hw_decon"),"","",1./500) );
}

Data::~Data()
{
    delete rootFile;
}
