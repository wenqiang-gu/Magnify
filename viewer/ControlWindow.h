#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class TGTextButton;
class TGNumberEntryField;
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

    TGGroupFrame       *fZoomControlFrame;
    TGHorizontalFrame  *fZoomButtonFrame;
    TGTextButton       *autoZoomButton;
    TGTextButton       *unZoomButton;

    ClassDef(ControlWindow, 0)
};

#endif
