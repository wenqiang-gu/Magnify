#ifndef DATA_H
#define DATA_H

class TH2F;
class TH1I;
class TFile;
class Waveforms;
class RawWaveforms;

#include <vector>

class Data {
public:
    Data();
    Data(const char* filename);

    virtual ~Data();

    TFile *rootFile;
    vector<Waveforms*> wfs;
    vector<int> bad_channels;
    vector<RawWaveforms*> raw_wfs;
    vector<TH1I*> thresh_histos;
    int runNo;
    int subRunNo;
    int eventNo;

private:
    void load_waveform(const char* name, const char* title="", double scale=1);
    void load_badchannels();
    void load_runinfo();
};

#endif
