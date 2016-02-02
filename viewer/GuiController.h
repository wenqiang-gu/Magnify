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

    void SyncTimeAxis0() { SyncTimeAxis(0); }
    void SyncTimeAxis1() { SyncTimeAxis(1); }
    void SyncTimeAxis2() { SyncTimeAxis(2); }
    void SyncTimeAxis3() { SyncTimeAxis(3); }
    void SyncTimeAxis4() { SyncTimeAxis(4); }
    void SyncTimeAxis5() { SyncTimeAxis(5); }
    void SyncTimeAxis(int i);

    MainWindow *mw;
    ViewWindow *vw;
    ControlWindow *cw;
    Data *data;

};

#endif
