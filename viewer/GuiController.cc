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
#include "TBox.h"

#include <iostream>
using namespace std;


GuiController::GuiController(const TGWindow *p, int w, int h, const char* filename)
{
    mw = new MainWindow(p, w, h);
    vw = mw->fViewWindow;
    cw = mw->fControlWindow;

    data = new Data("../data/dummy.root");
    vw->can->cd(1);
    data->wf->Draw2D();

    TH2F *hh = new TH2F("h", "h", 10, 0, 3800, 10, 0, 9600);
    for (int i=2; i<=9; i++) {
        vw->can->cd(i);
        hh->Draw("colz");
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
        double xx = pad->AbsPixeltoX(x);
        double yy = pad->AbsPixeltoY(y);
        cout << xx << ", " << yy << endl;
        vw->can->cd(7);
        data->wf->Draw1D( TMath::Nint(xx) ); // round
        vw->can->GetPad(7)->Modified();
        vw->can->GetPad(7)->Update();
    }

}