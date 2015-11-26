#include <TCanvas.h> // <-| 
#include <RooPlot.h> // <-- make more sense to move the plotting elsewhere?
#include <TH1.h>     // <-|
#include <RooFit.h>
#include <RooMinuit.h>

#include "Fitter.hpp"

void Fitter::fit()
{
  namespace rf=RooFit; // shorthand for the namespace
  RooMinuit minimiser(*_nll);
  _migrad = minimiser.migrad(); // call migrad, catch its return code
  _hesse  = minimiser.hesse();  // call hesse, catch its return code
  _rfr    = minimiser.save();
  return;
}

void Fitter::plot(const TString& name, const RooRealVar* x) //, RooRealVar* y)
{
  RooPlot* rp = x->frame();
  _data->plotOn(rp);
  _model->pdf()->plotOn(rp);
  TCanvas canvas;
  rp->Draw();
  canvas.SaveAs(name+".pdf");
  delete rp;
  return;
}

// Likelihood profiles with inbuild RooFit methods the key method called here:
// RooAbsReal::createProfile creates a RooProfileLL object behind the scenes
//
// https://root.cern.ch/doc/master/RooAbsReal_8cxx_source.html#l00463
// https://root.cern.ch/doc/master/RooProfileLL_8cxx_source.html#l00193
void Fitter::plotNLLProfile(RooRealVar* par)
{
  if (!_migrad<0) {
    std::cout << "Fitter::plotNLLScan ERROR you should fit first,"
        " otherwise the NLL is not minimised" << std::endl;
    return;
  }
  RooAbsReal* pnll = _nll->createProfile(*par); // creates RooProfileLL
  RooPlot* rp = par->frame();
  pnll->plotOn(rp);
  TCanvas canvas;
  rp->Draw();
  canvas.SaveAs(TString(par->GetName())+"_nll_profile.pdf");
  delete pnll;
  delete rp;
  return;      
}
void Fitter::plotNLLProfile(RooRealVar* parx, RooRealVar* pary)
{
  if (!_migrad<0) {
    std::cout << "Fitter::plotNLLScan ERROR you should fit first,"
        " otherwise the NLL is not minimised" << std::endl;
    return;
  }
  RooAbsReal* pnll = _nll->createProfile(RooArgSet(*parx, *pary));
  typedef TString str;
  str commasep = str(parx->GetName()) + str(",") + str(pary->GetName());
  TH1* h = pnll->createHistogram(commasep,50,50);
  h->SetLineColor(kBlue);
  TCanvas canvas;
  h->Draw("colz");
  canvas.SaveAs(commasep.ReplaceAll(",","_")+"_nll_profile.pdf");
  delete h;
  delete pnll;
  return;
}

// Likelihood SCANs are just plotting the value of the likelihood as a function
// of the parameters where all other parameters are at their current values
// (force the NLL to be minimised first)
//
// This is not actually the correct thing to do, really all other (nuisance)
// parameters ought to be marginalised (refit to put them at their minima) for
// each point in the NLL. The correct (but significantly slower) thing is
// implemented above as the PROFILE methods.
void Fitter::plotNLLScan(RooRealVar* par)
{
  if (!_migrad<0) {
    std::cout << "Fitter::plotNLLScan ERROR you should fit first,"
        " otherwise the NLL is not minimised" << std::endl;
    return;
  }
  RooPlot* rp = par->frame();
  _nll->plotOn(rp);
  TCanvas canvas;
  rp->Draw();
  canvas.SaveAs(TString(par->GetName())+"_nll_scan.pdf");
  delete rp;
  return;      
}
void Fitter::plotNLLScan(RooRealVar* parx, RooRealVar* pary)
{
  if (!_migrad<0) {
    std::cout << "Fitter::plotNLLScan ERROR you should fit first,"
        " otherwise the NLL is not minimised" << std::endl;
    return;
  }
  typedef TString str;
  str commasep = str(parx->GetName()) + str(",") + str(pary->GetName());
  std::cout << "Fitter::plotNLLScan INFO: scanning 100 points in the NLL" 
      << std::endl;
  TH1* h = _nll->createHistogram(commasep,50,50);
  h->SetLineColor(kBlue);
  TCanvas canvas;
  h->Draw("colz");
  canvas.SaveAs(commasep.ReplaceAll(",","_")+"_nll_scan.pdf");
  delete h;
  return;      
}
