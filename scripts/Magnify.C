void Magnify(const char* filename)
{
    // gROOT->Reset();
    // gROOT->ProcessLine(".x loadClasses.C" );

    GuiController *gc = new GuiController(
        gClient->GetRoot(),
        1600,
        900,
        filename
    );
}
