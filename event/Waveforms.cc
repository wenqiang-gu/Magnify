#include "Waveforms.h"

#include "TH2F.h"
#include "TBox.h"
#include "TColor.h"
#include "TDirectory.h"

#include <iostream>
#include <vector>
using namespace std;

Waveforms::Waveforms()
{}

Waveforms::Waveforms(TH2F *h)
{
    hOrig = h;
    // cout << hOrig->GetName() << endl;
    nBinsX = hOrig->GetNbinsX();
    nBinsY = hOrig->GetNbinsY();

    const int DUMMY_NBINS = 100;
    hDummy = new TH2F("hDummy", "", DUMMY_NBINS, 0, nBinsX, DUMMY_NBINS, 0, nBinsY);

    double thresh = 4;
    TBox *box = 0;
    for (int i=1; i<=nBinsX; i++) {
        for (int j=1; j<=nBinsY; j++) {
            double content = hOrig->GetBinContent(i, j);
            if (content>thresh) {
                box = new TBox(i-0.5, j-0.5, i+0.5, j+0.5);
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

void Waveforms::Draw1D(int wireNo)
{
    TH1F *hWire = (TH1F*)gDirectory->FindObject("hWire");
    if (hWire) delete hWire;

    hWire = new TH1F("hWire", "Wire No.", nBinsY, 0, nBinsY);
    for (int i=1; i<=nBinsY; i++) {
        hWire->SetBinContent(i, hOrig->GetBinContent(wireNo, i));
    }
    hWire->Draw();
}
