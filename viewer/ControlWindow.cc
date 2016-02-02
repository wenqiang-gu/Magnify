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
    TGGroupFrame *group_general = new TGGroupFrame(this, "General", kHorizontalFrame);
    group_general->SetTitlePos(TGGroupFrame::kLeft);
    AddFrame(group_general, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

    group_general->AddFrame(new TGLabel(group_general, "channel: "), new TGLayoutHints(kLHintsTop | kLHintsLeft,  2, 2, 1, 1));
    channelEntry = new TGNumberEntry(group_general, 0, 5, -1,
        TGNumberFormat::kNESInteger,
        TGNumberFormat::kNEANonNegative,
        TGNumberFormat::kNELLimitMinMax,
        0, 8255);
    group_general->AddFrame(channelEntry, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    badChanelButton = new TGCheckButton(group_general, "show bad channel  ");
    badChanelButton->SetState(kButtonUp);
    group_general->AddFrame(badChanelButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    TGGroupFrame *group[3];
    for (int i=0; i<3; i++) {
        group[i] = new TGGroupFrame(this, "", kHorizontalFrame);
        group[i]->SetTitlePos(TGGroupFrame::kLeft);
        AddFrame(group[i], new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

        group[i]->AddFrame(new TGLabel(group[i], "threshold: "), new TGLayoutHints(kLHintsTop | kLHintsLeft,  2, 2, 1, 1));
        threshEntry[i] = new TGNumberEntry(group[i], 3, 5, -1,
            TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEANonNegative,
            TGNumberFormat::kNELLimitMinMax,
            3, 30);
        group[i]->AddFrame(threshEntry[i], new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));
    }
    group[0]->SetTitle("U Plane");
    group[1]->SetTitle("V Plane");
    group[2]->SetTitle("W Plane");

    TGGroupFrame *group_misc = new TGGroupFrame(this, "Misc", kHorizontalFrame);
    group_misc->SetTitlePos(TGGroupFrame::kLeft);
    AddFrame(group_misc, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

    group_misc->AddFrame(new TGLabel(group_misc, "color range: "), new TGLayoutHints(kLHintsTop | kLHintsLeft,  2, 2, 1, 1));
    for (int i=0; i<2; i++) {
        zAxisRangeEntry[i] = new TGNumberEntry(group_misc, 10, 5, -1,
            TGNumberFormat::kNESInteger,
            TGNumberFormat::kNEAAnyNumber,
            TGNumberFormat::kNELLimitMinMax,
            -100, 100);
        group_misc->AddFrame(zAxisRangeEntry[i], new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));
    }


}

ControlWindow::~ControlWindow()
{
}
