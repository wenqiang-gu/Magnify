#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "TGFrame.h"
#include "RQ_OBJECT.h"

class MainWindow;
class ViewWindow;
class ControlWindow;
class Data;


class GuiController
{
    RQ_OBJECT("GuiController")
public:
    GuiController(const TGWindow *p, int w, int h, const char* filename=0);
    virtual ~GuiController();

    void InitConnections();
    void HandleMenu(int id);
    void ProcessCanvasEvent(Int_t ev, Int_t x, Int_t y, TObject *selected);
    void ThresholdUChanged() { ThresholdChanged(0); }
    void ThresholdVChanged() { ThresholdChanged(1); }
    void ThresholdWChanged() { ThresholdChanged(2); }
    void ThresholdChanged(int i);
    void ZRangeChanged();
    void ChannelChanged();

    MainWindow *mw;
    ViewWindow *vw;
    ControlWindow *cw;
    Data *data;

};

#endif
