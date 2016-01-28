#include "ControlWindow.h"

#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGButtonGroup.h"
#include "TGListBox.h"
#include "TGLabel.h"
#include "TGDoubleSlider.h"

#include <iostream>
using namespace std;

ControlWindow::ControlWindow(const TGWindow *p, int w, int h)
    :TGHorizontalFrame(p, w, h)
{

    // zoom control button frame
    fZoomControlFrame = new TGGroupFrame(this, "Display Options", kVerticalFrame);
    fZoomControlFrame->SetTitlePos(TGGroupFrame::kLeft);
    AddFrame(fZoomControlFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 1, 1));

    fZoomButtonFrame = new TGHorizontalFrame(fZoomControlFrame, w, 100);
    fZoomControlFrame->AddFrame(fZoomButtonFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    autoZoomButton = new TGTextButton(fZoomButtonFrame, "Auto Zoom");
    fZoomButtonFrame->AddFrame(autoZoomButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    unZoomButton = new TGTextButton(fZoomButtonFrame, "UnZoom");
    fZoomButtonFrame->AddFrame(unZoomButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

}

ControlWindow::~ControlWindow()
{
}
