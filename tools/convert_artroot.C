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
convert_artroot(std::string const& filename)
{
  InputTag rawdigits_tag{ "caldata:dataprep" };
  InputTag wires_tag{ "caldata:dataprep" };
  InputTag decons_tag{ "wclsdatanfsp:gauss" };
  InputTag thres_tag{ "wclsdatanfsp:threshold" };

  // Create a vector of length 1, containing the given filename.
  vector<string> filenames(1, filename);

  // Don't do the following in compiled C++. This code relies on the
  // interactive ROOT system to (implicitly) own the histograms we
  // create on the heap. In a C++ program, this object would be leaked
  // (the memory unrecoverable by the program).

  auto hsigma = new TH1F("hsigma", "", 3072, -0.5, 3072-0.5);

  const int nticks = 2048;
  auto hu_orig = new TH2I("hu_orig", "hu_orig", 952, -0.5, 951.5, nticks, 0, nticks);
  auto hv_orig = new TH2I("hv_orig", "hu_orig", 952, 951.5, 1903.5, nticks, 0, nticks);
  auto hw_orig = new TH2I("hw_orig", "hw_orig", 1168, 1903.5, 3071.5, nticks, 0, nticks);

  auto hu_baseline = new TH1I("hu_baseline", "hu_baseline", 952, -0.5, 951.5);
  auto hv_baseline = new TH1I("hv_baseline", "hv_baseline", 952, 951.5, 1903.5);
  auto hw_baseline = new TH1I("hw_baseline", "hw_baseline", 1168, 1903.5, 3071.5);

  auto hu_raw = new TH2F("hu_raw", "hu_raw", 952, -0.5, 951.5, nticks, 0, nticks);
  auto hv_raw = new TH2F("hv_raw", "hu_raw", 952, 951.5, 1903.5, nticks, 0, nticks);
  auto hw_raw = new TH2F("hw_raw", "hw_raw", 1168, 1903.5, 3071.5, nticks, 0, nticks);

  auto hu_decon = new TH2F("hu_decon", "hu_decon", 952, -0.5, 951.5, nticks, 0, nticks);
  auto hv_decon = new TH2F("hv_decon", "hu_decon", 952, 951.5, 1903.5, nticks, 0, nticks);
  auto hw_decon = new TH2F("hw_decon", "hw_decon", 1168, 1903.5, 3071.5, nticks, 0, nticks);

  auto hu_threshold = new TH1F("hu_threshold", "hu_threshold", 952, -0.5, 951.5);
  auto hv_threshold = new TH1F("hv_threshold", "hv_threshold", 952, 951.5, 1903.5);
  auto hw_threshold = new TH1F("hw_threshold", "hw_threshold", 1168, 1903.5, 3071.5);

  int n = 0;
  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {

    std::cout << "processing event: " << n << std::endl;

    auto const& rawdigits = *ev.getValidHandle<vector<raw::RawDigit>>(rawdigits_tag);
    auto const& wires = *ev.getValidHandle<vector<recob::Wire>>(wires_tag);
    auto const& deconwires = *ev.getValidHandle<vector<recob::Wire>>(decons_tag);
    auto const& thresholds = *ev.getValidHandle<vector<double>>(thres_tag);

    if (rawdigits.empty()) {
      std::cout << "WARNING: no RawDigit found." << std::endl;
      return;
    }

    if (wires.empty()) {
      std::cout << "WARNING: no recob::Wire found." << std::endl;
      return;
    }

    if (deconwires.empty()) {
      std::cout << "WARNING: no recob::Wire found for decon." << std::endl;
      return;
    }

    if (thresholds.empty()) {
      std::cout << "WARNING: no threshold found for decon." << std::endl;
      return;
    }

    for (auto rd: rawdigits) {
      // std::cout << "channel: " << rd.Channel()
      //           << " NADC: " << rd.NADC()
      //           << " Samples: " << rd.Samples()
      //           << " Pedestal: " << rd.GetPedestal()
      //           << " Sigma: " << rd.GetSigma()
      //           << std::endl;
      // hsigma->SetBinContent(rd.Channel()+1, rd.GetSigma());

      int channel = rd.Channel();

      // std::cout << "channel: " << channel << " threshold: " << thresholds.at(channel) << std::endl;

      if (channel < 952){
        hu_baseline->SetBinContent(channel+1, rd.GetPedestal());
        hu_threshold->SetBinContent(channel+1, thresholds.at(channel));
      }
      else if (channel < 1904){
        hv_baseline->SetBinContent(channel-952 +1, rd.GetPedestal());
        hv_threshold->SetBinContent(channel-952 +1, thresholds.at(channel));
      }
      else if (channel < 3072){
        hw_baseline->SetBinContent(channel-1904 +1, rd.GetPedestal());
        hw_threshold->SetBinContent(channel-1904 +1, thresholds.at(channel));
      }

      int nSamples = rd.Samples();
      for (int j = 0; j < nSamples; j++) {
        if (channel < 952){
          hu_orig->SetBinContent(channel+1, j+1, rd.ADC(j));
        }
        else if (channel < 1904){
          hv_orig->SetBinContent(channel-952 +1, j+1, rd.ADC(j));
        }
        else if (channel < 3072){
          hw_orig->SetBinContent(channel-1904 +1, j+1, rd.ADC(j));
        }
      }
    } // end of rawdigits

    for (auto wire: wires) {

      int channel = wire.Channel();
      int nSamples = wire.NSignal();
      std::vector<float> signal = wire.Signal();

      for (int j = 0; j < nSamples; j++) {
        if (channel < 952){
          hu_raw->SetBinContent(channel+1, j+1, signal.at(j));
        }
        else if (channel < 1904){
          hv_raw->SetBinContent(channel-952 +1, j+1, signal.at(j));
        }
        else if (channel < 3072){
          hw_raw->SetBinContent(channel-1904 +1, j+1, signal.at(j));
        }
      }
    } // end of wires

    for (auto wire: deconwires) {

      int channel = wire.Channel();
      int nSamples = wire.NSignal();
      std::vector<float> signal = wire.Signal();

      for (int j = 0; j < nSamples; j++) {
        if (channel < 952){
          hu_decon->SetBinContent(channel+1, j+1, signal.at(j) * 200.);
        }
        else if (channel < 1904){
          hv_decon->SetBinContent(channel-952 +1, j+1, signal.at(j) * 200.);
        }
        else if (channel < 3072){
          hw_decon->SetBinContent(channel-1904 +1, j+1, signal.at(j) * 200.);
        }
      }
    } // end of decon wires

    auto ofile = TFile::Open(Form("magnify-evt%d.root", n),"recreate");
    hu_orig->Write();
    hv_orig->Write();
    hw_orig->Write();
    hu_baseline->Write();
    hv_baseline->Write();
    hw_baseline->Write();
    hu_raw->Write();
    hv_raw->Write();
    hw_raw->Write();
    hu_decon->Write();
    hv_decon->Write();
    hw_decon->Write();
    hu_threshold->Write();
    hv_threshold->Write();
    hw_threshold->Write();
    ofile->Close();
      
    n++;
    if (n>5) break;
  }

  // hsigma->Draw();

}
