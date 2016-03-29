#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#include <vector>
#include "TString.h"

class TH2F;
class TH1F;
class TH1I;
class TBox;
class TLine;

class Waveforms {
public:
    Waveforms();
    Waveforms(TH2F *h, vector<int>* v, TString name="", TString title="", double scale=1);
    virtual ~Waveforms();

    void SetThreshold(double x);
    void SetThreshold(TH1I* h);

    void SetZRange(int min, int max);
    void Draw2D();
    TH1F* Draw1D(int chanNo, const char* options="");
    void Clear();
    void DrawLines();
    void HideLines();

    TH2F *hOrig;
    TH2F *hDummy;
    vector<TBox*> boxes;
    vector<float> box_values;
    vector<int>* bad_channels;
    vector<TLine*> lines;

    int nChannels;  // nBinsX
    int nTDCs;       // nBinsY
    int firstChannel;
    double fScale;    // electron to
    TString fName;
    TString fTitle;
    double threshold;
    bool useChannelThreshold;
    int zmin;
    int zmax;
};

#endif