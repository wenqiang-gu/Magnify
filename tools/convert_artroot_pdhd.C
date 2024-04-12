#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "TFile.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include "TROOT.h"

using namespace art;
using namespace std;
using namespace std::chrono;

void
convert_artroot_pdhd(std::string const& filename)
{
  InputTag rawdigits_tag{ "tpcrawdecoder:daq" };
  InputTag wires_tag{ "wclsdatahd:gauss" };

  // Create a vector of length 1, containing the given filename.
  vector<string> filenames(1, filename);

  // Don't do the following in compiled C++. This code relies on the
  // interactive ROOT system to (implicitly) own the histograms we
  // create on the heap. In a C++ program, this object would be leaked
  // (the memory unrecoverable by the program).

  const int nticks = 6000;
  const int nchans = 10240;
  auto h_baseline = new TH1I("h_baseline", "Pedestal", nchans, -0.5, nchans-0.5);
  auto h_orig = new TH2F("h_orig", "RawDigits", 3072, -0.5, 3071.5, nticks, 0, nticks);
  auto h_decon = new TH2F("h_decon", "Deconvolution", 3072, -0.5, 3071.5, nticks, 0, nticks);

  int n = 0;
  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {

    if (n>0) break;
    // std::cout << "processing event: " << n << std::endl;
    auto aux = ev.eventAuxiliary();
    std::cout << "processing event: " << aux.run() << "-" << aux.event() << std::endl;

    auto const& rawdigits = *ev.getValidHandle<vector<raw::RawDigit>>(rawdigits_tag);
    auto const& wires = *ev.getValidHandle<vector<recob::Wire>>(wires_tag);

    if (rawdigits.empty()) {
      std::cout << "WARNING: no RawDigit found." << std::endl;
      return;
    }

    if (wires.empty()) {
      std::cout << "WARNING: no recob::Wire found." << std::endl;
      return;
    }

    for (auto rd: rawdigits) {
      // std::cout << "channel: " << rd.Channel()
      //           << " NADC: " << rd.NADC()
      //           << " Samples: " << rd.Samples()
      //           << " Pedestal: " << rd.GetPedestal()
      //           << " Sigma: " << rd.GetSigma()
      //           << std::endl;

      int channel = rd.Channel();
      // std::cout << "channel: " << channel << " threshold: " << thresholds.at(channel) << std::endl;
      h_baseline->SetBinContent(channel+1, rd.GetPedestal());

      int nSamples = rd.Samples();
      for (int j = 0; j < nSamples; j++) {
         h_orig->SetBinContent(channel+1, j+1, rd.ADC(j) - rd.GetPedestal());
      }
    } // end of rawdigits

    for (auto wire: wires) {
      int channel = wire.Channel();
      int nSamples = wire.NSignal();
      std::vector<float> signal = wire.Signal();
      for (int j = 0; j < nSamples; j++) {
        h_decon->SetBinContent(channel+1, j+1, signal.at(j));       
      }
    } // end of wires


    auto ofile = TFile::Open(Form("magnify-%d-%d.root", aux.run(), aux.event()),"recreate");
    h_orig->Write();
    h_baseline->Write();
    h_decon->Write();

    ofile->Close();
      
    n++;
  }

  // hsigma->Draw();

}
