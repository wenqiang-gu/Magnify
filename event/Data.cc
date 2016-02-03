#include "Data.h"
#include "Waveforms.h"
#include "RawWaveforms.h"

#include "TH2F.h"
#include "TH2I.h"
#include "TH1I.h"
#include "TFile.h"
#include "TTree.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;


Data::Data()
{}

Data::Data(const char* filename)
{
    rootFile = TFile::Open(filename);
    if (!rootFile) {
    	string msg = "Unable to open ";
    	msg += filename;
    	throw runtime_error(msg.c_str());
    }

    load_badchannels();

    load_waveform("hu_raw", "U Plane (Denoised)");
    load_waveform("hv_raw", "V Plane (Denoised)");
    load_waveform("hw_raw", "W Plane (Denoised)");

    load_waveform("hu_decon", "U Plane (Deconvoluted)", 1./500);
    load_waveform("hv_decon", "V Plane (Deconvoluted)", 1./500);
    load_waveform("hw_decon", "W Plane (Deconvoluted)", 1./500);

    raw_wfs.push_back( new RawWaveforms((TH2I*)rootFile->Get("hu_orig"), (TH1I*)rootFile->Get("hu_baseline")) );
    raw_wfs.push_back( new RawWaveforms((TH2I*)rootFile->Get("hv_orig"), (TH1I*)rootFile->Get("hv_baseline")) );
    raw_wfs.push_back( new RawWaveforms((TH2I*)rootFile->Get("hw_orig"), (TH1I*)rootFile->Get("hw_baseline")) );

}

void Data::load_badchannels()
{
    TTree *t = (TTree*)rootFile->Get("T_bad");
    if (t) {
        int chid;
        t->SetBranchAddress("chid", &chid);
        int nEntries = t->GetEntries();
        for (int i=0; i<nEntries; i++) {
            t->GetEntry(i);
            bad_channels.push_back(chid);
            // cout << chid << endl;
        }
    }
}

void Data::load_waveform(const char* name, const char* title, double scale)
{
    TObject* obj = rootFile->Get(name);
    if (!obj) {
    	string msg = "Failed to get waveform ";
    	msg += name;
    	throw runtime_error(msg.c_str());
    }
    TH2F* hist = dynamic_cast<TH2F*>(obj);
    if (!hist) {
    	string msg = "Not a TH2F: ";
    	msg += name;
    	throw runtime_error(msg.c_str());
    }
    hist->SetXTitle("channel");
    hist->SetYTitle("ticks");
    wfs.push_back( new Waveforms(hist, &bad_channels, name, title, scale) );
}


Data::~Data()
{
    delete rootFile;
}
