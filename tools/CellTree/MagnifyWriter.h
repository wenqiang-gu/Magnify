#ifndef MAGNIFYWRITER_H
#define MAGNIFYWRITER_H

#include <string>

class CellEvent;
class TFile;

class MagnifyWriter {
public:
    CellEvent *ev;
    std::string expName;
    TFile *outputFile;

    int nTPC;
    int tpcNo; // process one TPC at a time
    vector<int> channel_starts; // starting (ending) channel no on each plane
    vector<int> channel_ends;
    int nTDC;


    //-------------------------------------
    MagnifyWriter();
    MagnifyWriter(std::string filename, std::string outname, std::string experiment, int tpc=0);
    virtual ~MagnifyWriter();

    void SetupIO(std::string filename, std::string outname);
    void SetupExperiment(std::string experiment, int tpc=0);
    void PrintInfo();
    void Write(int entry);
    void WriteHist(std::string name, int planeIndex);

};

#endif