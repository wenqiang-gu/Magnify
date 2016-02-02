#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class TGTextButton;
class TGNumberEntry;
class TGButtonGroup;
class TGRadioButton;
class TGListBox;
class TGCheckButton;
class TGLabel;

class ControlWindow: public TGHorizontalFrame
{
public:
    ControlWindow(const TGWindow *p, int w, int h);
    virtual ~ControlWindow();

    TGNumberEntry *channelEntry;
    TGNumberEntry *threshEntry[3];
    TGNumberEntry *zAxisRangeEntry[2];

    ClassDef(ControlWindow, 0)
};

#endif
