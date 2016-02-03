#include "Waveforms.h"

#include "TH2F.h"
#include "TH1F.h"
#include "TBox.h"
#include "TLine.h"
#include "TColor.h"
#include "TDirectory.h"
#include "TString.h"
#include "TPad.h"
#include "TPaletteAxis.h"
#include "TMath.h"

#include <iostream>
#include <vector>
using namespace std;

Waveforms::Waveforms()
{}

Waveforms::Waveforms(TH2F *h, vector<int>* v, TString name, TString title, double scale)
{
    // 2D hist: x: channel id; y: tdc 0 - 9600 or 0 - 9600/4
    hOrig = h;
    bad_channels = v;
    fScale = scale;
    nChannels = hOrig->GetNbinsX();
    nTDCs = hOrig->GetNbinsY();
    firstChannel = hOrig->GetXaxis()->GetBinCenter(1);
    fName = (name == "" ? hOrig->GetName() : name.Data());
    fTitle = (title == "" ? hOrig->GetTitle() : title.Data());

    const int DUMMY_NBINS = 100;
    hDummy = new TH2F(
        TString::Format("hDummy_%s", fName.Data()),
        fTitle,
        DUMMY_NBINS, hOrig->GetXaxis()->GetBinLowEdge(0), hOrig->GetXaxis()->GetBinUpEdge(nChannels),
        DUMMY_NBINS, hOrig->GetYaxis()->GetBinLowEdge(0), hOrig->GetYaxis()->GetBinUpEdge(nTDCs)
    );
    hDummy->SetXTitle("channel");
    hDummy->SetYTitle("ticks");

    int size = bad_channels->size();
    TLine *line = 0;
    for (int i=0; i<size; i++) {
        int channel = bad_channels->at(i);
        if (channel>=firstChannel && channel<firstChannel+nChannels) {
            line = new TLine(
                channel,
                hOrig->GetYaxis()->GetBinLowEdge(0),
                channel,
                hOrig->GetYaxis()->GetBinUpEdge(nTDCs)
            );
            line->SetLineColorAlpha(kGray, 0.5);
            lines.push_back(line);
        }
    }

    SetZRange(10, 20);
    SetThreshold(10);
}

Waveforms::~Waveforms()
{
    Clear();
}

void Waveforms::Clear()
{
    int size = boxes.size();
    // cout << "deleting " << size <<  " boxes ... " << flush;
    // I'm going to let memory leak as deleting the boxes is so slow!
    // for (int i=0; i<size; i++) {
    //     delete boxes[i];
    //     boxes[i] = 0;
    // }
    boxes.clear();
    box_values.clear();
    // cout << " finished. " << endl;

    // lines.clear();
}

void Waveforms::SetZRange(int min, int max)
{
    zmin = min;
    zmax = max;
}

void Waveforms::SetThreshold(double x)
{
    Clear();
    threshold = x;
    TBox *box = 0;
    cout << fName << ": creating boxes ... " << flush;
    for (int i=1; i<=nChannels; i++) {
        for (int j=1; j<=nTDCs; j++) {
            double content = hOrig->GetBinContent(i, j) * fScale;
            if (TMath::Abs(content)>threshold) {
                box = new TBox(
                    hOrig->GetXaxis()->GetBinLowEdge(i),
                    hOrig->GetYaxis()->GetBinLowEdge(j),
                    hOrig->GetXaxis()->GetBinUpEdge(i),
                    hOrig->GetYaxis()->GetBinUpEdge(j)
                );
                box->SetFillColor(kRed);
                boxes.push_back(box);
                box_values.push_back(TMath::Abs(content));
            }
        }
    }
    cout << boxes.size() <<  " created. " << endl;
}

void Waveforms::DrawLines()
{
    // cout << lines.size() << " lines" << endl;
    for (size_t i=0; i<lines.size(); i++) {
        lines[i]->Draw();
    }
}

void Waveforms::HideLines()
{
    // cout << lines.size() << " lines" << endl;
    for (size_t i=0; i<lines.size(); i++) {
        gPad->GetListOfPrimitives()->Remove(lines[i]);
    }
}

void Waveforms::Draw2D()
{
    hDummy->SetBinContent(1, 1);
    hDummy->Draw("colz");
    // hDummy->GetZaxis()->SetRangeUser(-1, 30);
    hDummy->GetZaxis()->SetRangeUser(zmin, zmax);
    gPad->Update();

    TPaletteAxis *palette = (TPaletteAxis*)hDummy->GetListOfFunctions()->FindObject("palette");
    int size = boxes.size();
    for (int i=0; i<size; i++) {
        boxes[i]->SetFillColor(palette->GetValueColor(box_values[i]));
        boxes[i]->Draw();
    }
}

TH1F* Waveforms::Draw1D(int chanNo, const char* options)
{
    TString name = TString::Format("hWire_%s", fName.Data());
    TString title = TString::Format("Channel %i", chanNo);

    TH1F *hWire = (TH1F*)gDirectory->FindObject(name);
    if (hWire) delete hWire;

    hWire = new TH1F(name, title.Data(),
        nTDCs,
        hOrig->GetYaxis()->GetBinLowEdge(0),
        hOrig->GetYaxis()->GetBinUpEdge(nTDCs)
    );
    int binNo = hOrig->GetXaxis()->FindBin(chanNo);
    for (int i=1; i<=nTDCs; i++) {
        hWire->SetBinContent(i, hOrig->GetBinContent(binNo, i)*fScale);
    }
    hWire->Draw(options);
    hWire->GetXaxis()->SetTitle("ticks");

    return hWire;
}
