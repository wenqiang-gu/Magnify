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
    Waveforms(TH2F *h, TString name="", TString title="", double scale=1);
    virtual ~Waveforms();

    void SetThreshold(double x);
    void SetZRange(int min, int max);
    void Draw2D();
    TH1F* Draw1D(int chanNo, const char* options="");
    void ClearBoxes();

    TH2F *hOrig;
    TH2F *hDummy;
    vector<TBox*> boxes;
    vector<float> box_values;

    int nChannels;  // nBinsX
    int nTDCs;       // nBinsY
    int firstChannel;
    double fScale;    // electron to
    TString fName;
    TString fTitle;
    double threshold;
    int zmin;
    int zmax;
};

#endif