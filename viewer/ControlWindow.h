#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class TGNumberEntry;
class TGCheckButton;

class ControlWindow: public TGHorizontalFrame
{
public:
    ControlWindow(const TGWindow *p, int w, int h);
    virtual ~ControlWindow();

    TGNumberEntry *channelEntry;
    TGNumberEntry *threshEntry[3];
    TGNumberEntry *zAxisRangeEntry[2];
    TGCheckButton *badChanelButton;


    ClassDef(ControlWindow, 0)
};

#endif
