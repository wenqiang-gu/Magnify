#include "Waveforms.h"

#include "TH2F.h"
#include "TH1F.h"
#include "TBox.h"
#include "TColor.h"
#include "TDirectory.h"
#include "TString.h"

#include <iostream>
#include <vector>
using namespace std;

Waveforms::Waveforms()
{}

Waveforms::Waveforms(TH2F *h, TString name, TString title)
{
    // 2D hist: x: channel id; y: tdc 0 - 9600 or 0 - 9600/4
    hOrig = h;
    nChannels = hOrig->GetNbinsX();
    nTDCs = hOrig->GetNbinsY();
    fName = (name == "" ? hOrig->GetName() : name.Data());
    fTitle = (title == "" ? hOrig->GetTitle() : title.Data());

    const int DUMMY_NBINS = 100;
    hDummy = new TH2F(
        TString::Format("hDummy_%s", fName.Data()),
        fTitle,
        DUMMY_NBINS, hOrig->GetXaxis()->GetBinLowEdge(0), hOrig->GetXaxis()->GetBinUpEdge(nChannels),
        DUMMY_NBINS, hOrig->GetYaxis()->GetBinLowEdge(0), hOrig->GetYaxis()->GetBinUpEdge(nTDCs)
    );

    double thresh = 4;
    TBox *box = 0;
    for (int i=1; i<=nChannels; i++) {
        for (int j=1; j<=nTDCs; j++) {
            double content = hOrig->GetBinContent(i, j);
            if (content>thresh) {
                box = new TBox(
                    hOrig->GetXaxis()->GetBinLowEdge(i),
                    hOrig->GetYaxis()->GetBinLowEdge(j),
                    hOrig->GetXaxis()->GetBinUpEdge(i),
                    hOrig->GetYaxis()->GetBinUpEdge(j)
                );
                box->SetFillColor(kRed);
                hits.push_back(box);
            }
        }
    }

}

Waveforms::~Waveforms()
{
    int size = hits.size();
    for (int i=0; i<size; i++) {
        delete hits[i];
        hits[i] = 0;
    }
}

void Waveforms::Draw2D()
{
    hDummy->Draw("colz");
    int size = hits.size();
    for (int i=0; i<size; i++) {
        hits[i]->Draw();
    }
}

TH1F* Waveforms::Draw1D(int chanNo, const char* options)
{
    TString name = TString::Format("hWire_%s", fName.Data());
    TString title = TString::Format("Channle %i", chanNo);

    TH1F *hWire = (TH1F*)gDirectory->FindObject(name);
    if (hWire) delete hWire;

    hWire = new TH1F(name, title.Data(),
        nTDCs,
        hOrig->GetYaxis()->GetBinLowEdge(0),
        hOrig->GetYaxis()->GetBinUpEdge(nTDCs)
    );
    int binNo = hOrig->GetXaxis()->FindBin(chanNo);
    for (int i=1; i<=nTDCs; i++) {
        hWire->SetBinContent(i, hOrig->GetBinContent(binNo, i));
    }
    hWire->Draw(options);

    return hWire;
}
