#include <TCanvas.h> // <-| 
#include <RooPlot.h> // <-- make more sense to move the plotting elsewhere?
#include <RooFit.h>
#include <RooMinuit.h>

#include "Fitter.hpp"

const bool DEBUG = 1;

void Fitter::fit()
{
  if (DEBUG) {
    std::cout << "Fitter::fit _nll = ";
    std::cout << _nll << std::endl;
  }
  namespace rf=RooFit; // shorthand for the namespace
  RooMinuit minimiser(*_nll);
  _migrad = minimiser.migrad(); // call migrad, catch its return code
  _hesse  = minimiser.hesse();  // call hesse, catch its return code
  _rfr    = minimiser.save();
  _nll->Print();
  std::cout << _nll << std::endl;
  return;
}

void Fitter::plotNLLScan(RooRealVar* par)
{
  //std::cout << _pdf << std::endl;
  std::cout << _nll << std::endl;
  std::cout << _rfr << std::endl;
  // just a simple scan of the NLL
  if (!_nll) {
    std::cout << "Fitter::plotNLLScan ERROR you should fit first,"
        " otherwise the NLL is not minimised" << std::endl;
    return;
  }
  RooPlot* rp = par->frame();
  _nll->plotOn(rp);
  TCanvas canvas;
  rp->Draw();
  canvas.SaveAs(TString(par->GetName())+"_nll_scan.pdf");
  return;      
}
