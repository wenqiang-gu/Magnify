#include "Data.h"
#include "Waveforms.h"

#include "TH2F.h"
#include "TFile.h"

#include <vector>
#include <string>
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

    load_waveform("hu_raw");
    load_waveform("hv_raw");
    load_waveform("hw_raw");

    load_waveform("hu_decon","",1./500);
    load_waveform("hv_decon","",1./500);
    load_waveform("hw_decon","",1./500);
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
    wfs.push_back( new Waveforms(hist, name, title, scale) );
}

Data::~Data()
{
    delete rootFile;
}
