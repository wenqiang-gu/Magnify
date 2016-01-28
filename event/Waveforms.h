#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#include <vector>
#include "TString.h"

class TH2F;
class TH1F;
class TBox;

class Waveforms {
public:
    Waveforms();
    Waveforms(TH2F *h, TString name="", TString title="");
    virtual ~Waveforms();

    void Draw2D();
    TH1F* Draw1D(int chanNo, const char* options="");

    TH2F *hOrig;
    TH2F *hDummy;
    vector<TBox*> hits;

    int nChannels;  // nBinsX
    int nTDCs;       // nBinsY
    TString fName;
    TString fTitle;
};

#endif