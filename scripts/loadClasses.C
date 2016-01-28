{
    TString include = ".include ";
    TString load = ".L ";
//    gSystem->Load("libGLEW");
//    gSystem->Load("libFTGL");

    TString prefix;
    prefix = "../event";
    gROOT->ProcessLine( include + prefix );
    gROOT->ProcessLine( load + prefix + "/Waveforms.cc+" );
    gROOT->ProcessLine( load + prefix + "/Data.cc+" );
    // gROOT->ProcessLine( load + prefix + "/MCEvent.cc+" );

    prefix = "../viewer";
    gROOT->ProcessLine( include + prefix );
    gROOT->ProcessLine( load + prefix + "/ViewWindow.cc+" );
    gROOT->ProcessLine( load + prefix + "/ControlWindow.cc+" );
    gROOT->ProcessLine( load + prefix + "/MainWindow.cc+" );
    gROOT->ProcessLine( load + prefix + "/GuiController.cc+" );

}
