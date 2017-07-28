#include "Data.h"
#include "Waveforms.h"
#include "RawWaveforms.h"
#include "BadChannels.h"

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

Data::Data(const char* filename, const char* frame)
{
    rootFile = TFile::Open(filename);
    if (!rootFile) {
    	string msg = "Unable to open ";
    	msg += filename;
    	throw runtime_error(msg.c_str());
    }
    bad_channels = new BadChannels( (TTree*)rootFile->Get("T_bad") );
    // load_badchannels();
    load_runinfo();

    load_waveform("hu_raw", "U Plane (Denoised)");
    load_waveform("hv_raw", "V Plane (Denoised)");
    load_waveform("hw_raw", "W Plane (Denoised)");

    for (int iplane=0; iplane<3; ++iplane) {
        load_waveform(Form("h%c_%s", 'u'+iplane, frame),
                      Form("%c Plane (Deconvoluted)", 'U'+iplane), 1./500);
    }

    load_rawwaveform("hu_orig", "hu_baseline");
    load_rawwaveform("hv_orig", "hv_baseline");
    load_rawwaveform("hw_orig", "hw_baseline");

    load_threshold("hu_threshold");
    load_threshold("hv_threshold");
    load_threshold("hw_threshold");
}

void Data::load_runinfo()
{
    TTree *t = (TTree*)rootFile->Get("Trun");
    if (t) {
        t->SetBranchAddress("runNo", &runNo);
        t->SetBranchAddress("subRunNo", &subRunNo);
        t->SetBranchAddress("eventNo", &eventNo);
        t->GetEntry(0);
    }
    else {
        runNo = 0;
        subRunNo = 0;
        eventNo = 0;
    }
}

void Data::load_waveform(const char* name, const char* title, double scale)
{
    TObject* obj = rootFile->Get(name);
    if (!obj) {
    	string msg = "Failed to get waveform ";
    	msg += name;
        msg += ", create dummy ...";
        cout << msg << endl;
    	// throw runtime_error(msg.c_str());
        obj = new TH2F(name, title, 4000,0,4000,9600,0,9600);
    }
    TH2F* hist = dynamic_cast<TH2F*>(obj);
    if (!hist) {
    	string msg = "Not a TH2F: ";
    	msg += name;
    	throw runtime_error(msg.c_str());
    }
    hist->SetXTitle("channel");
    hist->SetYTitle("ticks");
    wfs.push_back( new Waveforms(hist, bad_channels, name, title, scale) );
}

void Data::load_rawwaveform(const char* name, const char* baseline_name)
{
    TObject* obj = rootFile->Get(name);
    if (!obj) {
        string msg = "Failed to get waveform ";
        msg += name;
        msg += ", create dummy ...";
        cout << msg << endl;
        // throw runtime_error(msg.c_str());
        obj = new TH2I(name, "", 4000,0,4000,9600,0,9600);
    }
    TH2I* hist = dynamic_cast<TH2I*>(obj);
    if (!hist) {
        string msg = "Not a TH2I: ";
        msg += name;
        throw runtime_error(msg.c_str());
    }
    hist->SetXTitle("channel");
    hist->SetYTitle("ticks");

    TObject* obj2 = rootFile->Get(baseline_name);
    if (!obj2) {
        string msg = "Failed to get baseline ";
        msg += baseline_name;
        msg += ", create dummy ...";
        // throw runtime_error(msg.c_str());
        obj2 = new TH1I(baseline_name, "", hist->GetNbinsX(),0,hist->GetNbinsX());
    }
    TH1I* hist2 = dynamic_cast<TH1I*>(obj2);
    if (!hist2) {
        string msg = "Not a TH1I: ";
        msg += name;
        throw runtime_error(msg.c_str());
    }

    raw_wfs.push_back( new RawWaveforms(hist, hist2) );
}

void Data::load_threshold(const char* name)
{
    TObject* obj = rootFile->Get(name);
    if (!obj) {
        string msg = "Failed to get threshold ";
        msg += name;
        msg += ", create dummy ...";
        // throw runtime_error(msg.c_str());
        obj = new TH1I(name, "", 4000,0,4000);
    }
    thresh_hist_t* hist = dynamic_cast<thresh_hist_t*>(obj);
    if (!hist) {
        string msg = "Threshold histogram is the wrong type: ";
        msg += name;
        throw runtime_error(msg.c_str());
    }

    thresh_histos.push_back( hist );
}

Data::~Data()
{
    delete rootFile;
}
