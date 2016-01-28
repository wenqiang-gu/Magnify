#ifndef DATA_H
#define DATA_H

class TH2F;
class TFile;
class Waveforms;

#include <vector>

class Data {
public:
    Data();
    Data(const char* filename);

    virtual ~Data();

    TFile *rootFile;
    vector<Waveforms*> wfs;

};

#endif