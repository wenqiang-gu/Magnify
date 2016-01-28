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

    data = new Data("../data/dummy.root");
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
    vw->can->Connect(
        "ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
        "GuiController",
        this,
        "ProcessCanvasEvent(Int_t,Int_t,Int_t,TObject*)"
    );
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