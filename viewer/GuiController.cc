#include "GuiController.h"
#include "MainWindow.h"
#include "ViewWindow.h"
#include "ControlWindow.h"
#include "Data.h"
#include "Waveforms.h"

#include "TApplication.h"
#include "TSystem.h"
#include "TExec.h"
#include "TROOT.h"
#include "TMath.h"

#include "TGMenu.h"
#include "TGNumberEntry.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TBox.h"
#include "TColor.h"

#include <iostream>
#include <vector>
using namespace std;


GuiController::GuiController(const TGWindow *p, int w, int h, const char* filename)
{
    mw = new MainWindow(p, w, h);
    vw = mw->fViewWindow;
    cw = mw->fControlWindow;

    // data = new Data("../data/2D_display_3455_0_0.root");
    data = new Data(filename);
    for (int i=0; i<6; i++) {
        vw->can->cd(i+1);
        data->wfs.at(i)->Draw2D();
    }

    InitConnections();
}

GuiController::~GuiController()
{
    // gApplication->Terminate(0);
}

void GuiController::InitConnections()
{
    mw->fMenuFile->Connect("Activated(int)", "GuiController", this, "HandleMenu(int)");

    for (int i=0; i<3; i++) {
        cw->threshEntry[i]->SetNumber(data->wfs.at(i)->threshold);
    }
    cw->threshEntry[0]->Connect("ValueSet(Long_t)", "GuiController", this, "ThresholdUChanged()");
    cw->threshEntry[1]->Connect("ValueSet(Long_t)", "GuiController", this, "ThresholdVChanged()");
    cw->threshEntry[2]->Connect("ValueSet(Long_t)", "GuiController", this, "ThresholdWChanged()");

    cw->zAxisRangeEntry[0]->SetNumber(data->wfs.at(0)->zmin);
    cw->zAxisRangeEntry[1]->SetNumber(data->wfs.at(0)->zmax);
    cw->zAxisRangeEntry[0]->Connect("ValueSet(Long_t)", "GuiController", this, "ZRangeChanged()");
    cw->zAxisRangeEntry[1]->Connect("ValueSet(Long_t)", "GuiController", this, "ZRangeChanged()");



    vw->can->Connect(
        "ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
        "GuiController",
        this,
        "ProcessCanvasEvent(Int_t,Int_t,Int_t,TObject*)"
    );
}

void GuiController::ThresholdChanged(int i)
{
    int newThresh = cw->threshEntry[i]->GetNumber();
    cout << "new threshold: " << newThresh << endl;
    for (int ind=i; ind<6; ind+=3) {
        vw->can->cd(ind+1);
        data->wfs.at(ind)->SetThreshold(newThresh);
        data->wfs.at(ind)->Draw2D();
        vw->can->GetPad(ind+1)->Modified();
        vw->can->GetPad(ind+1)->Update();
    }
}

void GuiController::ZRangeChanged()
{
    int min = cw->zAxisRangeEntry[0]->GetNumber();
    int max = cw->zAxisRangeEntry[1]->GetNumber();
    for (int ind=0; ind<6; ind++) {
        vw->can->cd(ind+1);
        data->wfs.at(ind)->SetZRange(min, max);
        data->wfs.at(ind)->Draw2D();
        vw->can->GetPad(ind+1)->Modified();
        vw->can->GetPad(ind+1)->Update();
    }

}

void GuiController::ProcessCanvasEvent(Int_t ev, Int_t x, Int_t y, TObject *selected)
{
    if (ev == 11) { // clicked
        if (!(selected->IsA() == TH2F::Class() || selected->IsA() == TBox::Class())) return;
        TVirtualPad* pad = vw->can->GetClickSelectedPad();
        int padNo = pad->GetNumber();
        double xx = pad->AbsPixeltoX(x);
        double yy = pad->AbsPixeltoY(y);
        cout << "pad " << padNo << ": (" << xx << ", " << yy << ")" << endl;

        int drawPad = (padNo-1) % 3 + 7;
        vw->can->cd(drawPad);
        if (padNo<=6) {
            int wfNo = padNo - 1;
            wfNo = wfNo < 3 ? wfNo : wfNo-3;  // draw raw first
            int chanNo = TMath::Nint(xx); // round
            data->wfs.at(wfNo)->Draw1D(chanNo);
            TH1F *h = data->wfs.at(wfNo+3)->Draw1D(chanNo, "same" ); // draw calib
            h->SetLineColor(kRed);
        }
        vw->can->GetPad(drawPad)->Modified();
        vw->can->GetPad(drawPad)->Update();
    }

}

void GuiController::HandleMenu(int id)
{
    // const char *filetypes[] = {"ROOT files", "*.root", 0, 0};
    switch (id) {
        case M_FILE_EXIT:
            gApplication->Terminate(0);
            break;
    }
}