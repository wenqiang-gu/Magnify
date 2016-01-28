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

    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hU")) );
    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hV")) );
    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hW")) );

    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hUD")) );
    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hVD")) );
    wfs.push_back( new Waveforms((TH2F*)rootFile->Get("hWD")) );
}

Data::~Data()
{
    delete rootFile;
}
