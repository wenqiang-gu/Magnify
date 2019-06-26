#include "MagnifyWriter.h"
#include "CellEvent.h"

#include <iostream>
#include <vector>
#include <iomanip>

#include "TNamed.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TH2F.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TObject.h"

using namespace std;

MagnifyWriter::MagnifyWriter(){}

MagnifyWriter::MagnifyWriter(string filename)
{
	SetupIO(filename);
}

MagnifyWriter::MagnifyWriter(string filename, string experiment, int tpc)
{
	SetupIO(filename);	
	SetupExperiment(experiment, tpc);
}

MagnifyWriter::~MagnifyWriter()
{
	outputFile->Close();
}

void MagnifyWriter::SetupIO(string filename) {
	ev = new CellEvent(filename.c_str());
	outputFile = new TFile("magnify.root", "recreate");
}

void MagnifyWriter::SetupExperiment(string experiment, int tpc)
{
	tpcNo = tpc;

	channel_starts.clear();
	channel_ends.clear();

	if (experiment == "uboone" || experiment == "icarus") {
		expName = experiment;
		cout << "configuring " << expName << " ..."<< endl;
	}
	else {
		cout << experiment << " not supported (only uboone or icarus is supported)." << endl;
	}

	if (expName == "uboone") {
		nTPC = 1;
		int starts[] = {0, 2400, 4800};
		int ends[] = {2399, 4799, 8255};
		for (int i=0; i<nTPC*3; i++) {
			channel_starts.push_back(starts[i]);
			channel_ends.push_back(ends[i]);
		}
		nTDC = 9592;
	}
	else if (expName == "icarus") {
		nTPC = 4;
		int starts[] = {0, 1056, 6656, 12256, 13312, 18912, 24512, 25568, 31168, 36768, 37824, 43424};
		int ends[] = {1055, 6655, 12255, 13311, 18911, 24511, 25567, 31167, 36767, 37823, 43423, 49023};
		for (int i=0; i<nTPC*3; i++) {
			channel_starts.push_back(starts[i]);
			channel_ends.push_back(ends[i]);
		}
		nTDC = 4096;
	}
}

void MagnifyWriter::PrintInfo()
{
	cout << "Experiment: " << expName << endl;
	cout << "channel starts: ";
	for (int i=0; i<nTPC*3; i++) {
		cout << channel_starts[i] << " ";
	}
	cout << endl;
	cout << "channel ends: ";
	for (int i=0; i<nTPC*3; i++) {
		cout << channel_ends[i] << " ";
	}
	cout << endl;
	cout << "Converting TPC " << tpcNo << endl;
	cout << "Total TDC: " << nTDC << endl;
}

void MagnifyWriter::Write(int entry) {
	cout << "Getting entry " << entry << " ..." << endl;
	ev->GetEntry(0);

	TTree *Trun = new TTree("Trun", "Trun");
	Trun->Branch("eventNo", &(ev->eventNo));
	Trun->Branch("runNo", &(ev->runNo));
	Trun->Branch("subRunNo", &(ev->subRunNo));
	Trun->Fill();
	Trun->Write(0, TObject::kOverwrite);

	WriteHist("hu", tpcNo*3);
	WriteHist("hv", tpcNo*3+1);
	WriteHist("hw", tpcNo*3+2);

}

void MagnifyWriter::WriteHist(string name, int planeIndex)
{
	cout << "Writing " << name << " ..." << endl;
	float startX = channel_starts[planeIndex]-0.5;
	int nBinsX = channel_ends[planeIndex] - channel_starts[planeIndex]+1;
	// TH2I *h2 = new TH2I(name.c_str(), name.c_str(), nBinsX, startX, startX+nBinsX, nTDC, 0, nTDC);
	TH2I *h_orig = new TH2I((name+"_orig").c_str(), (name+"_orig").c_str(), nBinsX, startX, startX+nBinsX, nTDC, 0, nTDC);
	TH1I *h_baseline = new TH1I((name+"_baseline").c_str(), (name+"_baseline").c_str(), nBinsX, startX, startX+nBinsX);
	TH2F *h_raw = new TH2F((name+"_raw").c_str(), (name+"_raw").c_str(), nBinsX, startX, startX+nBinsX, nTDC, 0, nTDC);
	TH2F *h_decon = new TH2F((name+"_decon").c_str(), (name+"_decon").c_str(), nBinsX, startX, startX+nBinsX, nTDC, 0, nTDC);

	TH1I hf("hf","Bin Frequency",4096,0,4096); //12-bit ADC? for baseline calculation
	int size = ev->raw_channelId->size();
	for (int i=0; i<size; i++) {
		int chan = ev->raw_channelId->at(i);
		if (chan>=startX && chan <startX+nBinsX) {
			TH1F* h = (TH1F*)ev->raw_wf->At(i);
			int binX = h_orig->GetXaxis()->FindBin(chan);
			hf.Reset();
			for (int j=1; j<=nTDC; j++) {
				int content = h->GetBinContent(j);
				h_orig->SetBinContent(binX, j, content);
				hf.Fill(content);
			}
			int baseline = hf.GetMaximumBin()-1;
		    h_baseline->SetBinContent(binX, baseline);
		    for (int j=1; j<=nTDC; j++) {
		    	int content = h->GetBinContent(j);
		    	h_raw->SetBinContent(binX, j, content-baseline);
		    	if (content>baseline+1) {
		    		h_decon->SetBinContent(binX, j, content-baseline);
		    	}
		    }
		}
	}
	h_orig->Write();
	h_baseline->Write();
	h_raw->Write();
	h_decon->Write();
}

