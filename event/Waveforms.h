#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#include <vector>

class TH2F;
class TBox;

class Waveforms {
public:
    Waveforms();
    Waveforms(TH2F *h);
    virtual ~Waveforms();

    void Draw2D();
    void Draw1D(int wireNo);

    TH2F *hOrig;
    TH2F *hDummy;
    vector<TBox*> hits;

    int nBinsX;
    int nBinsY;
};

#endif