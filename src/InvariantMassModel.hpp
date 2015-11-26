// A simple 1D Gaussian shape for a particle's resonance with an exponential
// background... very simplified version of a B meson decay. 'x' is mass.
#ifndef INVARIANTMASSMODEL_HPP
#define INVARIANTMASSMODEL_HPP

#include <RooDataSet.h>
#include <RooGaussian.h>
#include <RooExponential.h>
#include <RooAddPdf.h>
#include <RooArgList.h>

#include "Model.hpp"

class InvariantMassModel : public Model
{
 public:
  InvariantMassModel(TString name, RooRealVar &x):
      Model(name), _x(x), _sig(NULL), _bkg(NULL), _pdf(NULL)
  { this->build(); }
  ~InvariantMassModel()
  {
    delete _pdf;
    for (auto p: _pars)
      delete p;
  }
  RooDataSet* generate(int n=10000)
  { return _pdf->generate(RooArgSet(_x), n); }
  RooAbsPdf* pdf() { return _pdf; }

 private:
  void build();
  RooRealVar&     _x;
  RooGaussian*    _sig;
  RooExponential* _bkg;
  RooAddPdf*      _pdf;
};

inline void InvariantMassModel::build()
{
  RooRealVar* m = new RooRealVar("mean", "#bar{x}", 5280, 4800, 5700);
  RooRealVar* s = new RooRealVar("sigma", "#sigma", 10, 0.1, 100);
  RooRealVar* k = new RooRealVar("lambda", "#lambda", 0.003, 0.0, 0.01);
  RooRealVar* f = new RooRealVar("f_sig", "f_{sig}", 0.5, 0.0, 1.0);
  _pars.push_back(m);
  _pars.push_back(s);
  _pars.push_back(k);
  _pars.push_back(f);
  _sig = new RooGaussian(_name+"_sig", _name, _x, *m, *s);
  _bkg = new RooExponential(_name+"_bkg", _name, _x, *k);
  _pdf = new RooAddPdf(_name+"_pdf", _name, RooArgList(*_sig, *_bkg), *f);
  return;
}

#endif // INVARIANTMASSMODEL_HPP
